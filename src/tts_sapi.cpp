/*************************************************************************/
/*  tts_sapi.cpp                                                         */
/*************************************************************************/

#include "tts_driver.hpp"

#include <objbase.h>
#include <sapi.h>
#include <sphelper.h>
#include <winnls.h>
#include <wchar.h>
#include <list>

/*************************************************************************/

struct TTSDriverData {
	ISpVoice *synth;
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
			driver_data->synth->Speak(p_text.unicode_str(), SPF_IS_NOT_XML | SPF_ASYNC | SPF_PURGEBEFORESPEAK, NULL);
		} else {
			driver_data->synth->Speak(p_text.unicode_str(), SPF_IS_NOT_XML | SPF_ASYNC, NULL);
		}
	} else {
		ERR_PRINTS("TTS driver is not initialized!");
	}
}

void TTSDriver::stop() {
	if (driver_data) {
		driver_data->synth->Speak(NULL, SPF_PURGEBEFORESPEAK, NULL);
	} else {
		ERR_PRINTS("TTS driver is not initialized!");
	}
}

bool TTSDriver::is_speaking() const {
	if (driver_data) {
		SPVOICESTATUS pStatus;
		driver_data->synth->GetStatus(&pStatus, NULL);
		return (pStatus.dwRunningState == SPRS_IS_SPEAKING);
	} else {
		ERR_PRINTS("TTS driver is not initialized!");
		return false;
	}
}

Array TTSDriver::get_voices() const {
	Array list;
	if (driver_data) {
		IEnumSpObjectTokens *cpEnum;
		ISpObjectToken *cpVoiceToken;
		ISpDataKey *cpDataKeyAttribs;
		ULONG ulCount = 0;
		HRESULT hr = SpEnumTokens(SPCAT_VOICES, NULL, NULL, &cpEnum);
		if (SUCCEEDED(hr)) {
			hr = cpEnum->GetCount(&ulCount);
			while (SUCCEEDED(hr) && ulCount--) {
				hr = cpEnum->Next(1, &cpVoiceToken, NULL);
				HRESULT hr_attr = cpVoiceToken->OpenKey(SPTOKENKEY_ATTRIBUTES, &cpDataKeyAttribs);
				if (SUCCEEDED(hr_attr)) {
					wchar_t *w_id = 0L;
					wchar_t *w_lang = 0L;
					cpVoiceToken->GetId(&w_id);
					cpDataKeyAttribs->GetStringValue(L"Language", &w_lang);
					LCID locale = wcstol(w_lang, NULL, 16);
					int locale_chars = GetLocaleInfoW(locale, LOCALE_SISO639LANGNAME, NULL, 0);
					int region_chars = GetLocaleInfoW(locale, LOCALE_SISO3166CTRYNAME, NULL, 0);
					wchar_t *w_lang_code = new wchar_t[locale_chars];
					wchar_t *w_reg_code = new wchar_t[region_chars];
					GetLocaleInfoW(locale, LOCALE_SISO639LANGNAME, w_lang_code, locale_chars);
					GetLocaleInfoW(locale, LOCALE_SISO3166CTRYNAME, w_reg_code, region_chars);
					Dictionary voice_d;
					voice_d["name"] = String(w_id);
					voice_d["language"] = String(w_lang_code) + "_" + String(w_reg_code);
					list.push_back(voice_d);
					delete[] w_lang_code;
					delete[] w_reg_code;
					cpDataKeyAttribs->Release();
				}
				cpVoiceToken->Release();
			}
			cpEnum->Release();
		} else {
			ERR_PRINTS("TTS failed to enum voices!");
		}
	} else {
		ERR_PRINTS("TTS driver is not initialized!");
	}
	return list;
}

void TTSDriver::set_voice(String p_voice) {
	if (driver_data) {
		IEnumSpObjectTokens *cpEnum;
		ISpObjectToken *cpVoiceToken;
		ULONG ulCount = 0;
		HRESULT hr = SpEnumTokens(SPCAT_VOICES, NULL, NULL, &cpEnum);
		if (SUCCEEDED(hr)) {
			hr = cpEnum->GetCount(&ulCount);

			while (SUCCEEDED(hr) && ulCount--) {
				wchar_t *w_id = 0L;
				hr = cpEnum->Next(1, &cpVoiceToken, NULL);
				cpVoiceToken->GetId(&w_id);
				if (String(w_id) == p_voice) {
					driver_data->voice_id = p_voice;
					driver_data->synth->SetVoice(cpVoiceToken);
					cpVoiceToken->Release();
					cpEnum->Release();
					return;
				}
				cpVoiceToken->Release();
			}
			cpEnum->Release();
		}
		ERR_PRINTS("TTS failed to set voice!");
		driver_data->voice_id = String();
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
		driver_data->synth->SetVolume(p_volume);
	} else {
		ERR_PRINTS("TTS driver is not initialized!");
	}
}

int TTSDriver::get_volume() const {
	USHORT volume = 0;
	if (driver_data) {
		driver_data->synth->GetVolume(&volume);
	} else {
		ERR_PRINTS("TTS driver is not initialized!");
	}
	return volume;
}

void TTSDriver::set_rate(int p_rate) {
	if (driver_data) {
		driver_data->synth->SetRate(p_rate / 10);
	} else {
		ERR_PRINTS("TTS driver is not initialized!");
	}
}

int TTSDriver::get_rate() const {
	long rate = 0;
	if (driver_data) {
		driver_data->synth->GetRate(&rate);
	} else {
		ERR_PRINTS("TTS driver is not initialized!");
	}
	return rate * 10;
}

void TTSDriver::_init() {
	driver_data = new TTSDriverData();
	if (driver_data) {
		HRESULT hr = CoInitialize(NULL);
		if (SUCCEEDED(hr)) {
			HRESULT hr = CoCreateInstance(CLSID_SpVoice, NULL, CLSCTX_ALL, IID_ISpVoice, (void **)&driver_data->synth);
			if (FAILED(hr)) {
				CoUninitialize();
				ERR_PRINTS("Cannot initialize ISpVoice");
				delete driver_data;
				driver_data = NULL;
			}
		} else {
			ERR_PRINTS("CoInitializeEx failed!");
			delete driver_data;
			driver_data = NULL;
		}
	} else {
		ERR_PRINTS("Cannot initialize DriverData!");
	}
}

TTSDriver::~TTSDriver() {
	if (driver_data) {
		driver_data->synth->Release();
		CoUninitialize();

		delete driver_data;
	}
}
