webrtc_kvm_src_plugins_video_noop = files([])

noop_src = files([
	'factory.cpp',
	'pipe.cpp',
	'sink.cpp',
])

noop_plugin = shared_library(
	'video_noop',
	noop_src,
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

webrtc_kvm_plugins += noop_plugin
