/*************************************************************************/
/*  tts_sapi.cpp                                                         */
/*************************************************************************/

#include "tts_common.h"

#include <objbase.h>
#include <sapi.h>
#include <sphelper.h>
#include <wchar.h>
#include <winnls.h>

namespace godot {

struct TTSDriverPrivate {
	ISpVoice *synth = nullptr;
	bool co_initialized = false;
	int pitch = 0;
};

/*************************************************************************/

void TTSDriver::speak(const String &p_text, bool p_interrupt) {
	if (data && data->synth) {
		String text;
		DWORD flags = SPF_ASYNC;
		if (data->pitch == 0) {
			text = p_text;
			flags |= SPF_IS_NOT_XML;
		} else {
			text = String("<pitch absmiddle=\"") + String::num_int64(data->pitch, 10) + String("\">") + p_text + String("</pitch>");
			flags |= SPF_IS_XML;
		}
		if (p_interrupt) {
			data->synth->Speak((LPCWSTR)text.utf16().get_data(), flags | SPF_PURGEBEFORESPEAK, nullptr);
		} else {
			data->synth->Speak((LPCWSTR)text.utf16().get_data(), flags, nullptr);
		}
	} else {
		WARN_PRINT("TTSDriver is not initialized!");
	}
}

void TTSDriver::stop() {
	if (data && data->synth) {
		data->synth->Speak(nullptr, SPF_PURGEBEFORESPEAK, nullptr);
	} else {
		WARN_PRINT("TTSDriver is not initialized!");
	}
}

Array TTSDriver::get_voices() const {
	Array list;
	IEnumSpObjectTokens *cpEnum;
	ISpObjectToken *cpVoiceToken;
	ISpDataKey *cpDataKeyAttribs;
	ULONG ulCount = 0;
	HRESULT hr = SpEnumTokens(SPCAT_VOICES, nullptr, nullptr, &cpEnum);
	if (SUCCEEDED(hr)) {
		hr = cpEnum->GetCount(&ulCount);
		while (SUCCEEDED(hr) && ulCount--) {
			hr = cpEnum->Next(1, &cpVoiceToken, nullptr);
			HRESULT hr_attr = cpVoiceToken->OpenKey(SPTOKENKEY_ATTRIBUTES, &cpDataKeyAttribs);
			if (SUCCEEDED(hr_attr)) {
				wchar_t *w_id = 0L;
				wchar_t *w_lang = 0L;
				cpVoiceToken->GetId(&w_id);
				cpDataKeyAttribs->GetStringValue(L"Language", &w_lang);
				LCID locale = wcstol(w_lang, nullptr, 16);

				int locale_chars = GetLocaleInfoW(locale, LOCALE_SISO639LANGNAME, nullptr, 0);
				int region_chars = GetLocaleInfoW(locale, LOCALE_SISO3166CTRYNAME, nullptr, 0);
				wchar_t *w_lang_code = new wchar_t[locale_chars];
				wchar_t *w_reg_code = new wchar_t[region_chars];
				GetLocaleInfoW(locale, LOCALE_SISO639LANGNAME, w_lang_code, locale_chars);
				GetLocaleInfoW(locale, LOCALE_SISO3166CTRYNAME, w_reg_code, region_chars);

				Dictionary voice_d;
				voice_d["name"] = String::utf16((const char16_t *)w_id);
				voice_d["language"] = String::utf16((const char16_t *)w_lang_code) + "_" + String::utf16((const char16_t *)w_reg_code);
				list.push_back(voice_d);

				delete[] w_lang_code;
				delete[] w_reg_code;

				cpDataKeyAttribs->Release();
			}
			cpVoiceToken->Release();
		}
		cpEnum->Release();
	} else {
		WARN_PRINT("TTSDriver is not initialized!");
	}
	return list;
}

void TTSDriver::set_voice(const String &p_voice) {
	if (data && data->synth) {
		IEnumSpObjectTokens *cpEnum;
		ISpObjectToken *cpVoiceToken;
		ULONG ulCount = 0;
		HRESULT hr = SpEnumTokens(SPCAT_VOICES, nullptr, nullptr, &cpEnum);
		if (SUCCEEDED(hr)) {
			hr = cpEnum->GetCount(&ulCount);
			while (SUCCEEDED(hr) && ulCount--) {
				wchar_t *w_id = 0L;
				hr = cpEnum->Next(1, &cpVoiceToken, nullptr);
				cpVoiceToken->GetId(&w_id);
				if (String::utf16((const char16_t *)w_id) == p_voice) {
					data->synth->SetVoice(cpVoiceToken);
					cpVoiceToken->Release();
					cpEnum->Release();
					return;
				}
				cpVoiceToken->Release();
			}
			cpEnum->Release();
		}
	} else {
		WARN_PRINT("TTSDriver is not initialized!");
	}
}

String TTSDriver::get_voice() const {
	if (data && data->synth) {
		ISpObjectToken *cpVoiceToken;
		HRESULT hr = data->synth->GetVoice(&cpVoiceToken);
		if (SUCCEEDED(hr)) {
			wchar_t *w_id = 0L;
			cpVoiceToken->GetId(&w_id);
			String name = String::utf16((const char16_t *)w_id);
			cpVoiceToken->Release();
			return name;
		} else {
			return String();
		}
	} else {
		WARN_PRINT("TTSDriver is not initialized!");
		return String();
	}
}

bool TTSDriver::is_speaking() const {
	if (data->synth) {
		SPVOICESTATUS pStatus;
		data->synth->GetStatus(&pStatus, nullptr);
		return (pStatus.dwRunningState == SPRS_IS_SPEAKING);
	} else {
		WARN_PRINT("TTSDriver is not initialized!");
		return false;
	}
}

void TTSDriver::set_volume(int p_volume) {
	if (data && data->synth) {
		data->synth->SetVolume(p_volume);
	} else {
		WARN_PRINT("TTSDriver is not initialized!");
	}
}

int TTSDriver::get_volume() const {
	USHORT volume = 0;
	if (data && data->synth) {
		data->synth->GetVolume(&volume);
	} else {
		WARN_PRINT("TTSDriver is not initialized!");
	}
	return volume;
}

void TTSDriver::set_pitch(float p_pitch) {
	if (data && data->synth) {
		data->pitch = p_pitch;
	} else {
		WARN_PRINT("TTSDriver is not initialized!");
	}
}

float TTSDriver::get_pitch() const {
	if (data && data->synth) {
		return data->pitch;
	} else {
		WARN_PRINT("TTSDriver is not initialized!");
		return 0;
	}
}

void TTSDriver::set_rate(int p_rate) {
	if (data && data->synth) {
		data->synth->SetRate(p_rate / 10);
	} else {
		WARN_PRINT("TTSDriver is not initialized!");
	}
}

int TTSDriver::get_rate() const {
	long rate = 0;
	if (data && data->synth) {
		data->synth->GetRate(&rate);
	} else {
		WARN_PRINT("TTSDriver is not initialized!");
	}
	return rate * 10;
}

TTSDriver::TTSDriver() {
	data = new TTSDriverPrivate;
	HRESULT hr = CoInitialize(nullptr);
	if (SUCCEEDED(hr)) {
		data->co_initialized = true;
		HRESULT hr = CoCreateInstance(CLSID_SpVoice, nullptr, CLSCTX_ALL, IID_ISpVoice, (void **)&data->synth);
		if (!SUCCEEDED(hr)) {
			data->synth = nullptr;
			if (data->co_initialized) {
				data->co_initialized = false;
				CoUninitialize();
			}
			delete data;
			data = nullptr;
			ERR_FAIL_MSG("Cannot initialize ISpVoice!");
		}
		print_verbose("Text-to-Speech: SAPI initialized.");
	} else {
		delete data;
		data = nullptr;
		ERR_FAIL_MSG("CoInitializeEx failed!");
	}
	singleton = this;
}

TTSDriver::~TTSDriver() {
	if (data) {
		if (data->synth) {
			data->synth->Release();
		}
		if (data->co_initialized) {
			CoUninitialize();
		}
		delete data;
	}
	singleton = nullptr;
}

} /* namespace godot */
