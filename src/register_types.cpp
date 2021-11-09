/*************************************************************************/
/*  register_types.cpp                                                   */
/*************************************************************************/

#include <godot_cpp/godot.hpp>

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/core/memory.hpp>

#include <godot_cpp/classes/engine.hpp>

#include "tts_common.h"

using namespace godot;

extern "C" {

TTSDriver *tts_singleton = nullptr;

void register_types() {
	printf("[GDE] TTS register...\n");

	ClassDB::register_class<TTSDriver>();

	tts_singleton = new TTSDriver;
	Engine::get_singleton()->register_singleton("TTS", TTSDriver::get_singleton());
}

void unregister_types() {
	printf("[GDE] TTS unregister...\n");

	Engine::get_singleton()->unregister_singleton("TTS");
	if (tts_singleton) {
		delete tts_singleton;
		tts_singleton = nullptr;
	}
}

GDNativeBool GDN_EXPORT tts2_init(const GDNativeInterface *p_interface, const GDNativeExtensionClassLibraryPtr p_library, GDNativeInitialization *r_initialization) {
	printf("[GDE] Init TTS2 library...\n");
	GDExtensionBinding::InitObject init_obj(p_interface, p_library, r_initialization);

	init_obj.register_scene_initializer(register_types);
	init_obj.register_scene_terminator(unregister_types);

	return init_obj.init();
}

} /* extern "C" */
