webrtc_kvm_src_video_ffmpeg = files([])
if libavutil.found()
	subdir('shared')
	webrtc_kvm_src_video_ffmpeg += webrtc_kvm_src_video_ffmpeg_shared
	if libswscale.found()
		subdir('swscale')
		webrtc_kvm_src_video_ffmpeg += webrtc_kvm_src_video_ffmpeg_swscale
	endif
endif
