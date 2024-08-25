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
    include_directories('src')
]
add_project_arguments(['-include','global.h'], language: 'c')
add_project_arguments(['-include','global.h','-Wno-psabi'], language: 'cpp')

libdatachannel = dependency('LibDataChannel', required: true, static: static_link)
libmicrohttpd = dependency('libmicrohttpd', required: true, static: static_link)
libyaml_cpp = dependency('yaml-cpp', required: true, static: static_link)
libjsoncpp = dependency('jsoncpp', required: true, static: static_link)
libuuid = dependency('uuid', required: true, static: static_link)
openssl = dependency('openssl', required: true, static: static_link)
zlib = dependency('zlib', required: false, static: static_link)
libzstd = dependency('libzstd', required: false, static: static_link)
libbrotlidec = dependency('libbrotlidec', required: false, static: static_link)
libbrotlienc = dependency('libbrotlienc', required: false, static: static_link)
bzip2 = dependency('bzip2', required: false, static: static_link)
pam = dependency('pam', required: false, static: static_link)
x264 = dependency('x264', required: false, static: static_link)
rkmpp = dependency('rockchip_mpp', required: false, static: static_link)
rkvpu = dependency('rockchip_vpu', required: false, static: static_link)
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

webrtc_kvm_src = files([])
subdir('src')

web_builtin_dir = join_paths(meson.current_source_dir(), 'assets/web')
web_builtin_asm = custom_target(
    'web_builtin.S',
    output : 'web_builtin.S',
    input : 'assets/assets.py',
    command : [prog_python, '@INPUT@', '@OUTPUT@', web_builtin_dir, 'web'],
)
webrtc_kvm_src += web_builtin_asm

webrtc_kvm = executable(
    'webrtc-kvm',
    webrtc_kvm_src,
    dependencies: [
        libdatachannel,
        libmicrohttpd,
        libyaml_cpp,
        libjsoncpp,
        libuuid,
        openssl,
        zlib,
        libzstd,
        libbrotlidec,
        libbrotlienc,
        bzip2,
        pam,
        x264,
        rkmpp,
        rkvpu,
    ],
    link_args: link_args,
    link_language: 'cpp',
    install: true,
    include_directories: inc,
)

test('webrtc-kvm test', webrtc_kvm, args : ['-T'])
