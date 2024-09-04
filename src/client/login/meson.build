webrtc_kvm_src_client += files([
	'login.cpp',
])

webrtc_kvm_src_client += qt6.compile_moc(
	headers: [
		'login.h',
	],
	include_directories: inc,
	dependencies: qt6_dep,
)
