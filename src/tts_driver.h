/*************************************************************************/
/*  tts_driver.h                                                         */
/*************************************************************************/

#pragma once

#include "array.h"
#include "dictionary.h"
#include "variant.h"
#include "ustring.h"

class TTSDriver {
	static TTSDriver *singleton;

public:
	TTSDriver();

	static TTSDriver *get_singleton();
	void set_singleton();

	static void register_methods(void *p_handle);

	virtual bool initialize() = 0;

	virtual void speak(const String &p_text, bool p_interrupt = false) = 0;
	virtual void stop() = 0;

	virtual bool is_speaking() = 0;

	virtual Array get_voices() = 0;
	virtual void set_voice(const String &p_voice) = 0;

	virtual void set_volume(int p_volume) = 0;
	virtual int get_volume() = 0;

	virtual void set_rate(int p_rate) = 0;
	virtual int get_rate() = 0;

	virtual ~TTSDriver();
};
