.. _doc_compile_desktop:

Compiling for Windows, macOS and Linux
======================================

Requirements
^^^^^^^^^^^^

- Meson build system
- SCons build system

Building `godot-cpp` static library
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

1. Open a Terminal and navigate to the `godot-cpp` subdirectory.

2. Compile `godot-cpp` static libraries for required platforms (`{os}` - `windows`, `osx` or `linux`), by typing:

::

    $ scons platform={os} target=release bits=64 generate_bindings=yes

Building `libgdtts` static library and built-in dependencies
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

1. Navigate to the root directory of the source.
2. For MinGW cross-build edit toolchain path in the Meson cross compile files (`toolchains/ming.{arch}.toolchain`).
3. Configure `libgdtts` static libraries, by typing:

::

    $ meson ./_{os_build_dir} -Dgodot-cpp-lib-name=libgodot-cpp.{os}.release.{bits} --buildtype=release

4. Compile `libgdtts` static libraries, by typing:

::

    $ ninja -C ./{os_build_dir}

5. Copy `.so`\`.dll`\`.dylib` files to the `libgdtts/bin/{os}.{arch}/`.