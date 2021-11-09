/*************************************************************************/
/*  tts_common.h                                                         */
/*************************************************************************/

#ifndef TTS_COMMON_H
#define TTS_COMMON_H

#include <godot_cpp/godot.hpp>

#include <godot_cpp/core/binder_common.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/core/error_macros.hpp>
#include <godot_cpp/core/memory.hpp>
#include <godot_cpp/core/object.hpp>

#include <godot_cpp/variant/array.hpp>
#include <godot_cpp/variant/string.hpp>

namespace godot {

struct TTSDriverPrivate;
class TTSDriver : public Object {
	GDCLASS(TTSDriver, Object);

	TTSDriverPrivate *data = nullptr;
	static TTSDriver *singleton;

protected:
	static void _bind_methods();

public:
	static TTSDriver *get_singleton() {
		return singleton;
	}

	void speak(const String &p_text, bool p_interrupt = false);
	void stop();

	bool is_speaking() const;

	Array get_voices() const;

	void set_voice(const String &p_voice);
	String get_voice() const;

	void set_volume(int p_volume);
	int get_volume() const;

	void set_pitch(float p_pitch);
	float get_pitch() const;

	void set_rate(int p_rate);
	int get_rate() const;

	TTSDriver();
	~TTSDriver();
};

} /* namespace godot */

#endif // TTS_COMMON_H
