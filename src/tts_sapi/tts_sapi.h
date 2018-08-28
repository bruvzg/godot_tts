/*************************************************************************/
/*  tts_sapi.h                                                           */
/*************************************************************************/

#pragma once

#include "tts_driver.h"

#include <objbase.h>
#include <sapi.h>
#include <list>

class TTSDriverSAPI : public TTSDriver {
	ISpVoice *synth;
	bool co_initialized;

public:
	TTSDriverSAPI();

	bool initialize();

	void speak(const String &p_text, bool p_interrupt);
	void stop();
	bool is_speaking();

	Array get_voices();
	void set_voice(const String &p_voice);

	void set_volume(int p_volume);
	int get_volume();
	void set_rate(int p_rate);
	int get_rate();

	~TTSDriverSAPI();
};

static TTSDriverSAPI singleton;
