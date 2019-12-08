.. _doc_compile_ios:

Compiling for iOS
=================

Requirements
^^^^^^^^^^^^

- Meson build system
- SCons build system
- Xcode 10.0 (or later) with the iOS (10.0) SDK and the command line tools.

Building `godot-cpp` static library
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

1. Open a Terminal and navigate to the `godot-cpp` subdirectory.

2. Apply provided patch for iOS build:

::

    $ git apply ../patch_gdcpp_ios.diff

3. Compile `godot-cpp` static libraries for required platforms (`arm64` and `armv7` for real devices and `x86_64` for simulator), by typing:

::

    $ scons platform=ios target=release ios_arch=arm64 generate_bindings=yes
    $ scons platform=ios target=release ios_arch=armv7 generate_bindings=yes
    $ scons platform=ios target=release ios_arch=x86_64 generate_bindings=yes

Building `libgdtts` static library and built-in dependencies
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

1. Navigate to the root directory of the source.
2. Edit iOS SDK and toolchain paths in the Meson cross compile files (`toolchains/ios.{arch}.toolchain`).
3. Configure `libgdtts` static libraries, by typing:

::

    $ meson ./_ios_arm8 -Dgodot-cpp-lib-name=libgodot-cpp.ios.release.arm64 --buildtype=release --cross-file ./toolchains/ios.arm64.toolchain
    $ meson ./_ios_arm7 -Dgodot-cpp-lib-name=libgodot-cpp.ios.release.armv7 --buildtype=release --cross-file ./toolchains/ios.armv7.toolchain
    $ meson ./_ios_x86_64 -Dgodot-cpp-lib-name=libgodot-cpp.ios.release.x86_64 --buildtype=release --cross-file ./toolchains/ios.x86_64.toolchain

4. Compile `libgdtts` static libraries, by typing:

::

    $ ninja -C ./_ios_arm8
    $ ninja -C ./_ios_arm7
    $ ninja -C ./_ios_x86_64

5. (Optional) Bundle `libgdtts` and it's dependencies into one file:

::

    $ libtool -static -o ./_ios_arm8/libgdtts_full.a ./_ios_arm8/libgdtts.a ./godot-cpp/bin/libgodot-cpp.ios.release.arm64.a
    $ libtool -static -o ./_ios_arm7/libgdtts_full.a ./_ios_arm7/libgdtts.a ./godot-cpp/bin/libgodot-cpp.ios.release.armv7.a
    $ libtool -static -o ./_ios_x86_64/libgdtts_full.a ./_ios_x86_64/libgdtts.a ./godot-cpp/bin/libgodot-cpp.ios.release.x86_64.a

6. (Optional) Bundle static libs for all platforms into one "fat" file:

::

    $ lipo -create ./_ios_arm8/libgdtts_full.a ./_ios_arm7/libgdtts_full.a ./_ios_x86_64/libgdtts_full.a -output ./libgdtts/bin/ios.fat/libgdtts.a
