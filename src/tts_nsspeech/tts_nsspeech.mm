/*************************************************************************/
/*  tts_nsspeech.mm                                                      */
/*************************************************************************/

#include "tts_nsspeech.h"

@implementation GodotSpeechSynthesizerDelegate
- (void)discardMessages {
	messages.clear();
};

- (void)appendMessage:(const String &)message {
	messages.push_back(message);
};

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
};
@end

TTSDriverNSSpeech::TTSDriverNSSpeech() {
	synth = NULL;
	delegate = NULL;
};

bool TTSDriverNSSpeech::initialize() {
	synth = [[NSSpeechSynthesizer alloc] init];
	if (synth) {
		delegate = [[GodotSpeechSynthesizerDelegate alloc] init];
		if (delegate) {
			[synth setDelegate:delegate];
			return true;
		} else {
			DEBUG_PRINT_ERROR("Cannot initialize NSSpeechSynthesizerDelegate");
			return false;
		}
	} else {
		DEBUG_PRINT_ERROR("Cannot initialize NSSpeechSynthesizer!");
		return false;
	}
};

void TTSDriverNSSpeech::speak(const String &p_text, bool p_interrupt) {
	if (synth && delegate) {
		if (p_interrupt) {
			[delegate discardMessages];
			[synth stopSpeaking];
			[delegate appendMessage:p_text];
			[synth startSpeakingString:[NSString stringWithUTF8String:p_text.utf8().get_data()]];
		} else {
			[delegate appendMessage:p_text];
			if (![synth isSpeaking]) {
				[synth startSpeakingString:[NSString stringWithUTF8String:p_text.utf8().get_data()]];
			}
		}
	} else {
		DEBUG_PRINT_WARNING("TTS driver is not initialized!");
	}
};

void TTSDriverNSSpeech::stop() {
	if (synth && delegate) {
		[delegate discardMessages];
		[synth stopSpeaking];
	} else {
		DEBUG_PRINT_WARNING("TTS driver is not initialized!");
	}
};

Array TTSDriverNSSpeech::get_voices() {
	Array list;
	for (NSString *voiceIdentifierString in [NSSpeechSynthesizer availableVoices]) {
		NSString *voiceLocaleIdentifier = [[NSSpeechSynthesizer attributesForVoice:voiceIdentifierString] objectForKey:NSVoiceLocaleIdentifier];
		Dictionary voice_d;
		voice_d.set(String("name"), String([voiceIdentifierString UTF8String]));
		voice_d.set(String("language"), String([voiceLocaleIdentifier UTF8String]));
		list.push_back(voice_d);
	}
	return list;
};

void TTSDriverNSSpeech::set_voice(const String &p_voice) {
	if (synth) {
		[synth setVoice:[NSString stringWithUTF8String:p_voice.utf8().get_data()]];
	} else {
		DEBUG_PRINT_WARNING("TTS driver is not initialized!");
	}
};

bool TTSDriverNSSpeech::is_speaking() {
	if (synth && delegate) {
		return [synth isSpeaking] || [delegate isSpeaking];
	} else {
		DEBUG_PRINT_WARNING("TTS driver is not initialized!");
		return false;
	}
};

void TTSDriverNSSpeech::set_volume(int p_volume) {
	if (synth) {
		[synth setVolume:p_volume / 100.0];
	} else {
		DEBUG_PRINT_WARNING("TTS driver is not initialized!");
	}
};

int TTSDriverNSSpeech::get_volume() {
	if (synth) {
		return [synth volume] * 100.0;
	} else {
		DEBUG_PRINT_WARNING("TTS driver is not initialized!");
		return 0;
	}
};

void TTSDriverNSSpeech::set_rate(int p_rate) {
	if (synth) {
		[synth setRate:200 + (p_rate / 2)];
	} else {
		DEBUG_PRINT_WARNING("TTS driver is not initialized!");
	}
};

int TTSDriverNSSpeech::get_rate() {
	if (synth) {
		return ([synth rate] - 200) * 2;
	} else {
		DEBUG_PRINT_WARNING("TTS driver is not initialized!");
		return 0;
	}
};

TTSDriverNSSpeech::~TTSDriverNSSpeech() {
	if (synth) {
		[synth release];
		synth = NULL;
	}
	if (delegate) {
		[delegate release];
		delegate = NULL;
	}
};
