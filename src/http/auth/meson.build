webrtc_kvm_src_http_auth = files([
	'authorization.cpp',
	'password.cpp'
])
if pam.found()
	webrtc_kvm_src_http_auth += files(['pam.cpp'])
else
	webrtc_kvm_src_http_auth += files(['no_pam.cpp'])
endif
