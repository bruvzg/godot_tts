project(
    'libgdtts',
    'cpp',
    meson_version : '>= 0.45'
)
library_version = '0.2.0'

prefix = ''
cpp_args = []
link_args = []
dependencies = []
sources = [
    'src/array.cpp',
    'src/dictionary.cpp',
    'src/gdnative.cpp',
    'src/tts_driver.cpp',
    'src/ustring.cpp',
    'src/variant.cpp'
]

if host_machine.system() == 'darwin'
    add_languages('objcpp')
    sources += [
        'src/tts_nsspeech/tts_nsspeech.mm'
    ]
    cpp_args += [
        '-DOS_MACOS',
        '-std=c++14',
        '-Wwrite-strings',
    ]
    link_args += [
        '-framework', 'Foundation',
        '-framework', 'Appkit',
        '-framework', 'Cocoa',
        '-Wl,-undefined,dynamic_lookup',
    ]
elif host_machine.system() == 'linux'
    dependencies += dependency('speech-dispatcher', required: true)
    sources += [
        'src/tts_libspeechd/tts_libspeechd.cpp'
    ]
    cpp_args += [
        '-DOS_LINUX',
        '-std=c++14',
        '-fPIC',
        '-Wwrite-strings',
        '-Wno-attributes'
    ]
    link_args += [
        '-static-libgcc',
        '-static-libstdc++',
        '-Wl,--no-undefined',
        '-Wl,-R,\'\$ORIGIN\'',
    ]
elif host_machine.system() == 'windows'
    sources += [
        'src/tts_sapi/tts_sapi.cpp'
    ]
    if meson.get_compiler('cpp').get_id() == 'msvc'
        prefix = 'lib'
        cpp_args += [
            '/TP'
        ]
        link_args += [
            '/WX',
            'kernel32.lib',
            'ole32.lib',
            'oleaut32.lib',
            'sapi.lib'
        ]
    else
        cpp_args += [
            '-DOS_WINDOWS',
            '-std=c++14',
            '-Wwrite-strings',
        ]
        link_args += [
            '-lkernel32',
            '-lole32',
            '-loleaut32',
            '-lsapi',
            '--static',
            '-static-libgcc',
            '-static-libstdc++',
            '-Wl,--no-undefined',
        ]
    endif
else
    error('Text-to-speech is not implemented on this platform!')
endif

shared_library(
    prefix + 'gdtts' + '.' + target_machine.system() + '.' + target_machine.cpu_family(),
    sources,
    dependencies : dependencies,
    link_args : link_args,
    cpp_args : cpp_args,
    include_directories : include_directories('src', 'godot_headers')
)
