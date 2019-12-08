/*************************************************************************/
/*  tts_driver.h                                                         */
/*************************************************************************/

#pragma once

#include "tts_core.hpp"

#include <String.hpp>
#include <Array.hpp>
#include <Resource.hpp>

using namespace godot;

struct TTSDriverData;
class TTSDriver : public Resource {
	GODOT_CLASS(TTSDriver, Resource);

	TTSDriverData *driver_data;

public:
	void speak(String p_text, bool p_interrupt = false);
	void stop();

	bool is_speaking() const;

	Array get_voices() const;

	void set_voice(String p_voice);
	String get_voice() const;

	void set_volume(int p_volume);
	int get_volume() const;

	void set_rate(int p_rate);
	int get_rate() const;

	void _init();
	static void _register_methods();

	TTSDriver();
	~TTSDriver();
};
