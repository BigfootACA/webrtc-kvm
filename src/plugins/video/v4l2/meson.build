webrtc_kvm_src_plugins_video_v4l2 = files([])

v4l2_src = files([
	'v4l2.cpp',
])
subdir('cap')
v4l2_src += webrtc_kvm_src_video_v4l2_cap
subdir('dev')
v4l2_src += webrtc_kvm_src_video_v4l2_dev
subdir('m2m')
v4l2_src += webrtc_kvm_src_video_v4l2_m2m
subdir('media')
v4l2_src += webrtc_kvm_src_video_v4l2_media
subdir('out')
v4l2_src += webrtc_kvm_src_video_v4l2_out

v4l2_plugin = shared_library(
	'video_v4l2',
	v4l2_src,
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

webrtc_kvm_plugins += v4l2_plugin
