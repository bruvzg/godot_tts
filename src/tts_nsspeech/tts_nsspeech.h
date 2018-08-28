/*************************************************************************/
/*  tts_nsspeech.h                                                       */
/*************************************************************************/

#pragma once

#include "tts_driver.h"

#include <AppKit/NSSpeechSynthesizer.h>
#include <Foundation/NSString.h>
#include <Foundation/NSThread.h>
#include <list>

@interface GodotSpeechSynthesizerDelegate : NSObject <NSSpeechSynthesizerDelegate> {
	std::list<String> messages;
};
- (void)discardMessages;
- (void)appendMessage:(const String &)message;
- (bool)isSpeaking;
@end

class TTSDriverNSSpeech : public TTSDriver {
	GodotSpeechSynthesizerDelegate *delegate;
	NSSpeechSynthesizer *synth;

public:
	TTSDriverNSSpeech();

	bool initialize();

	void speak(const String &p_text, bool p_interrupt);
	void stop();
	bool is_speaking();

	Array get_voices();
	void set_voice(const String &p_voice);

	void set_volume(int p_volume);
	int get_volume();
	void set_rate(int p_rate);
	int get_rate();

	~TTSDriverNSSpeech();
};

static TTSDriverNSSpeech singleton;
