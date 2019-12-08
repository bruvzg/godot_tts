# Godot text-to-speech module (libgdtts)

This GDNative module implements minimal native text-to-speech interface for Godot Engine on iOS, Linux, macOS and Windows.

## Dependencies:

- Godot 3.1+
- C++14 compiler
- Meson build system
- SCons build system
- Speech Dispatcher library and development files (Linux only)

## Compiling

### Build options

Name | Description | Default value
--- | --- | ---
`godot-cpp-lib-name` | godot-cpp static library name (without `.a` or `.lib` extension) | `libgodot-cpp`
`static-lib` | Build static library | `false`
`static-runtime` | Link libgcc and libstdc++ statically | `false`
`static-speech-dispatcher` | Link speech-dispatcher statically | `false`
`builtin-runtime` | Use the built-in libraries | `false`

### Building `godot-cpp` static library

See [Compiling the cpp bindings library](https://github.com/GodotNativeTools/godot-cpp/blob/master/README.md#compiling-the-cpp-bindings-library)

### Building `libgdtts` module

See [Compiling for Windows, macOS and Linux](https://bruvzg.github.io/godot_tts/docs/html/development/compiling_desktop.html#doc-compile-desktop) and [Compiling for iOS](https://bruvzg.github.io/godot_tts/docs/html/development/compiling_ios.html#doc-compile-ios) for build instructions.


## Documentation

[Documentation](https://bruvzg.github.io/godot_tts/docs/html/index.html)

[Class reference](https://bruvzg.github.io/godot_tts/docs/html/classes/index.html)

## License

- The source code for the **libgdtts** module is released under unlicense.
For more information, see http://unlicense.org/ or the accompanying UNLICENSE file.

- **Godot** and **GodotNativeTools** are licensed under MIT license.
For more information, see https://github.com/godotengine/godot/blob/master/LICENSE.txt.

- **Speech Dispatcher** C API library is licensed under GNU Lesser General Public License 2.1 or later.
For more information, see https://github.com/brailcom/speechd/blob/master/src/api/c/libspeechd.c

- **Speech Dispatcher** backend is licensed under GNU General Public License.
For more information, see (https://github.com/brailcom/speechd/blob/master/COPYING.