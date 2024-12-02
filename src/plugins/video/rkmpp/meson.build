webrtc_kvm_src_plugins_video_rkmpp = files([])

rkmpp = dependency('rockchip_mpp', required: false, static: static_link)
rkvpu = dependency('rockchip_vpu', required: false, static: static_link)

if rkmpp.found() and rkvpu.found()
	rkmpp_src = files([
		'config_list.cpp',
		'config.cpp',
		'decoder_init.cpp',
		'decoder.cpp',
		'encoder_init.cpp',
		'encoder.cpp',
		'factory.cpp',
		'format.cpp',
		'fourcc.cpp',
		'init.cpp',
		'process.cpp',
		'start.cpp',
	])

	rkmpp_plugin = shared_library(
		'video_rkmpp',
		rkmpp_src,
		dependencies: [
			libyaml_cpp,
			rkmpp,
			rkvpu,
		],
		link_language: 'cpp',
		link_with: [
			webrtc_kvm_lib,
			webrtc_kvm_server,
		],
		include_directories: video_plugin_inc,
		name_prefix: '',
	)

	webrtc_kvm_plugins += rkmpp_plugin
endif
