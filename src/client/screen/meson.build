webrtc_kvm_src_client += files([
	'connection.cpp',
	'display.cpp',
	'event.cpp',
	'handshake.cpp',
	'http.cpp',
	'login.cpp',
	'screen.cpp',
	'slots.cpp',
	'webrtc.cpp',
])

subdir('input')
subdir('video')

webrtc_kvm_src_client += qt6.compile_moc(
	headers: [
		'screen.h',
	],
	include_directories: inc,
	dependencies: qt6_dep,
)
