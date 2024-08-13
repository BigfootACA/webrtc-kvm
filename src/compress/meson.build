webrtc_kvm_src += files(['compress.cpp'])
if zlib.found()
	webrtc_kvm_src += files(['zlib.cpp'])
endif
if libzstd.found()
	webrtc_kvm_src += files(['zstd.cpp'])
endif
if libbrotlidec.found() and libbrotlienc.found()
	webrtc_kvm_src += files(['brotli.cpp'])
endif
if bzip2.found()
	webrtc_kvm_src += files(['bzip2.cpp'])
endif
