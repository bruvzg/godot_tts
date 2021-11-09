/*************************************************************************/
/*  tts_libspeechd.cpp                                                   */
/*************************************************************************/

#include "tts_common.h"

#include <set>
#include <speech-dispatcher/libspeechd.h>

namespace godot {

struct TTSDriverPrivate {
	static std::set<size_t> messages;
	String voice;
	SPDConnection *synth = nullptr;

	static void end_of_speech(size_t p_msg_id, size_t p_client_id, SPDNotificationType p_type) {
		messages.erase(p_msg_id);
	}
};

std::set<size_t> TTSDriverPrivate::messages;

/*************************************************************************/

void TTSDriver::speak(const String &p_text, bool p_interrupt) {
	if (data && data->synth) {
		if (p_interrupt) {
			spd_cancel(data->synth);
		}
		int id = spd_say(data->synth, SPD_MESSAGE, p_text.utf8().get_data());
		if (id != -1) {
			data->messages.insert(id);
		}
	} else {
		WARN_PRINT("TTSDriver is not initialized!");
	}
}

void TTSDriver::stop() {
	if (data && data->synth) {
		spd_cancel(data->synth);
	} else {
		WARN_PRINT("TTSDriver is not initialized!");
	}
}

Array TTSDriver::get_voices() const {
	Array list;
	if (data && data->synth) {
		SPDVoice **voices = spd_list_synthesis_voices(data->synth);
		if (voices != nullptr) {
			SPDVoice **voices_ptr = voices;
			while (*voices_ptr != nullptr) {
				Dictionary voice_d;
				voice_d["name"] = String::utf8((*voices_ptr)->name);
				voice_d["language"] = String::utf8((*voices_ptr)->language) + "_" + String::utf8((*voices_ptr)->variant);
				list.push_back(voice_d);

				voices_ptr++;
			}
			free_spd_voices(voices);
		}
	} else {
		WARN_PRINT("TTSDriver is not initialized!");
	}
	return list;
}

void TTSDriver::set_voice(const String &p_voice) {
	if (data && data->synth) {
		if (spd_set_synthesis_voice(data->synth, p_voice.utf8().get_data()) != -1) {
			data->voice = p_voice;
		}
	} else {
		WARN_PRINT("TTSDriver is not initialized!");
	}
}

String TTSDriver::get_voice() const {
	if (data && data->synth) {
		return data->voice;
	} else {
		WARN_PRINT("TTSDriver is not initialized!");
		return String();
	}
}

bool TTSDriver::is_speaking() const {
	if (data && data->synth) {
		return !data->messages.empty();
	} else {
		WARN_PRINT("TTSDriver is not initialized!");
		return false;
	}
}

void TTSDriver::set_volume(int p_volume) {
	if (data && data->synth) {
		spd_set_volume(data->synth, p_volume * 2 - 100);
	} else {
		WARN_PRINT("TTSDriver is not initialized!");
	}
}

int TTSDriver::get_volume() const {
	if (data && data->synth) {
		return spd_get_volume(data->synth) / 2 + 100;
	} else {
		WARN_PRINT("TTSDriver is not initialized!");
		return 0;
	}
}

void TTSDriver::set_pitch(float p_pitch) {
	if (data && data->synth) {
		spd_set_voice_pitch(data->synth, p_pitch * 10.f);
	} else {
		WARN_PRINT("TTSDriver is not initialized!");
	}
}

float TTSDriver::get_pitch() const {
	if (data && data->synth) {
		return spd_get_voice_pitch(data->synth) / 10.f;
	} else {
		WARN_PRINT("TTSDriver is not initialized!");
		return 0;
	}
}

void TTSDriver::set_rate(int p_rate) {
	if (data && data->synth) {
		spd_set_voice_rate(data->synth, p_rate);
	} else {
		WARN_PRINT("TTSDriver is not initialized!");
	}
}

int TTSDriver::get_rate() const {
	if (data && data->synth) {
		return spd_get_voice_rate(data->synth);
	} else {
		WARN_PRINT("TTSDriver is not initialized!");
		return 0;
	}
}

TTSDriver::TTSDriver() {
	data = new TTSDriverPrivate;
	data->synth = spd_open("Godot", nullptr, nullptr, SPD_MODE_THREADED);
	if (data->synth) {
		data->synth->callback_end = data->synth->callback_cancel = data->end_of_speech;
		spd_set_notification_on(data->synth, SPD_END);
		spd_set_notification_on(data->synth, SPD_CANCEL);
		SPDVoice **voice = spd_list_synthesis_voices(data->synth);
		if (voice != nullptr) {
			data->voice = String::utf8((*voice)->name);
			spd_set_synthesis_voice(data->synth, (*voice)->name);
			free_spd_voices(voice);
		}
		print_verbose("Text-to-Speech: Speech Dispatcher initialized.");
	} else {
		delete data;
		data = nullptr;
		ERR_FAIL_MSG("Cannot initialize Speech Dispatcher synthesizer!");
	}
	singleton = this;
}

TTSDriver::~TTSDriver() {
	if (data) {
		spd_close(data->synth);
		delete data;
	}
	singleton = nullptr;
}

} /* namespace godot */
