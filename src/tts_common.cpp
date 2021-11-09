/*************************************************************************/
/*  tts_common.cpp                                                       */
/*************************************************************************/

#include "tts_common.h"

namespace godot {

TTSDriver *TTSDriver::singleton = nullptr;

void TTSDriver::_bind_methods() {
	ClassDB::bind_method(D_METHOD("speak", "text", "interrupt"), &TTSDriver::speak);
	ClassDB::bind_method(D_METHOD("stop"), &TTSDriver::stop);

	ClassDB::bind_method(D_METHOD("is_speaking"), &TTSDriver::is_speaking);

	ClassDB::bind_method(D_METHOD("get_voices"), &TTSDriver::get_voices);

	ClassDB::bind_method(D_METHOD("set_voice", "voice"), &TTSDriver::set_voice);
	ClassDB::bind_method(D_METHOD("get_voice"), &TTSDriver::get_voice);
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "voice"), "set_voice", "get_voice");

	ClassDB::bind_method(D_METHOD("set_volume", "volume"), &TTSDriver::set_volume);
	ClassDB::bind_method(D_METHOD("get_volume"), &TTSDriver::get_volume);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "volume", PROPERTY_HINT_RANGE, "0,100,1"), "set_volume", "get_volume");

	ClassDB::bind_method(D_METHOD("set_pitch", "pitch"), &TTSDriver::set_pitch);
	ClassDB::bind_method(D_METHOD("get_pitch"), &TTSDriver::get_pitch);
	ADD_PROPERTY(PropertyInfo(Variant::FLOAT, "pitch", PROPERTY_HINT_RANGE, "-10,10,0.001"), "set_pitch", "get_pitch");

	ClassDB::bind_method(D_METHOD("set_rate", "rate"), &TTSDriver::set_rate);
	ClassDB::bind_method(D_METHOD("get_rate"), &TTSDriver::get_rate);
	ADD_PROPERTY(PropertyInfo(Variant::INT, "rate", PROPERTY_HINT_RANGE, "-100,100,1"), "set_rate", "get_rate");
}

} /* namespace godot */
