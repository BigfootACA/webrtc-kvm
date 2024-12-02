webrtc_kvm_src_plugins_video_rkrga = files([])

rkrga = dependency('librga', required: false, static: static_link)

if rkrga.found()
	rkrga_src = files([
		'buffer.cpp',
		'config.cpp',
		'factory.cpp',
		'fourcc.cpp',
		'process.cpp',
		'start.cpp',
		'string.cpp',
	])

	rkrga_plugin = shared_library(
		'video_rkrga',
		rkrga_src,
		dependencies: [
			libyaml_cpp,
			rkrga,
		],
		link_language: 'cpp',
		link_with: [
			webrtc_kvm_lib,
			webrtc_kvm_server,
		],
		include_directories: video_plugin_inc,
		name_prefix: '',
	)

	webrtc_kvm_plugins += rkrga_plugin
endif
