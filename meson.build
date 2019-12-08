project(
    'libgdtts',
    'cpp',
    meson_version : '>= 0.38'
)
library_version = '0.1.0'

godot_cpp = meson.get_compiler('cpp').find_library(get_option('godot-cpp-lib-name'), dirs: join_paths(meson.current_source_dir(), 'godot-cpp', 'bin'))

prefix = ''
objcpp_args = []
cpp_args = []
link_args = []
dependencies = [
    godot_cpp
]
sources = [
    'src/tts_driver.cpp',
    'src/tts_core.cpp'
]

if get_option('use-ubsan')
    cpp_args += ['-fsanitize=undefined']
    link_args += ['-fsanitize=undefined']
endif

if get_option('use-asan')
    cpp_args += ['-fsanitize=address']
    link_args += ['-fsanitize=address']
endif

if get_option('use-lsan')
    cpp_args += ['-fsanitize=leak']
    link_args += ['-fsanitize=leak']
endif

if get_option('use-tsan')
    cpp_args += ['-fsanitize=thread']
    link_args += ['-fsanitize=thread']
endif

if host_machine.system() == 'darwin'
    add_languages('objcpp')
    sources += [
        'src/tts_nsspeech.mm'
    ]
    objcpp_args += [
        '-std=c++14',
        '-Wwrite-strings',
    ]
    cpp_args += [
        '-std=c++14',
        '-Wwrite-strings',
    ]
    link_args += [
        '-framework', 'Foundation',
        '-framework', 'Appkit',
        '-framework', 'Cocoa',
        '-Wl,-undefined,dynamic_lookup',
    ]
elif host_machine.system() == 'ios'
    add_languages('objcpp')
    sources += [
        'src/tts_avspeech.mm'
    ]
    objcpp_args += [
        '-std=c++14',
        '-Wwrite-strings',
    ]
    cpp_args += [
        '-std=c++14',
        '-Wwrite-strings',
    ]
    link_args += [
        '-framework', 'Foundation',
        '-framework', 'AVFoundation',
        '-framework', 'Cocoa'
    ]
elif host_machine.system() == 'linux'
    dependencies += dependency('speech-dispatcher', required: true, static : get_option('static-speech-dispatcher'))
    if get_option('static-runtime')
        link_args += [
            '-static-libgcc',
            '-static-libstdc++'
        ]
    endif
    sources += [
        'src/tts_libspeechd.cpp'
    ]
    cpp_args += [
        '-std=c++14',
        '-fPIC',
        '-Wwrite-strings',
    ]
    link_args += [
        '-ldl',
        '-Wl,--no-undefined',
        '-Wl,-R,\'\$ORIGIN\'',
    ]
elif host_machine.system() == 'android'
    #TODO
    error('Text-to-speech is not implemented on this platform!')
elif host_machine.system() == 'windows'
    sources += [
        'src/tts_sapi.cpp'
    ]
    if meson.get_compiler('cpp').get_id() == 'msvc'
        prefix = 'lib'
        cpp_args += [
            '/MD',
            '/TP',
            '/EHsc'
        ]
        link_args += [
            '/WX',
            'kernel32.lib',
            'ole32.lib',
            'oleaut32.lib',
            'sapi.lib'
        ]
    else
        if get_option('static-runtime')
            link_args += [
                '-static',
                '-static-libgcc',
                '-static-libstdc++'
            ]
        endif
        cpp_args += [
            '-std=c++14',
            '-Wwrite-strings',
        ]
        link_args += [
            '-lkernel32',
            '-lole32',
            '-loleaut32',
            '-lsapi',
            '-Wl,--no-undefined',
        ]
    endif
else
    error('Text-to-speech is not implemented on this platform!')
endif

if host_machine.system() == 'ios'
    static_library(
        prefix + 'gdtts',
        sources,
        dependencies : dependencies,
        link_args : link_args,
        cpp_args : cpp_args,
        objcpp_args : objcpp_args,
        include_directories : include_directories('src', 'godot-cpp/include', 'godot-cpp/include/core', 'godot-cpp/include/gen', 'godot-cpp/godot_headers')
    )
else
    shared_library(
        prefix + 'gdtts',
        sources,
        dependencies : dependencies,
        link_args : link_args,
        cpp_args : cpp_args,
        objcpp_args : objcpp_args,
        include_directories : include_directories('src', 'godot-cpp/include', 'godot-cpp/include/core', 'godot-cpp/include/gen', 'godot-cpp/godot_headers')
    )
endif
