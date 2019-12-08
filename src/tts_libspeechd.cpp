/*************************************************************************/
/*  tts_libspeechd.cpp                                                   */
/*************************************************************************/

#include "tts_driver.hpp"

#include <speech-dispatcher/libspeechd.h>
#include <map>
#include <algorithm>

/*************************************************************************/

std::map<size_t, const TTSDriver*> _messages;

void _end_of_speech(size_t msg_id, size_t client_id, SPDNotificationType type) {
	_messages.erase(msg_id);
};

/*************************************************************************/

struct TTSDriverData {
	SPDConnection *synth;
	String voice_id;

	TTSDriverData() {
		synth = NULL;
	}
};

/*************************************************************************/

void TTSDriver::speak(String p_text, bool p_interrupt) {
	if (driver_data) {
		if (driver_data->voice_id == String()) {
			ERR_PRINTS("TTS voice is not set!");
			return;
		}
		if (p_interrupt) {
			spd_cancel(driver_data->synth);
		}
		int id = spd_say(driver_data->synth, SPD_MESSAGE, p_text.utf8().get_data());
		if (id != -1) {
			_messages[id] = this;
		}
	} else {
		ERR_PRINTS("TTS driver is not initialized!");
	}
}

void TTSDriver::stop() {
	if (driver_data) {
		spd_cancel(driver_data->synth);
	} else {
		ERR_PRINTS("TTS driver is not initialized!");
	}
}

bool TTSDriver::is_speaking() const {
	if (driver_data) {
		size_t count = 0;
		for (std::map<size_t, const TTSDriver*>::iterator it = _messages.begin(); it != _messages.end(); ++it) {
			if (it->second == this) count++;
		}
		return (count > 0);
	} else {
		ERR_PRINTS("TTS driver is not initialized!");
		return false;
	}
}

Array TTSDriver::get_voices() const {
	Array list;
	if (driver_data) {
		SPDVoice **voices = spd_list_synthesis_voices(driver_data->synth);
		if (voices != NULL) {
			SPDVoice **voices_ptr = voices;
			while (*voices_ptr != NULL) {
				Dictionary voice_d;
				voice_d["name"] = String((*voices_ptr)->name);
				voice_d["language"] = String((*voices_ptr)->language) + "_" + String((*voices_ptr)->variant);
				list.push_back(voice_d);

				voices_ptr++;
			}
			free_spd_voices(voices);
		}
	} else {
		ERR_PRINTS("TTS driver is not initialized!");
	}
	return list;
}

void TTSDriver::set_voice(String p_voice) {
	if (driver_data) {
		if (spd_set_synthesis_voice(driver_data->synth, p_voice.utf8().get_data()) == 0) {
			driver_data->voice_id = p_voice;
		} else {
			ERR_PRINTS("TTS failed to set voice!");
			driver_data->voice_id = String();
		}
	} else {
		ERR_PRINTS("TTS driver is not initialized!");
	}
}

String TTSDriver::get_voice() const {
	if (driver_data) {
		return driver_data->voice_id;
	} else {
		ERR_PRINTS("TTS driver is not initialized!");
		return String();
	}
}

void TTSDriver::set_volume(int p_volume) {
	if (driver_data) {
		spd_set_volume(driver_data->synth, p_volume * 2 - 100);
	} else {
		ERR_PRINTS("TTS driver is not initialized!");
	}
}

int TTSDriver::get_volume() const {
	if (driver_data) {
		return spd_get_volume(driver_data->synth) / 2 + 100;
	} else {
		ERR_PRINTS("TTS driver is not initialized!");
		return 0;
	}
}

void TTSDriver::set_rate(int p_rate) {
	if (driver_data) {
		spd_set_voice_rate(driver_data->synth, p_rate);
	} else {
		ERR_PRINTS("TTS driver is not initialized!");
	}
}

int TTSDriver::get_rate() const {
	if (driver_data) {
		return spd_get_voice_rate(driver_data->synth);
	} else {
		ERR_PRINTS("TTS driver is not initialized!");
		return 0;
	}
}

void TTSDriver::_init() {
	driver_data = new TTSDriverData();
	if (driver_data) {
		driver_data->synth = spd_open("GodotTTS", NULL, NULL, SPD_MODE_THREADED);
		if (driver_data->synth) {
			driver_data->synth->callback_end = &_end_of_speech;
			driver_data->synth->callback_cancel = &_end_of_speech;
			spd_set_notification_on(driver_data->synth, SPD_END);
			spd_set_notification_on(driver_data->synth, SPD_CANCEL);
		} else {
			ERR_PRINTS("Cannot initialize Speech Dispatcher!");
			delete driver_data;
			driver_data = NULL;
		}
	} else {
		ERR_PRINTS("Cannot initialize DriverData!");
	}
}

TTSDriver::~TTSDriver() {
	if (driver_data) {
		spd_close(driver_data->synth);

		delete driver_data;
	}
}
