webrtc_kvm_src_compress = files([
	'find.cpp',
	'process.cpp',
	'property.cpp',
	'register.cpp',
])
if zlib.found()
	webrtc_kvm_src_compress += files(['zlib.cpp'])
endif
if libzstd.found()
	webrtc_kvm_src_compress += files(['zstd.cpp'])
endif
if libbrotlidec.found() and libbrotlienc.found()
	webrtc_kvm_src_compress += files(['brotli.cpp'])
endif
if bzip2.found()
	webrtc_kvm_src_compress += files(['bzip2.cpp'])
endif
