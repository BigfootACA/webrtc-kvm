webrtc_kvm_src_plugins_video_ffmpeg = files([])
if libavutil.found()
	ffmpeg_src = files([])
	subdir('shared')
	ffmpeg_src += webrtc_kvm_src_video_ffmpeg_shared
	if libswscale.found()
		subdir('swscale')
		ffmpeg_src += webrtc_kvm_src_video_ffmpeg_swscale
	endif

	ffmpeg_plugin = shared_library(
		'video_ffmpeg',
		ffmpeg_src,
		dependencies: [
			libyaml_cpp,
			libavcodec,
			libavformat,
			libavutil,
			libswscale,
		],
		link_language: 'cpp',
		link_with: [
			webrtc_kvm_lib,
			webrtc_kvm_server,
		],
		include_directories: video_plugin_inc,
		name_prefix: '',
	)

	webrtc_kvm_plugins += ffmpeg_plugin
endif
