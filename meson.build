project(
    'libgdtts2',
    'cpp',
    meson_version : '>= 0.45'
)
library_version = '2.0.0'

godot_cpp = meson.get_compiler('cpp').find_library(get_option('godot-cpp-lib-name'), dirs: join_paths(meson.current_source_dir(), 'godot-cpp', 'bin'))

prefix = ''
cpp_args = []
c_args = []
objcpp_args = []
link_args = []
dependencies = [
    godot_cpp
]
sources = [
    'src/register_types.cpp',
    'src/tts_common.cpp'
]

if host_machine.system() == 'darwin' or host_machine.system() == 'ios'
    add_languages('objcpp')
    sources += [
        'src/tts_avspeech.mm'
    ]
    cpp_args += [
        '-std=c++20'
    ]
    c_args += [
        '-std=c++17',
    ]
    objcpp_args += [
        '-std=c++17',
    ]
    link_args += [
        '-framework', 'Foundation',
        '-framework', 'AVFAudio',
        '-Wl,-undefined,dynamic_lookup',
    ]
elif host_machine.system() == 'linux'
    dependencies += dependency('speech-dispatcher', required: true)
    sources += [
        'src/tts_libspeechd.cpp'
    ]
    cpp_args += [
        '-std=c++17',
        '-fPIC'
    ]
    c_args += [
        '-std=c++17',
        '-fPIC'
    ]
    link_args += [
        '-ldl',
        '-static-libgcc',
        '-static-libstdc++',
        '-Wl,--no-undefined',
        '-Wl,-R,\'\$ORIGIN\''
    ]
elif host_machine.system() == 'windows'
    sources += [
        'src/tts_sapi.cpp'
    ]
    if meson.get_compiler('cpp').get_id() == 'msvc'
        prefix = 'lib'
        cpp_args += [
            '/MT',
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
        cpp_args += [
            '-std=c++17'
        ]
        c_args += [
            '-std=c++17',
        ]
        link_args += [
            '-lkernel32',
            '-lole32',
            '-loleaut32',
            '-lsapi',
            '-static',
            '-static-libgcc',
            '-static-libstdc++',
            '-Wl,--no-undefined',
        ]
    endif
else
    error('Text-to-speech is not implemented on this platform!')
endif

if host_machine.system() == 'ios'
    static_library(
        prefix + 'gdtts2' + '_' + target_machine.system() + '_' + target_machine.cpu_family(),
        sources,
        dependencies : dependencies,
        link_args : link_args,
        cpp_args : cpp_args,
        c_args : c_args,
        objcpp_args : objcpp_args,
        include_directories : include_directories('src', 'godot-cpp/godot-headers', 'godot-cpp/gen/include', 'godot-cpp/include')
    )
else
    shared_library(
        prefix + 'gdtts2' + '_' + target_machine.system() + '_' + target_machine.cpu_family(),
        sources,
        dependencies : dependencies,
        link_args : link_args,
        cpp_args : cpp_args,
        c_args : c_args,
        objcpp_args : objcpp_args,
        include_directories : include_directories('src', 'godot-cpp/godot-headers', 'godot-cpp/gen/include', 'godot-cpp/include')
    )
endif