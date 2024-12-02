webrtc_kvm_src_plugins_video_posix = files([])

posix_src = files([
	'config.cpp',
	'connection.cpp',
	'factory.cpp',
	'initialize.cpp',
	'open.cpp',
	'read.cpp',
	'start.cpp',
	'write.cpp',
])

posix_plugin = shared_library(
	'video_posix',
	posix_src,
	dependencies: [
		libyaml_cpp,
	],
	link_language: 'cpp',
	link_with: [
		webrtc_kvm_lib,
		webrtc_kvm_server,
	],
	include_directories: video_plugin_inc,
	name_prefix: '',
)

webrtc_kvm_plugins += posix_plugin
