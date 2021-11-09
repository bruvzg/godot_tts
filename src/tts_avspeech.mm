/*************************************************************************/
/*  tts_avspeech.mm                                                      */
/*************************************************************************/

#include "tts_common.h"

#include <list>
#include <AVFAudio/AVSpeechSynthesis.h>
#include <Foundation/NSString.h>
#include <Foundation/NSThread.h>

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

namespace godot {

/*************************************************************************/

struct TTSDriverPrivate {
	GodotSpeechSynthesizerDelegate *delegate = nullptr;
	AVSpeechSynthesizer *synth = nullptr;
	AVSpeechSynthesisVoice *voice_ref = nullptr;

	float volume = 50.f;
	float rate = 0.f;
	float pitch = 0.f;
};

/*************************************************************************/

void TTSDriver::speak(const String &p_text, bool p_interrupt) {
	if (data && data->voice_ref) {
		if (p_interrupt) {
			[data->synth stopSpeakingAtBoundary:AVSpeechBoundaryImmediate];

			AVSpeechUtterance *new_utterance = [[AVSpeechUtterance alloc] initWithString:[NSString stringWithUTF8String:p_text.utf8().get_data()]];
			float rate = (data->rate + 100.f) / 200.f * (AVSpeechUtteranceMaximumSpeechRate - AVSpeechUtteranceMinimumSpeechRate) + AVSpeechUtteranceMinimumSpeechRate;
			float pitch = (data->pitch + 10.f) / 20.f * 1.0f;
			[new_utterance setVoice:data->voice_ref];
			[new_utterance setRate:rate];
			[new_utterance setPitchMultiplier:pitch];
			[new_utterance setVolume:data->volume];

			[data->synth speakUtterance:new_utterance];
		} else {
			AVSpeechUtterance *new_utterance = [[AVSpeechUtterance alloc] initWithString:[NSString stringWithUTF8String:p_text.utf8().get_data()]];
			float rate = (data->rate + 100.f) / 200.f * (AVSpeechUtteranceMaximumSpeechRate - AVSpeechUtteranceMinimumSpeechRate) + AVSpeechUtteranceMinimumSpeechRate;
			float pitch = (data->pitch + 10.f) / 20.f * 1.0f;
			[new_utterance setVoice:data->voice_ref];
			[new_utterance setRate:rate];
			[new_utterance setPitchMultiplier:pitch];
			[new_utterance setVolume:data->volume];

			if ([data->synth isSpeaking]) {
				[data->delegate appendMessage:new_utterance];
			} else {
				[data->synth speakUtterance:new_utterance];
			}
		}
	} else {
		WARN_PRINT("TTSDriver is not initialized!");
	}
}

void TTSDriver::stop() {
	if (data) {
		[data->synth stopSpeakingAtBoundary:AVSpeechBoundaryImmediate];
	} else {
		WARN_PRINT("TTSDriver is not initialized!");
	}
}

Array TTSDriver::get_voices() const {
	Array list;
	if (data) {
		for (AVSpeechSynthesisVoice *voice in [AVSpeechSynthesisVoice speechVoices]) {
			NSString *voiceIdentifierString = [voice identifier];
			NSString *voiceLocaleIdentifier = [voice language];
			Dictionary voice_d;
			voice_d["name"] = String::utf8([voiceIdentifierString UTF8String]);
			voice_d["language"] = String::utf8([voiceLocaleIdentifier UTF8String]);
			list.push_back(voice_d);
		}
	} else {
		WARN_PRINT("TTSDriver is not initialized!");
	}
	return list;
}

bool TTSDriver::is_speaking() const {
	if (data) {
		return [data->synth isSpeaking] || [data->delegate isSpeaking];
	} else {
		WARN_PRINT("TTSDriver is not initialized!");
		return false;
	}
}

void TTSDriver::set_voice(const String &p_voice) {
	if (data) {
		data->voice_ref = [AVSpeechSynthesisVoice voiceWithIdentifier:[NSString stringWithUTF8String:p_voice.utf8().get_data()]];
	} else {
		WARN_PRINT("TTSDriver is not initialized!");
	}
}

String TTSDriver::get_voice() const {
	if (data) {
		if (data->voice_ref) {
			return String::utf8([[data->voice_ref name] UTF8String]);
		} else {
			return String();
		}
	} else {
		WARN_PRINT("TTSDriver is not initialized!");
		return String();
	}
}

void TTSDriver::set_volume(int p_volume) {
	if (data) {
		data->volume = p_volume / 100.0;
	} else {
		WARN_PRINT("TTSDriver is not initialized!");
	}
}

int TTSDriver::get_volume() const {
	if (data) {
		return data->volume * 100.0;
	} else {
		WARN_PRINT("TTSDriver is not initialized!");
		return 0;
	}
}

void TTSDriver::set_pitch(float p_pitch) {
	if (data) {
		data->pitch = p_pitch;
	} else {
		WARN_PRINT("TTSDriver is not initialized!");
	}
}

float TTSDriver::get_pitch() const {
	if (data) {
		return data->pitch;
	} else {
		WARN_PRINT("TTSDriver is not initialized!");
		return 0;
	}
}

void TTSDriver::set_rate(int p_rate) {
	if (data) {
		data->rate = p_rate;
	} else {
		WARN_PRINT("TTSDriver is not initialized!");
	}
}

int TTSDriver::get_rate() const {
	if (data) {
		return data->rate;
	} else {
		WARN_PRINT("TTSDriver is not initialized!");
		return 0;
	}
}

TTSDriver::TTSDriver() {
	data = new TTSDriverPrivate;
	data->synth = [[AVSpeechSynthesizer alloc] init];
	if (data->synth) {
		data->delegate = [[GodotSpeechSynthesizerDelegate alloc] init];
		if (data->delegate) {
			[data->synth setDelegate:data->delegate];
			data->voice_ref = [[AVSpeechSynthesisVoice speechVoices] firstObject];
			data->rate = (AVSpeechUtteranceDefaultSpeechRate - AVSpeechUtteranceMinimumSpeechRate) / (AVSpeechUtteranceMaximumSpeechRate - AVSpeechUtteranceMinimumSpeechRate) * 200.f - 100.f;
			print_verbose("Text-to-Speech: AVSpeechSynthesizer initialized.");
		} else {
			[data->synth release];
			delete data;
			data = nullptr;
			ERR_FAIL_MSG("Cannot initialize AVSpeechSynthesizerDelegate!");
		}
	} else {
		delete data;
		data = nullptr;
		ERR_FAIL_MSG("Cannot initialize AVSpeechSynthesizer!");
	}
	singleton = this;
}

TTSDriver::~TTSDriver() {
	if (data) {
		[data->synth release];
		[data->delegate release];

		delete data;
	}
	singleton = nullptr;
}

} /* namespace godot */
