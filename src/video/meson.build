webrtc_kvm_src_video = files([])
subdir('factory')
webrtc_kvm_src_video += webrtc_kvm_src_video_factory
subdir('ffmpeg')
webrtc_kvm_src_video += webrtc_kvm_src_video_ffmpeg
subdir('link')
webrtc_kvm_src_video += webrtc_kvm_src_video_link
subdir('list')
webrtc_kvm_src_video += webrtc_kvm_src_video_list
subdir('noop')
webrtc_kvm_src_video += webrtc_kvm_src_video_noop
subdir('posix')
webrtc_kvm_src_video += webrtc_kvm_src_video_posix
subdir('stream')
webrtc_kvm_src_video += webrtc_kvm_src_video_stream
subdir('v4l2')
webrtc_kvm_src_video += webrtc_kvm_src_video_v4l2
if rkmpp.found() and rkvpu.found()
	subdir('rkmpp')
	webrtc_kvm_src_video += webrtc_kvm_src_video_rkmpp
endif
