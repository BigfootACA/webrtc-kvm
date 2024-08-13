#include"webrtc_kvm.h"
#include"lib/readable.h"
#include"lib/log.h"
#include"v4l2.h"
#include"stream.h"

int stream_set_key_frame(struct webrtc_kvm*ctx){
	v4l2_control_set_with(ctx->video.encoder_fd,V4L2_CID_MPEG_VIDEO_FORCE_KEY_FRAME,0);
}

static int queue_buffer(struct webrtc_kvm*ctx,struct v4l2_stream*stream,uint32_t idx){
	int ret;
	struct v4l2_stream_buffer*buffer=&stream->buffers[idx];
	if(likely(!buffer->waiting)){
		buffer->buffer.memory=stream->memory;
		buffer->buffer.type=stream->type;
		buffer->buffer.index=idx;
		buffer->waiting=true;
	}
	if(unlikely((ret=v4l2_buffer_queue(stream->fd,&buffer->buffer))))return ret;
	buffer->waiting=false;
	buffer->queued=true;
	return 0;
}

static int queue_output_buffer(struct webrtc_kvm*ctx,uint32_t idx,uint32_t src_idx){
	uint32_t length=0;
	struct v4l2_stream_buffer*buffer=&ctx->video.output.buffers[idx];
	if(likely(!buffer->waiting)){
		v4l2_buffer_plane_length(&buffer->buffer,0,&length);
		v4l2_buffer_setup_plane_length_used(&buffer->buffer,0,length);
		v4l2_buffer_setup_timestamp(&buffer->buffer,ctx->video.frames*1000UL);
		struct v4l2_stream_buffer*src=&ctx->video.source.buffers[src_idx];
		if(ctx->video.source.memory==V4L2_MEMORY_MMAP)switch(ctx->video.output.memory){
			case V4L2_MEMORY_DMABUF:
				for(uint32_t idx_plane=0;idx_plane<ctx->video.output.plane_count;idx_plane++)
					buffer->planes[idx_plane].m.fd=src->dmabuf_fds[idx_plane];
				if(!v4l2_type_mplane_check(ctx->video.output.type))
					buffer->buffer.m.fd=buffer->planes[0].m.fd;
			break;
			case V4L2_MEMORY_MMAP:
				for(uint32_t idx_plane=0;idx_plane<ctx->video.output.plane_count;idx_plane++){
					uint32_t src_len=0;
					v4l2_buffer_plane_length(&src->buffer,idx_plane,&src_len);
					uint32_t used=MIN(length,src_len);
					v4l2_buffer_setup_plane_length_used(&buffer->buffer,idx_plane,used);
					memcpy(buffer->mmap_data[idx_plane],src->mmap_data[idx_plane],used);
				}
			break;
		}
	}
	return queue_buffer(ctx,&ctx->video.output,idx);
}

static int dequeue_buffer(struct v4l2_stream*stream,uint32_t*idx){
	int ret;
	bool mplane=v4l2_type_mplane_check(stream->type);
	struct v4l2_stream_buffer*buffer;
	struct v4l2_plane planes[4];
	struct v4l2_buffer buf={
		.type=stream->type,
		.memory=stream->memory,
	};
	if(mplane){
		buf.m.planes=planes;
		buf.length=stream->plane_count;
	}
	if(idx)*idx=-1;
	v4l2_buffer_dequeue(stream->fd,&buf);
	stream->index=buf.index;
	if(idx)*idx=buf.index;
	buffer=&stream->buffers[stream->index];
	buffer->queued=false;
	memcpy(&buffer->buffer,&buf,sizeof(buf));
	if(mplane){
		memcpy(buffer->planes,planes,sizeof(planes));
		buffer->buffer.m.planes=buffer->planes;
	}
	if(buffer->buffer.flags&V4L2_BUF_FLAG_ERROR){
		stream->error++;
	}else stream->error/=2;
	return 0;
}

static uint32_t find_available(v4l2_stream*stream){
	for(uint32_t idx=0;idx<ARRAY_SIZE(stream->buffers);idx++){
		if(stream->buffers[idx].waiting)continue;
		if(stream->buffers[idx].queued)continue;
		return idx;
	}
	return UINT32_MAX;
}

static int queue_all(struct webrtc_kvm*ctx,struct v4l2_stream*stream){
	int ret;
	for(uint32_t idx=0;idx<ARRAY_SIZE(stream->buffers);idx++){
		if(stream->buffers[idx].queued)continue;
		if(unlikely((ret=queue_buffer(ctx,stream,idx))))return ret;
	}
	return 0;
}

static int queue_waiting(struct webrtc_kvm*ctx,struct v4l2_stream*stream){
	int ret;
	for(uint32_t idx=0;idx<ARRAY_SIZE(stream->buffers);idx++){
		if(!stream->buffers[idx].waiting)continue;
		if(!stream->buffers[idx].queued)continue;
		if(unlikely((ret=queue_buffer(ctx,stream,idx))))return NEG(ret);
		return (int)idx+1;
	}
	return 0;
}

static int queue_waiting_all(struct webrtc_kvm*ctx){
	int ret;
	if(unlikely((ret=queue_waiting(ctx,&ctx->video.source))<0))return ret;
	if(unlikely((ret=queue_waiting(ctx,&ctx->video.output))<0))return ret;
	if(unlikely((ret=queue_waiting(ctx,&ctx->video.capture))<0))return ret;
	return 0;
}

int stream_process_source(struct webrtc_kvm*ctx){
	int ret;
	uint32_t src_idx,enc_idx;
	if(unlikely(!ctx->video.streaming))return 0;
	if(unlikely((ret=queue_waiting_all(ctx))))return ret;
	if(likely((enc_idx=find_available(&ctx->video.output))!=UINT32_MAX)){
		if(unlikely((ret=dequeue_buffer(&ctx->video.source,&src_idx))))return ret;
		if(unlikely((ret=queue_output_buffer(ctx,enc_idx,src_idx))))return ret;
		if(unlikely((ret=queue_buffer(ctx,&ctx->video.capture,enc_idx))))return ret;
		if(unlikely((ret=queue_buffer(ctx,&ctx->video.source,src_idx))))return ret;
	}
	return 0;
}

int stream_process_encoder(struct webrtc_kvm*ctx){
	int ret;
	list*l;
	uint32_t length=0,out_idx,cap_idx;
	if(unlikely(!ctx->video.streaming))return 0;
	if(unlikely((ret=queue_waiting_all(ctx))))return ret;
	if(unlikely((ret=dequeue_buffer(&ctx->video.output,&out_idx))))return ret;
	if(unlikely((ret=dequeue_buffer(&ctx->video.capture,&cap_idx))))return ret;
	//struct v4l2_stream_buffer*out=&ctx->video.output.buffers[out_idx];
	struct v4l2_stream_buffer*cap=&ctx->video.capture.buffers[cap_idx];
	for(uint32_t idx_plane=0;idx_plane<ctx->video.capture.plane_count;idx_plane++){
		v4l2_buffer_plane_length_used(&cap->buffer,idx_plane,&length);
		if(likely(!(cap->buffer.flags&V4L2_BUF_FLAG_ERROR)&&length>0)){
			if(likely((l=list_first(ctx->video.stream_callbacks))))do{
				LIST_DATA_DECLARE(item,l,struct stream_callback*);
				if(unlikely(!item||!item->callback))continue;
				ret=item->callback(ctx,cap->mmap_data[idx_plane],length,item);
				if(unlikely(ret))return ret;
				ctx->video.outbound+=length;
			}while((l=l->next));
		}
	}
	//if(unlikely((ret=queue_buffer(ctx,&ctx->video.source,out->remote))))return ret;
	ctx->video.frames++;
	return 0;
}

int stream_stop(struct webrtc_kvm*ctx){
	char buff[64];
	int ret,err=0;
	if(unlikely(!ctx->video.streaming))return 0;
	if(unlikely((ret=v4l2_stream_off(ctx->video.encoder_fd,ctx->video.capture.type))))err=ret;
	if(unlikely((ret=v4l2_stream_off(ctx->video.encoder_fd,ctx->video.output.type))))err=ret;
	if(unlikely((ret=v4l2_stream_off(ctx->video.capture_fd,ctx->video.source.type))))err=ret;
	epoll_remove_fd(ctx,ctx->video.capture_fd);
	epoll_remove_fd(ctx,ctx->video.encoder_fd);
	log_info("stream stopped");
	log_info("  Outbound: {} ({} bytes)",format_size_float(buff,ctx->video.outbound),ctx->video.outbound);
	log_info("  Frames:   {}",ctx->video.frames);
	ctx->video.frames=0,ctx->video.outbound=0;
	ctx->video.capture.error=0;
	ctx->video.output.error=0;
	ctx->video.source.error=0;
	ctx->video.streaming=false;
	return err;
}

int stream_start(struct webrtc_kvm*ctx){
	int ret;
	if(unlikely(ctx->video.streaming))return 0;
	if(unlikely((ret=v4l2_stream_on(ctx->video.encoder_fd,ctx->video.capture.type))))goto error;
	if(unlikely((ret=v4l2_stream_on(ctx->video.encoder_fd,ctx->video.output.type))))goto error;
	if(unlikely((ret=v4l2_stream_on(ctx->video.capture_fd,ctx->video.source.type))))goto error;
	if(unlikely((ret=epoll_add_fd(ctx,ctx->video.capture_fd))))goto error;
	if(unlikely((ret=epoll_add_fd(ctx,ctx->video.encoder_fd))))goto error;
	if(likely(ctx->video.frames==0)){
		stream_set_key_frame(ctx);
		queue_all(ctx,&ctx->video.source);
	}
	ctx->video.streaming=true;
	return 0;
error:
	stream_stop(ctx);
	return ret;
}

int stream_auto_stop(struct webrtc_kvm*ctx){
	int ret=0;
	if(ctx->video.stream_refcnt<=1&&ctx->video.streaming&&ctx->video.auto_stop){
		log_info("no active client, auto stop stream");
		ret=stream_stop(ctx);
	}
	ctx->video.stream_refcnt--;
	return ret;
}

int stream_auto_start(struct webrtc_kvm*ctx){
	int ret=0;
	if(ctx->video.stream_refcnt<=0&&!ctx->video.streaming){
		log_info("new client request auto start stream");
		ret=stream_start(ctx);
	}
	ctx->video.stream_refcnt++;
	return ret;
}
