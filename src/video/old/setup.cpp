/*
 * Copyright (C) 2019-2023 Paul Kocialkowski <contact@paulk.fr>
 * Copyright (C) 2020-2023 Bootlin
 */
 
#include<sys/mman.h>
#include"webrtc_kvm.h"
#include"lib/log.h"
#include"lib/exception.h"
#include"v4l2.h"
#include"stream.h"

static int v4l2_setup_format(struct webrtc_kvm*ctx,struct v4l2_stream*stream){
	int ret;
	memset(&stream->format,0,sizeof(stream->format));
	if(stream->width==0)stream->width=ctx->video.width;
	if(stream->height==0)stream->height=ctx->video.height;
	stream->format.type=stream->type;
	if(v4l2_type_mplane_check(stream->type)){
		stream->format.fmt.pix_mp.colorspace=V4L2_COLORSPACE_RAW;
	}else{
		stream->format.fmt.pix.colorspace=V4L2_COLORSPACE_RAW;
	}
	if((ret=v4l2_format_get(stream->fd,&stream->format)))
		throw ErrnoException("failed to get stream format");
	v4l2_format_setup_sizeimage(&stream->format,0,stream->size);
	v4l2_format_setup_pixel(&stream->format,stream->width,stream->height,stream->fourcc);
	if((ret=v4l2_format_set(stream->fd,&stream->format)))
		throw ErrnoException("failed to set stream format");
	if((ret=v4l2_format_get(stream->fd,&stream->format)))
		throw ErrnoException("failed to get stream format");
	v4l2_format_sizeimage(&stream->format,0,&stream->size);
	v4l2_format_pixel(&stream->format,&stream->width,&stream->height,NULL);
	stream->plane_count=v4l2_format_planes_count(&stream->format);
	if(stream->plane_count>ARRAY_SIZE(stream->buffers[0].planes)){
		log_warn("too many planes: {}",stream->plane_count);
		stream->plane_count=ARRAY_SIZE(stream->buffers[0].planes);
	}
	return 0;
}

static int v4l2_setup_param(struct webrtc_kvm*ctx){
	int ret;
	ctx->video.source.fourcc=ctx->video.fourcc_pic;
	ctx->video.output.fourcc=ctx->video.fourcc_pic;
	ctx->video.capture.fourcc=ctx->video.fourcc_coded;
	ctx->video.capture.size=1024*1024;
	if((ret=v4l2_setup_format(ctx,&ctx->video.output)))return ret;
	if((ret=v4l2_setup_format(ctx,&ctx->video.capture)))return ret;
	if(ctx->video.output.width!=ctx->video.width||ctx->video.output.height!=ctx->video.height){
		struct v4l2_selection selection={
			.type=v4l2_type_base(ctx->video.output.type),
			.target=V4L2_SEL_TGT_CROP,
			.r={
				.left=0,.top=0,
				.width=ctx->video.width,
				.height=ctx->video.height,
			}
		};
		if((ret=v4l2_selection_set(ctx->video.encoder_fd,&selection)))
			throw ErrnoException("failed to set encoder output selection");
	}
	ctx->video.source.size=ctx->video.output.size;
	if((ret=v4l2_setup_format(ctx,&ctx->video.source)))return ret;
	return 0;
}

int v4l2_cleanup(struct webrtc_kvm*ctx){
	v4l2_buffers_destroy(ctx->video.encoder_fd,ctx->video.output.type,ctx->video.output.memory);
	v4l2_buffers_destroy(ctx->video.encoder_fd,ctx->video.capture.type,ctx->video.capture.memory);
	return 0;
}

static int setup_buffer(struct webrtc_kvm*ctx,struct v4l2_stream*stream){
	int ret;
	uint32_t count=ARRAY_SIZE(stream->buffers);
	struct v4l2_create_buffers cb={.count=count,.memory=stream->memory};
	memcpy(&cb.format,&stream->format,sizeof(cb.format));
	if(stream==&ctx->video.source)for(uint32_t idx_plane=0;idx_plane<stream->plane_count;idx_plane++)
		v4l2_format_setup_sizeimage(&cb.format,idx_plane,MAX(ctx->video.source.size,ctx->video.output.size));
	if((ret=v4l2_buffers_create(stream->fd,&cb)))
		throw ErrnoException("failed to create buffers");
	for(uint32_t idx_buffer=0;idx_buffer<count;idx_buffer++){
		struct v4l2_stream_buffer*buffer=&stream->buffers[idx_buffer];
		memset(&buffer->buffer,0,sizeof(buffer->buffer));
		buffer->buffer.type=stream->type;
		buffer->buffer.memory=stream->memory;
		buffer->buffer.index=idx_buffer;
		if(v4l2_type_mplane_check(stream->type)){
			buffer->buffer.m.planes=buffer->planes;
			buffer->buffer.length=stream->plane_count;
		}
		if((ret=v4l2_buffer_query(stream->fd,&buffer->buffer)))
			throw ErrnoException("failed to query encoder buffer");
		if(stream->memory==V4L2_MEMORY_MMAP)for(uint32_t idx_plane=0;idx_plane<stream->plane_count;idx_plane++){
			uint32_t offset,length;
			if((ret=v4l2_buffer_plane_offset(&buffer->buffer,idx_plane,&offset)))return ret;
			if((ret=v4l2_buffer_plane_length(&buffer->buffer,idx_plane,&length)))return ret;
			buffer->mmap_data[idx_plane]=mmap(NULL,length,PROT_READ|PROT_WRITE,MAP_SHARED,stream->fd,offset);
			if(buffer->mmap_data[idx_plane]==MAP_FAILED)return -errno;
			log_info("buffer {} plane {} size {}",idx_buffer,idx_plane,length);
			if(stream->dmabuf){
				struct v4l2_exportbuffer expbuf={
					.type=stream->type,
					.index=idx_buffer,
					.plane=idx_plane,
					.flags=O_RDWR,
				};
				if((ret=v4l2_export_buffer(stream->fd,&expbuf)))
					throw ErrnoException("failed to export buffer");
				buffer->dmabuf_fds[idx_plane]=expbuf.fd;
			}else buffer->dmabuf_fds[idx_plane]=-1;
		}
	}
	return 0;
}

static int v4l2_encoder_setup_h264(struct webrtc_kvm*ctx){
	int ret;
	#define SET_CTRL(id,val) if((ret=v4l2_control_set_with(ctx->video.encoder_fd,id,val)))return ret
	SET_CTRL(V4L2_CID_MPEG_VIDEO_PREPEND_SPSPPS_TO_IDR,1);
	SET_CTRL(V4L2_CID_MPEG_VIDEO_H264_I_FRAME_QP,ctx->video.opts_h264.qp_i);
	SET_CTRL(V4L2_CID_MPEG_VIDEO_H264_P_FRAME_QP,ctx->video.opts_h264.qp_p);
	SET_CTRL(V4L2_CID_MPEG_VIDEO_H264_LEVEL,V4L2_MPEG_VIDEO_H264_LEVEL_5_0);
	SET_CTRL(V4L2_CID_MPEG_VIDEO_H264_PROFILE,V4L2_MPEG_VIDEO_H264_PROFILE_CONSTRAINED_BASELINE);
	SET_CTRL(V4L2_CID_MPEG_VIDEO_H264_ENTROPY_MODE,V4L2_MPEG_VIDEO_H264_ENTROPY_MODE_CAVLC);
	SET_CTRL(V4L2_CID_MPEG_VIDEO_GOP_CLOSURE,ctx->video.opts_h264.gop_closure);
	SET_CTRL(ctx->video.opts_h264.gop_closure?
		V4L2_CID_MPEG_VIDEO_GOP_SIZE:
		V4L2_CID_MPEG_VIDEO_H264_I_PERIOD,
		ctx->video.opts_h264.gop_size
	);
	#undef SET_CTRL
	return 0;
}

static int v4l2_encoder_setup(struct webrtc_kvm*ctx){
	int ret;
	switch(ctx->video.fourcc_coded){
		case V4L2_PIX_FMT_H264:ret=v4l2_encoder_setup_h264(ctx);break;
		case V4L2_PIX_FMT_HEVC:ret=0;break;
		default:throw InvalidArgument(
			"unsupported coded fourcc type {:c}{:c}{:c}{:c}",
			CHAR_FOURCC(ctx->video.fourcc_coded)
		);
	}
	if(ret)return ret;
	struct v4l2_streamparm streamparm={
		.type=ctx->video.output.type,
		.parm={.output={.timeperframe={
			.numerator=ctx->video.fps*1000,
			.denominator=1000,
		}}}
	};
	return v4l2_parm_set(ctx->video.encoder_fd,&streamparm);
}

static int v4l2_init_buffers(struct v4l2_stream*stream){
	return v4l2_buffers_capabilities_probe(stream->fd,stream->type,stream->memory,&stream->capabilities);
}

static int v4l2_encoder_probe(struct webrtc_kvm*ctx){
	int ret;
	uint32_t cap=0;
	char driver[32],card[32];
	if((ret=v4l2_capabilities_probe(ctx->video.encoder_fd,&cap,driver,card)))
		throw ErrnoException("failed to probe encoder capabilities");
	log_info("Using encoder driver {} card {}",driver,card);
	if(v4l2_capabilities_check(cap,V4L2_CAP_VIDEO_M2M_MPLANE)){
		ctx->video.output.type=V4L2_BUF_TYPE_VIDEO_OUTPUT_MPLANE;
		ctx->video.capture.type=V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
		log_info("Encoder using multi-plane API");
	}else if(v4l2_capabilities_check(cap,V4L2_CAP_VIDEO_M2M)){
		ctx->video.output.type=V4L2_BUF_TYPE_VIDEO_OUTPUT;
		ctx->video.capture.type=V4L2_BUF_TYPE_VIDEO_CAPTURE;
	}else throw InvalidArgument("target encoder device is not a V4L2 Memory-to-Memory");
	return 0;
}

static int v4l2_capture_probe(struct webrtc_kvm*ctx){
	int ret;
	uint32_t cap=0;
	char driver[32],card[32];
	if((ret=v4l2_capabilities_probe(ctx->video.capture_fd,&cap,driver,card)))
		throw ErrnoException("failed to probe capture capabilities");
	log_info("Using capture driver {} card {}",driver,card);
	if(v4l2_capabilities_check(cap,V4L2_CAP_VIDEO_CAPTURE_MPLANE)){
		ctx->video.source.type=V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
		log_info("Capture using multi-plane API");
	}else if(v4l2_capabilities_check(cap,V4L2_CAP_VIDEO_CAPTURE)){
		ctx->video.source.type=V4L2_BUF_TYPE_VIDEO_CAPTURE;
	}else throw InvalidArgument("target capture device is not a V4L2 Capture");
	return 0;
}

static int get_fmt_info(struct webrtc_kvm*ctx){
	int ret;
	struct v4l2_format fmt;
	if(ctx->video.width==0||ctx->video.height==0||ctx->video.fourcc_pic==0){
		uint32_t width=0,height=0,fourcc=0;
		memset(&fmt,0,sizeof(fmt));
		fmt.type=ctx->video.source.type;
		if((ret=v4l2_format_get(ctx->video.capture_fd,&fmt)))
			throw ErrnoException("get source capture format failed");
		v4l2_format_pixel(&fmt,&width,&height,&fourcc);
		if(ctx->video.width==0||ctx->video.height==0)ctx->video.width=width,ctx->video.height=height;
		if(ctx->video.fourcc_pic==0)ctx->video.fourcc_pic=fourcc;
	}
	if(ctx->video.fourcc_coded==0){
		uint32_t fourcc=0;
		memset(&fmt,0,sizeof(fmt));
		fmt.type=ctx->video.capture.type;
		if((ret=v4l2_format_get(ctx->video.encoder_fd,&fmt)))
			throw ErrnoException("get encoder capture format failed");
		v4l2_format_pixel(&fmt,NULL,NULL,&fourcc);
		ctx->video.fourcc_coded=fourcc;
	}
	return 0;
}

int stream_probe(struct webrtc_kvm*ctx){
	int ret;
	if((ret=v4l2_encoder_probe(ctx)))return ret;
	if((ret=v4l2_capture_probe(ctx)))return ret;
	if((ret=get_fmt_info(ctx)))return ret;
	if((ret=v4l2_init_buffers(&ctx->video.output)))return ret;
	if((ret=v4l2_init_buffers(&ctx->video.source)))return ret;
	if(!v4l2_pixel_format_check(ctx->video.source.fd,ctx->video.source.type,ctx->video.fourcc_pic))
		throw InvalidArgument("missing source pixel format");
	if(!v4l2_pixel_format_check(ctx->video.output.fd,ctx->video.output.type,ctx->video.fourcc_pic))
		throw InvalidArgument("missing encoder output pixel format");
	if(!v4l2_pixel_format_check(ctx->video.capture.fd,ctx->video.capture.type,ctx->video.fourcc_coded))
		throw InvalidArgument("missing encoder capture pixel format");
	if((ret=v4l2_setup_param(ctx)))
		throw ErrnoException("setup param failed");
	log_info(
		"Format: {}x{}@{}hz input {:c}{:c}{:c}{:c} output {:c}{:c}{:c}{:c}",
		ctx->video.width,ctx->video.height,ctx->video.fps,
		CHAR_FOURCC(ctx->video.fourcc_pic),CHAR_FOURCC(ctx->video.fourcc_coded)
	);
	if(ctx->video.output.plane_count!=ctx->video.source.plane_count)
		throw InvalidArgument("encoder output and source capture mplane mismatch");
	if((ret=setup_buffer(ctx,&ctx->video.source)))
		throw ErrnoException("setup source buffer failed");
	if((ret=setup_buffer(ctx,&ctx->video.capture)))
		throw ErrnoException("setup encoder capture failed");
	if((ret=setup_buffer(ctx,&ctx->video.output)))
		throw ErrnoException("setup encoder output failed");
	if((ret=v4l2_encoder_setup(ctx)))
		throw ErrnoException("setup encoder failed");
	return 0;
}
