#include"stream.h"
#include"lib/exception.h"

//int media_init(const char*dev){
//	int ret;
//	int media_fd;
//	struct media_v2_topology*topology=NULL;
//	ret=open(dev,O_RDWR|O_CLOEXEC);
//	if(ret<0)ret_printf(ret,"open media failed: %m\n");
//	media_fd=ret;
//	printf("open %s\n",dev);
//
//	ret=media_get_topology(media_fd,&topology);
//	if(ret!=0)ret_printf(ret,"get topology failed: %m\n");
//
//	struct media_v2_entity*entities=(struct media_v2_entity*)(uintptr_t)topology->ptr_entities;
//
//	for(uint32_t i=0;i<topology->num_entities;i++){
//		struct media_v2_entity*entity=&entities[i];
//		printf("id %u name %s func %x flags %x\n",entity->id,entity->name,entity->function,entity->flags);
//	}
//	return 0;
//}

// int stream_init(struct webrtc_kvm*ctx){
// 	int ret;
// 	//media_init("/dev/media0");
// 	//media_init("/dev/media1");
// 	//media_init("/dev/media2");
// 	//if(!ctx->webrtc)exit(1);
// 	try{
// 		if((ret=dev_open(ctx,&ctx->video.encoder_fd,ctx->video.encoder_path)))
// 			throw ErrnoException("open encoder {} failed",ctx->video.encoder_path);
// 		if((ret=dev_open(ctx,&ctx->video.capture_fd,ctx->video.capture_path)))
// 			throw ErrnoException("open capture {} failed",ctx->video.capture_path);
// 		ctx->video.capture.fd=ctx->video.encoder_fd;
// 		ctx->video.output.fd=ctx->video.encoder_fd;
// 		ctx->video.source.fd=ctx->video.capture_fd;
// 		if((ret=stream_probe(ctx)))throw ErrnoException("probe devices failed");
// 	}catch(std::exception&exc){
// 		if(ctx->video.encoder_fd>0)close(ctx->video.encoder_fd);
// 		if(ctx->video.capture_fd>0)close(ctx->video.capture_fd);
// 		ctx->video.capture.fd=-1;
// 		ctx->video.output.fd=-1;
// 		ctx->video.source.fd=-1;
// 		ctx->video.encoder_fd=-1;
// 		ctx->video.capture_fd=-1;
// 		throw exc;
// 	}
// 	return 0;
// }
