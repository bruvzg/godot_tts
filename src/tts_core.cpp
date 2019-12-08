/*************************************************************************/
/*  tts_core.cpp                                                         */
/*************************************************************************/

#include "tts_core.hpp"

#include "tts_driver.hpp"

using namespace godot;

extern "C" void GDN_EXPORT gdtts_gdnative_init(godot_gdnative_init_options *p_options) {

	//Init GDNative
	Godot::gdnative_init(p_options);
}

extern "C" void GDN_EXPORT gdtts_gdnative_terminate(godot_gdnative_terminate_options *p_options) {

	//Finish GDNative
	Godot::gdnative_terminate(p_options);
}

extern "C" void GDN_EXPORT gdtts_nativescript_init(void *p_handle) {

	Godot::nativescript_init(p_handle);

	register_class<TTSDriver>();
	register_tool_class<TTSDriver>();
}
