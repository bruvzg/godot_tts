## Godot text-to-speech module (libgdtts)

This GDNative module implements minimal native text-to-speech interface for Godot Engine on Windows, macOS and Linux.

### Dependencies:
#### Linux:
- Godot 3.1 or Godot 3.0.6
- GCC or Clang
- Meson build system
- Speech Dispatcher library and development files (libspeechd)

#### macOS:
- Godot 3.1 or Godot 3.0.6
- Xcode
- Meson build system

#### Windows:
- Godot 3.1 or Godot 3.0.6
- MinGW-W64 or Microsoft Visual C++
- Meson build system

### Binary downloads:
Download [release](https://github.com/bruvzg/godot_tts/releases) archive and extract it into your project directory.

### Compiling:
1. Go to the root directory of the source code.
2. Run the build system:
```sh
meson [build_dir] --buildtype=release
ninja -C [build_dir]
```

### Loading module:
```gdscript
	const TTSDriver = preload("res://TTSDriver.gdns")
	...
	var tts = TTSDriver.new()
```

### Module API:
- `void tts.speak(String text, bool interrupt)`, Begins speaking synthesized text.
- `void tts.stop()`, Stops synthesis in progress.
- `bool tts.is_speaking()`, Returns `true` if the synthesizer is generating speech, `false` otherwise.
- `Array tts.get_voices()`, Returns an `Array` of voice information `Dictionaries`.
Where `String name` is voice identifier and `String language` is language code in `lang_variant` format.
`lang` part is 2 or 3-letter code based on the ISO-639 standard, in lowercase.
`variant` part is engine dependent string describing country, region or/and dialect.
- `void tts.set_voice(String name)`, Sets the speech synthesizer’s current voice.
- `int tts.get_volume()`/`void tts.set_volume(int volume)`, The speech synthesizer’s volume.
The granularity of the volume is engine dependent.
Values may be truncated. Ranges from 0 to 100.
- `int tts.get_rate`/`void tts.set_rate(int rate)`, The speech synthesizer’s rendering rate adjustment.
The granularity of the rate is engine dependent.
Values may be truncated. Ranges from -100 to 100.

### License:
- The source code for the **libgdtts** module is released under unlicense.
For more information, see http://unlicense.org/ or the accompanying UNLICENSE file.
- **Godot** and **GodotNativeTools** are licensed under MIT license.
For more information, see https://github.com/godotengine/godot/blob/master/LICENSE.txt.
- **Speech Dispatcher** C API library is licensed under GNU Lesser General Public License 2.1 or later.
For more information, see https://github.com/brailcom/speechd/blob/master/src/api/c/libspeechd.c
- **Speech Dispatcher** backend is licensed under GNU General Public License.
For more information, see (https://github.com/brailcom/speechd/blob/master/COPYING.
