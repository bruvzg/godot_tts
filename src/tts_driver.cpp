/*************************************************************************/
/*  tts_driver.hpp                                                       */
/*************************************************************************/

#include "tts_driver.hpp"

void TTSDriver::_register_methods() {
	register_method("speak", &TTSDriver::speak);
	register_method("stop", &TTSDriver::stop);

	register_method("is_speaking", &TTSDriver::is_speaking);

	register_method("get_voices", &TTSDriver::get_voices);

	register_method("set_voice", &TTSDriver::set_voice);
	register_method("get_voice", &TTSDriver::get_voice);
	register_property<TTSDriver, String>("voice", &TTSDriver::set_voice, &TTSDriver::get_voice, "");

	register_method("set_volume", &TTSDriver::set_volume);
	register_method("get_volume", &TTSDriver::get_volume);
	register_property<TTSDriver, int>("volume", &TTSDriver::set_volume, &TTSDriver::get_volume, 100);

	register_method("set_rate", &TTSDriver::set_rate);
	register_method("get_rate", &TTSDriver::get_rate);
	register_property<TTSDriver, int>("rate", &TTSDriver::set_rate, &TTSDriver::get_rate, 100);
}

TTSDriver::TTSDriver() {
	driver_data = NULL;
}
