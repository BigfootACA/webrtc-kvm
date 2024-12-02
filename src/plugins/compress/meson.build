webrtc_kvm_src_plugins_compress = files([])
compress_plugin_inc = inc + include_directories('../../compress')

zlib = dependency('zlib', required: false, static: static_link)
if zlib.found()
	zlib_src = files(['zlib.cpp'])
	zlib_plugin = shared_library(
		'compress_zlib',
		zlib_src,
		dependencies: [zlib],
		link_language: 'cpp',
		link_with: [webrtc_kvm_lib],
		include_directories: compress_plugin_inc,
		name_prefix: '',
	)
	webrtc_kvm_plugins += zlib_plugin
endif

libzstd = dependency('libzstd', required: false, static: static_link)
if libzstd.found()
	zstd_src = files(['zstd.cpp'])
	zstd_plugin = shared_library(
		'compress_zstd',
		zstd_src,
		dependencies: [libzstd],
		link_language: 'cpp',
		link_with: [webrtc_kvm_lib],
		include_directories: compress_plugin_inc,
		name_prefix: '',
	)
	webrtc_kvm_plugins += zstd_plugin
endif

libbrotlidec = dependency('libbrotlidec', required: false, static: static_link)
libbrotlienc = dependency('libbrotlienc', required: false, static: static_link)
if libbrotlidec.found() and libbrotlienc.found()
	brotli_src = files(['brotli.cpp'])
	brotli_plugin = shared_library(
		'compress_brotli',
		brotli_src,
		dependencies: [libbrotlidec, libbrotlienc],
		link_language: 'cpp',
		link_with: [webrtc_kvm_lib],
		include_directories: compress_plugin_inc,
		name_prefix: '',
	)
	webrtc_kvm_plugins += brotli_plugin
endif

bzip2 = dependency('bzip2', required: false, static: static_link)
if bzip2.found()
	bzip2_src = files(['bzip2.cpp'])
	bzip2_plugin = shared_library(
		'compress_bzip2',
		bzip2_src,
		dependencies: [bzip2],
		link_language: 'cpp',
		link_with: [webrtc_kvm_lib],
		include_directories: compress_plugin_inc,
		name_prefix: '',
	)
	webrtc_kvm_plugins += bzip2_plugin
endif
