/*************************************************************************/
/*  tts_nsspeech.mm                                                      */
/*************************************************************************/

#include "tts_driver.hpp"

#include <AppKit/NSSpeechSynthesizer.h>
#include <Foundation/NSString.h>
#include <Foundation/NSThread.h>
#include <list>

/*************************************************************************/

@interface GodotSpeechSynthesizerDelegate : NSObject <NSSpeechSynthesizerDelegate> {
	std::list<String> messages;
};
- (void)discardMessages;
- (void)appendMessage:(const String &)message;
- (bool)isSpeaking;
@end

@implementation GodotSpeechSynthesizerDelegate
- (void)discardMessages {
	messages.clear();
}

- (void)appendMessage:(const String &)message {
	messages.push_back(message);
}

- (void)speechSynthesizer:(NSSpeechSynthesizer *)sender didFinishSpeaking:(BOOL)finishedSpeaking {
	if (messages.size() > 0) {
		messages.pop_front();
		if (messages.size() > 0) {
			[sender startSpeakingString:[NSString stringWithUTF8String:messages.front().utf8().get_data()]];
		}
	}
};

- (bool)isSpeaking {
	return !messages.empty();
}
@end

/*************************************************************************/

struct TTSDriverData {
	GodotSpeechSynthesizerDelegate *delegate;
	NSSpeechSynthesizer *synth;
	String voice_id;

	TTSDriverData() {
		synth = NULL;
		delegate = NULL;
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
			[driver_data->delegate discardMessages];
			[driver_data->synth stopSpeaking];
			[driver_data->delegate appendMessage:p_text];
			[driver_data->synth startSpeakingString:[NSString stringWithUTF8String:p_text.utf8().get_data()]];
		} else {
			[driver_data->delegate appendMessage:p_text];
			if (![driver_data->synth isSpeaking]) {
				[driver_data->synth startSpeakingString:[NSString stringWithUTF8String:p_text.utf8().get_data()]];
			}
		}
	} else {
		ERR_PRINTS("TTS driver is not initialized!");
	}
}

void TTSDriver::stop() {
	if (driver_data) {
		[driver_data->delegate discardMessages];
		[driver_data->synth stopSpeaking];
	} else {
		ERR_PRINTS("TTS driver is not initialized!");
	}
}

bool TTSDriver::is_speaking() const {
	if (driver_data) {
		return [driver_data->synth isSpeaking] || [driver_data->delegate isSpeaking];
	} else {
		ERR_PRINTS("TTS driver is not initialized!");
		return false;
	}
}

Array TTSDriver::get_voices() const {
	Array list;
	if (driver_data) {
		for (NSString *voiceIdentifierString in [NSSpeechSynthesizer availableVoices]) {
			NSString *voiceLocaleIdentifier = [[NSSpeechSynthesizer attributesForVoice:voiceIdentifierString] objectForKey:NSVoiceLocaleIdentifier];
			Dictionary voice_d;
			voice_d[String("name")] = String([voiceIdentifierString UTF8String]);
			voice_d[String("language")] = String([voiceLocaleIdentifier UTF8String]);
			list.push_back(voice_d);
		}
	} else {
		ERR_PRINTS("TTS driver is not initialized!");
	}
	return list;
}

void TTSDriver::set_voice(String p_voice) {
	if (driver_data) {
		if ([driver_data->synth setVoice:[NSString stringWithUTF8String:p_voice.utf8().get_data()]]) {
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
		[driver_data->synth setVolume:p_volume / 100.0];
	} else {
		ERR_PRINTS("TTS driver is not initialized!");
	}
}

int TTSDriver::get_volume() const {
	if (driver_data) {
		return [driver_data->synth volume] * 100.0;
	} else {
		ERR_PRINTS("TTS driver is not initialized!");
		return 0;
	}
}

void TTSDriver::set_rate(int p_rate) {
	if (driver_data) {
		[driver_data->synth setRate:200 + (p_rate)];
	} else {
		ERR_PRINTS("TTS driver is not initialized!");
	}
}

int TTSDriver::get_rate() const {
	if (driver_data) {
		return ([driver_data->synth rate] - 200);
	} else {
		ERR_PRINTS("TTS driver is not initialized!");
		return 0;
	}
}

void TTSDriver::_init() {
	driver_data = new TTSDriverData();
	if (driver_data) {
		driver_data->synth = [[NSSpeechSynthesizer alloc] init];
		if (driver_data->synth) {
			driver_data->delegate = [[GodotSpeechSynthesizerDelegate alloc] init];
			if (driver_data->delegate) {
				[driver_data->synth setDelegate:driver_data->delegate];
			} else {
				ERR_PRINTS("Cannot initialize NSSpeechSynthesizerDelegate");
				[driver_data->synth release];
				delete driver_data;
				driver_data = NULL;
			}
		} else {
			ERR_PRINTS("Cannot initialize NSSpeechSynthesizer!");
			delete driver_data;
			driver_data = NULL;
		}
	} else {
		ERR_PRINTS("Cannot initialize DriverData!");
	}
}

TTSDriver::~TTSDriver() {
	if (driver_data) {
		[driver_data->synth release];
		[driver_data->delegate release];

		delete driver_data;
	}
}
