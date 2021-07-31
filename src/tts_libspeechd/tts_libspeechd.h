/*************************************************************************/
/*  tts_libspeechd.h                                                     */
/*************************************************************************/

#pragma once

#include "tts_driver.h"

#include <speech-dispatcher/libspeechd.h>
#include <list>
#include <algorithm>

class TTSDriverSPD : public TTSDriver {
	static std::list<int> messages;
	SPDConnection *synth;

protected:
	static void end_of_speech(size_t msg_id, size_t client_id, SPDNotificationType type);

public:
	TTSDriverSPD();

	bool initialize();

	void speak(const String &p_text, bool p_interrupt);
	void stop();
	bool is_speaking();

	Array get_voices();
	void set_voice(const String &p_voice);

	void set_volume(int p_volume);
	int get_volume();

	void set_pitch(float p_pitch);
	float get_pitch();

	void set_rate(int p_rate);
	int get_rate();

	~TTSDriverSPD();
};

static TTSDriverSPD singleton;
