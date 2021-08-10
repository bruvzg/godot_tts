/*************************************************************************/
/*  tts_libspeechd.cpp                                                   */
/*************************************************************************/

#include "tts_libspeechd.h"

std::list<int> TTSDriverSPD::messages;

void TTSDriverSPD::end_of_speech(size_t msg_id, size_t client_id, SPDNotificationType type) {
	messages.erase(std::find(messages.begin(), messages.end(), msg_id));
};

TTSDriverSPD::TTSDriverSPD() {
	synth = NULL;
};

bool TTSDriverSPD::initialize() {
	synth = spd_open("Godot", NULL, NULL, SPD_MODE_THREADED);
	if (synth) {
		synth->callback_end = synth->callback_cancel = end_of_speech;
		spd_set_notification_on(synth, SPD_END);
		spd_set_notification_on(synth, SPD_CANCEL);
		return true;
	} else {
		DEBUG_PRINT_ERROR("Cannot initialize Speech Dispatcher!");
		return false;
	}
};

void TTSDriverSPD::speak(const String &p_text, bool p_interrupt) {
	if (synth) {
		if (p_interrupt) {
			spd_cancel(synth);
		}
		int id = spd_say(synth, SPD_MESSAGE, p_text.utf8().get_data());
		if (id != -1)
			messages.push_back(id);
	} else {
		DEBUG_PRINT_WARNING("TTS driver is not initialized!");
	}
};

void TTSDriverSPD::stop() {
	if (synth) {
		spd_cancel(synth);
	} else {
		DEBUG_PRINT_WARNING("TTS driver is not initialized!");
	}
};

Array TTSDriverSPD::get_voices() {
	Array list;
	if (synth) {
		SPDVoice **voices = spd_list_synthesis_voices(synth);
		if (voices != NULL) {
			SPDVoice **voices_ptr = voices;
			while (*voices_ptr != NULL) {
				Dictionary voice_d;
				voice_d.set(String("name"), String((*voices_ptr)->name));
				voice_d.set(String("language"), String((*voices_ptr)->language) + "_" + String((*voices_ptr)->variant));
				list.push_back(voice_d);

				voices_ptr++;
			}
			free_spd_voices(voices);
		}
	} else {
		DEBUG_PRINT_WARNING("TTS driver is not initialized!");
	}
	return list;
};

void TTSDriverSPD::set_voice(const String &p_voice) {
	if (synth) {
		spd_set_synthesis_voice(synth, p_voice.utf8().get_data());
	} else {
		DEBUG_PRINT_WARNING("TTS driver is not initialized!");
	}
};

bool TTSDriverSPD::is_speaking() {
	if (synth) {
		return !messages.empty();
	} else {
		DEBUG_PRINT_WARNING("TTS driver is not initialized!");
		return false;
	}
};

void TTSDriverSPD::set_volume(int p_volume) {
	if (synth) {
		spd_set_volume(synth, p_volume * 2 - 100);
	} else {
		DEBUG_PRINT_WARNING("TTS driver is not initialized!");
	}
};

int TTSDriverSPD::get_volume() {
	if (synth) {
		return spd_get_volume(synth) / 2 + 100;
	} else {
		DEBUG_PRINT_WARNING("TTS driver is not initialized!");
		return 0;
	}
};

void TTSDriverSPD::set_pitch(float p_pitch) {
	if (synth) {
		spd_set_voice_pitch(synth, p_pitch);
	} else {
		DEBUG_PRINT_WARNING("TTS driver is not initialized!");
	}
};

float TTSDriverSPD::get_pitch() {
	if (synth) {
		return spd_get_voice_pitch(synth) / 10.0f;
	} else {
		DEBUG_PRINT_WARNING("TTS driver is not initialized!");
		return 0;
	}
};

void TTSDriverSPD::set_rate(int p_rate) {
	if (synth) {
		spd_set_voice_rate(synth, p_rate * 10.0f);
	} else {
		DEBUG_PRINT_WARNING("TTS driver is not initialized!");
	}
};

int TTSDriverSPD::get_rate() {
	if (synth) {
		return spd_get_voice_rate(synth);
	} else {
		DEBUG_PRINT_WARNING("TTS driver is not initialized!");
		return 0;
	}
};

TTSDriverSPD::~TTSDriverSPD() {
	if (synth) {
		spd_close(synth);
		synth = NULL;
	}
};
