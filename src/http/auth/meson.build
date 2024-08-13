webrtc_kvm_src += files([
	'authorization.cpp',
	'password.cpp'
])
if pam.found()
	webrtc_kvm_src += files(['pam.cpp'])
else
	webrtc_kvm_src += files(['no_pam.cpp'])
endif
