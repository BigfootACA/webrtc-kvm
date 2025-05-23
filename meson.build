project(
	'webrtc-kvm',
	['c','cpp'],
	version: '1.0',
	license: 'GPLv3',
	default_options: [
		'werror=true',
		'b_lto=false',
		'cpp_std=gnu++20',
		'optimization=g',
	],
	meson_version: '>=1.1.1',
)

static_link = get_option('static')
link_args = ['-z','noexecstack']
if static_link
	message('use statically link')
	link_args += '-static'
endif
prog_python = find_program('python3')

inc = [
	include_directories('src'),
]
add_project_arguments(['-include','global.h'], language: 'c')
add_project_arguments(['-include','global.h','-Wno-psabi'], language: 'cpp')

qt6 = import('qt6')
qt6_dep = dependency('qt6', required: false, modules: [
	'Core',
	'Gui',
	'Widgets',
	'Network',
])
libdatachannel = dependency('LibDataChannel', required: true, static: static_link)
libmicrohttpd = dependency('libmicrohttpd', required: true, static: static_link)
libyaml_cpp = dependency('yaml-cpp', required: true, static: static_link)
libjsoncpp = dependency('jsoncpp', required: true, static: static_link)
libuuid = dependency('uuid', required: true, static: static_link)
openssl = dependency('openssl', required: true, static: static_link)
pam = dependency('pam', required: false, static: static_link)
libavutil = dependency('libavutil', required: false, static: static_link)
libavcodec = dependency('libavcodec', required: false, static: static_link)
libavformat = dependency('libavformat', required: false, static: static_link)
libswscale = dependency('libswscale', required: false, static: static_link)
libx11 = dependency('x11', required: false, static: static_link)
systemd = dependency('systemd', required: false)
if systemd.found()
	systemd_system_unit_dir = systemd.get_variable('systemdsystemunitdir')
else
	systemd_system_unit_dir = get_option('libdir') / 'systemd' / 'system'
endif

conf_data = configuration_data({
	'bindir': get_option('prefix') / get_option('bindir'),
	'sysconfdir': get_option('prefix') / get_option('sysconfdir'),
})

configure_file(
	input: 'assets/webrtc-kvm.service.in',
	output: 'webrtc-kvm.service',
	install: true,
	install_mode: 'rw-r--r--',
	install_dir: systemd_system_unit_dir,
	configuration : conf_data,
)

install_data(
	'assets/config.yaml',
	install_mode: 'rw-r--r--',
	install_dir: get_option('sysconfdir') / 'webrtc-kvm',
)
install_data(
	'assets/webrtc-kvm.svg',
	install_mode: 'rw-r--r--',
	install_dir: get_option('datadir') / 'icons/hicolor/scalable/apps/',
)

enable_client = get_option('client')
enable_client.disable_if(not qt6_dep.found())
enable_client.disable_if(not libavcodec.found())
enable_client.disable_if(not libavutil.found())
enable_client.disable_if(not libswscale.found())

webrtc_kvm_src = files([])
webrtc_kvm_lib_src = files([])
webrtc_kvm_server_src = files([])
webrtc_kvm_client_src = files([])
webrtc_kvm_plugins = []
plugins_dir = meson.current_build_dir() / 'plugins'
plugins_install_dir = get_option('libdir') / 'webrtc-kvm'
subdir('src')

web_builtin_dir = join_paths(meson.current_source_dir(), 'assets/web')
web_builtin_asm = custom_target(
	'web_builtin.S',
	output : 'web_builtin.S',
	input : 'assets/assets.py',
	command : [prog_python, '@INPUT@', '@OUTPUT@', web_builtin_dir, 'web'],
)
webrtc_kvm_server_src += web_builtin_asm

webrtc_kvm_lib = shared_library(
	'webrtc-kvm-lib',
	webrtc_kvm_lib_src,
	dependencies: [
		libyaml_cpp,
		libjsoncpp,
		libuuid,
		openssl,
	],
	link_language: 'cpp',
	install: true,
	include_directories: inc,
)

webrtc_kvm_server = shared_library(
	'webrtc-kvm-server',
	webrtc_kvm_server_src,
	dependencies: [
		libdatachannel,
		libmicrohttpd,
		libyaml_cpp,
		libjsoncpp,
		pam,
	],
	link_language: 'cpp',
	link_with: [
		webrtc_kvm_lib,
	],
	install: true,
	include_directories: inc,
)

subdir('src/plugins')
plugins_script = join_paths(meson.current_source_dir(), 'assets/plugins.py')
prepare_plugins_dir = custom_target(
	'prepare_plugins_dir',
	input: webrtc_kvm_plugins,
	output: ['plugins'],
	command: [prog_python, plugins_script, 'copy-plugins', plugins_dir, '@INPUT@'],
	build_by_default: true,
)
meson.add_install_script(
	prog_python, plugins_script, 'install-plugins',
	plugins_dir, plugins_install_dir,
)

webrtc_kvm = executable(
	'webrtc-kvm',
	webrtc_kvm_src,
	link_args: link_args,
	link_language: 'cpp',
	link_with: [
		webrtc_kvm_lib,
		webrtc_kvm_server,
	],
	install: true,
	include_directories: inc,
)

if enable_client.enabled()
	executable(
		'webrtc-kvm-client',
		webrtc_kvm_client_src,
		dependencies: [
			libdatachannel,
			libjsoncpp,
			libuuid,
			openssl,
			qt6_dep,
			libavcodec,
			libavutil,
			libswscale,
			libx11,
		],
		link_args: link_args,
		link_language: 'cpp',
		link_with: [
			webrtc_kvm_lib,
		],
		install: true,
		include_directories: inc,
	)
	configure_file(
		input: 'assets/webrtc-kvm-client.desktop.in',
		output: 'webrtc-kvm-client.desktop',
		install: true,
		install_mode: 'rw-r--r--',
		install_dir: get_option('datadir') / 'applications',
		configuration : conf_data,
	)
endif

test('webrtc-kvm test', webrtc_kvm, args : ['-T'])
