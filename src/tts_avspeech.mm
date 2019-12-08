/*************************************************************************/
/*  tts_avspeech.mm                                                      */
/*************************************************************************/

#include "tts_driver.hpp"

#include <AVFoundation/AVSpeechSynthesis.h>
#include <Foundation/NSString.h>
#include <Foundation/NSThread.h>
#include <list>

/*************************************************************************/

@interface GodotSpeechSynthesizerDelegate : NSObject <AVSpeechSynthesizerDelegate> {
	std::list<AVSpeechUtterance *> messages;
};
- (void)discardMessages;
- (void)appendMessage:(AVSpeechUtterance *)message;
- (bool)isSpeaking;
@end

@implementation GodotSpeechSynthesizerDelegate
- (void)discardMessages {
	messages.clear();
}

- (void)appendMessage:(AVSpeechUtterance *)message {
	messages.push_back(message);
}

- (void)speechSynthesizer:(AVSpeechSynthesizer *)synth didCancelSpeechUtterance:(AVSpeechUtterance *)utterance {
	[utterance release];
	messages.clear();
}

- (void)speechSynthesizer:(AVSpeechSynthesizer *)synth didFinishSpeechUtterance:(AVSpeechUtterance *)utterance {
	[utterance release];
	if (messages.size() > 0) {
		[synth speakUtterance:messages.front()];
		messages.pop_front();
	}
}

- (bool)isSpeaking {
	return !messages.empty();
}
@end

/*************************************************************************/

struct TTSDriverData {
	GodotSpeechSynthesizerDelegate *delegate;
	AVSpeechSynthesizer *synth;
	AVSpeechSynthesisVoice *voice_ref;

	String voice_id;
	float volume;
	float rate;

	TTSDriverData() {
		synth = NULL;
		delegate = NULL;
		voice_ref = NULL;

		volume = 1.0;
		rate = 1.0;
	}
};

/*************************************************************************/

void TTSDriver::speak(String p_text, bool p_interrupt) {
	if (driver_data) {
		if (!driver_data->voice_ref) {
			ERR_PRINTS("TTS voice is not set!");
			return;
		}
		if (p_interrupt) {
			[driver_data->synth stopSpeakingAtBoundary:AVSpeechBoundaryImmediate];

			AVSpeechUtterance *new_utterance = [[AVSpeechUtterance alloc] initWithString:[NSString stringWithUTF8String:p_text.utf8().get_data()]];
			[new_utterance setVoice:driver_data->voice_ref];
			if (driver_data->rate < 0) {
				float rate = (driver_data->rate / 100.f) * (AVSpeechUtteranceDefaultSpeechRate - AVSpeechUtteranceMinimumSpeechRate);
				[new_utterance setRate:(AVSpeechUtteranceDefaultSpeechRate + rate)];
			} else {
				float rate = (driver_data->rate / 100.f) * (AVSpeechUtteranceMaximumSpeechRate - AVSpeechUtteranceDefaultSpeechRate);
				[new_utterance setRate:(AVSpeechUtteranceDefaultSpeechRate + rate)];
			}
			[new_utterance setVolume:driver_data->volume];

			[driver_data->synth speakUtterance:new_utterance];
		} else {
			AVSpeechUtterance *new_utterance = [[AVSpeechUtterance alloc] initWithString:[NSString stringWithUTF8String:p_text.utf8().get_data()]];
			[new_utterance setVoice:driver_data->voice_ref];
			if (driver_data->rate < 0) {
				float rate = (driver_data->rate / 100.f) * (AVSpeechUtteranceDefaultSpeechRate - AVSpeechUtteranceMinimumSpeechRate);
				[new_utterance setRate:(AVSpeechUtteranceDefaultSpeechRate + rate)];
			} else {
				float rate = (driver_data->rate / 100.f) * (AVSpeechUtteranceMaximumSpeechRate - AVSpeechUtteranceDefaultSpeechRate);
				[new_utterance setRate:(AVSpeechUtteranceDefaultSpeechRate + rate)];
			}
			[new_utterance setVolume:driver_data->volume];

			if ([driver_data->synth isSpeaking]) {
				[driver_data->delegate appendMessage:new_utterance];
			} else {
				[driver_data->synth speakUtterance:new_utterance];
			}
		}
	} else {
		ERR_PRINTS("TTS driver is not initialized!");
	}
}

void TTSDriver::stop() {
	if (driver_data) {
		[driver_data->synth stopSpeakingAtBoundary:AVSpeechBoundaryImmediate];
	} else {
		ERR_PRINTS("TTS driver is not initialized!");
	}
}

Array TTSDriver::get_voices() const {
	Array list;
	if (driver_data) {
		for (AVSpeechSynthesisVoice *voice in [AVSpeechSynthesisVoice speechVoices]) {
			NSString *voiceIdentifierString = [voice identifier];
			NSString *voiceLocaleIdentifier = [voice language];
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

bool TTSDriver::is_speaking() const {
	if (driver_data) {
		return [driver_data->synth isSpeaking] || [driver_data->delegate isSpeaking];
	} else {
		ERR_PRINTS("TTS driver is not initialized!");
		return false;
	}
}

void TTSDriver::set_voice(String p_voice) {
	if (driver_data) {
		driver_data->voice_ref = [AVSpeechSynthesisVoice voiceWithIdentifier:[NSString stringWithUTF8String:p_voice.utf8().get_data()]];
		if (driver_data->voice_ref) {
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
		driver_data->volume = p_volume / 100.0;
	} else {
		ERR_PRINTS("TTS driver is not initialized!");
	}
};

int TTSDriver::get_volume() const {
	if (driver_data) {
		return driver_data->volume * 100.0;
	} else {
		ERR_PRINTS("TTS driver is not initialized!");
		return 0;
	}
}

void TTSDriver::set_rate(int p_rate) {
	if (driver_data) {
		driver_data->rate = p_rate;
	} else {
		ERR_PRINTS("TTS driver is not initialized!");
	}
}

int TTSDriver::get_rate() const {
	if (driver_data) {
		return driver_data->rate;
	} else {
		ERR_PRINTS("TTS driver is not initialized!");
		return 0;
	}
}

void TTSDriver::_init() {
	driver_data = new TTSDriverData();
	if (driver_data) {
		driver_data->synth = [[AVSpeechSynthesizer alloc] init];
		if (driver_data->synth) {
			driver_data->delegate = [[GodotSpeechSynthesizerDelegate alloc] init];
			if (driver_data->delegate) {
				[driver_data->synth setDelegate:driver_data->delegate];
			} else {
				ERR_PRINTS("Cannot initialize AVSpeechSynthesizerDelegate");
				[driver_data->synth release];
				delete driver_data;
				driver_data = NULL;
			}
		} else {
			ERR_PRINTS("Cannot initialize AVSpeechSynthesizer!");
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
