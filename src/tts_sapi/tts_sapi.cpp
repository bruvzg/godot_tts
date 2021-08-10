/*************************************************************************/
/*  tts_sapi.cpp                                                         */
/*************************************************************************/

#include "tts_sapi.h"

#include <sphelper.h>
#include <winnls.h>
#include <wchar.h>

TTSDriverSAPI::TTSDriverSAPI() {
	co_initialized = false;
	synth = NULL;
	pitch = 0;
};

bool TTSDriverSAPI::initialize() {
	HRESULT hr = CoInitialize(NULL);
	if (SUCCEEDED(hr)) {
		co_initialized = true;
		HRESULT hr = CoCreateInstance(CLSID_SpVoice, NULL, CLSCTX_ALL, IID_ISpVoice, (void **)&synth);
		if (SUCCEEDED(hr)) {
			return true;
		} else {
			synth = NULL;
			if (co_initialized) {
				co_initialized = false;
				CoUninitialize();
			}
			DEBUG_PRINT_ERROR("Cannot initialize ISpVoice");
			return false;
		}
	} else {
		co_initialized = false;
		DEBUG_PRINT_ERROR("CoInitializeEx failed!");
		return false;
	}
};

void TTSDriverSAPI::speak(const String &p_text, bool p_interrupt) {
	if (synth) {
		String text;
		DWORD flags = SPF_ASYNC;
		if (pitch == 0) {
			text = p_text;
			flags |= SPF_IS_NOT_XML;
		} else {
			text = String("<pitch absmiddle=\"") + String::itos(pitch, 10) + String("\">") + p_text + String("</pitch>");
			flags |= SPF_IS_XML;
		}
		if (p_interrupt) {
			synth->Speak(text.c_str(), flags | SPF_PURGEBEFORESPEAK, NULL);
		} else {
			synth->Speak(text.c_str(), flags, NULL);
		}
	} else {
		DEBUG_PRINT_WARNING("TTS driver is not initialized!");
	}
};

void TTSDriverSAPI::stop() {
	if (synth) {
		synth->Speak(NULL, SPF_PURGEBEFORESPEAK, NULL);
	} else {
		DEBUG_PRINT_WARNING("TTS driver is not initialized!");
	}
};

Array TTSDriverSAPI::get_voices() {
	Array list;
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
				voice_d.set(String("name"), String(w_id));
				voice_d.set(String("language"), String(w_lang_code) + "_" + String(w_reg_code));
				list.push_back(voice_d);
				delete[] w_lang_code;
				delete[] w_reg_code;
				cpDataKeyAttribs->Release();
			}
			cpVoiceToken->Release();
		}
		cpEnum->Release();
	} else {
		DEBUG_PRINT_WARNING("TTS driver is not initialized!");
	}
	return list;
};

void TTSDriverSAPI::set_voice(const String &p_voice) {
	if (synth) {
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
					synth->SetVoice(cpVoiceToken);
					cpVoiceToken->Release();
					cpEnum->Release();
					return;
				}
				cpVoiceToken->Release();
			}
			cpEnum->Release();
		}
	} else {
		DEBUG_PRINT_WARNING("TTS driver is not initialized!");
	}
};

bool TTSDriverSAPI::is_speaking() {
	if (synth) {
		SPVOICESTATUS pStatus;
		synth->GetStatus(&pStatus, NULL);
		return (pStatus.dwRunningState == SPRS_IS_SPEAKING);
	} else {
		DEBUG_PRINT_WARNING("TTS driver is not initialized!");
		return false;
	}
};

void TTSDriverSAPI::set_volume(int p_volume) {
	if (synth) {
		synth->SetVolume(p_volume);
	} else {
		DEBUG_PRINT_WARNING("TTS driver is not initialized!");
	}
};

int TTSDriverSAPI::get_volume() {
	USHORT volume = 0;
	if (synth) {
		synth->GetVolume(&volume);
	} else {
		DEBUG_PRINT_WARNING("TTS driver is not initialized!");
	}
	return volume;
};

void TTSDriverSAPI::set_pitch(float p_pitch) {
	if (synth) {
		pitch = p_pitch;
	} else {
		DEBUG_PRINT_WARNING("TTS driver is not initialized!");
	}
};

float TTSDriverSAPI::get_pitch() {
	if (synth) {
		return pitch;
	} else {
		DEBUG_PRINT_WARNING("TTS driver is not initialized!");
		return 0;
	}
};

void TTSDriverSAPI::set_rate(int p_rate) {
	if (synth) {
		synth->SetRate(p_rate / 10);
	} else {
		DEBUG_PRINT_WARNING("TTS driver is not initialized!");
	}
};

int TTSDriverSAPI::get_rate() {
	long rate = 0;
	if (synth) {
		synth->GetRate(&rate);
	} else {
		DEBUG_PRINT_WARNING("TTS driver is not initialized!");
	}
	return rate * 10;
};

TTSDriverSAPI::~TTSDriverSAPI() {
	if (synth) {
		synth->Release();
		synth = NULL;
	}
	if (co_initialized) {
		co_initialized = false;
		CoUninitialize();
	}
};
