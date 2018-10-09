/*************************************************************************/
/*  tts_driver.cpp                                                       */
/*************************************************************************/

#include "tts_driver.h"

TTSDriver *TTSDriver::singleton = NULL;

//GDNative wrappers
void GDCALLINGCONV *gdn_tts_init(godot_object *p_instance, void *p_method_data) {
	//TTSDriver is singleton, nothing to do
	return NULL;
};

void GDCALLINGCONV gdn_tts_terminate(godot_object *p_instance, void *p_method_data, void *p_user_data) {
	//TTSDriver is singleton, nothing to do
};

//void speak(const String &p_text, bool p_interrupt = false);
Variant GDCALLINGCONV gdn_tts_speak(godot_object *p_instance, void *p_method_data, void *p_user_data, int p_num_args, Variant **p_args) {
	if ((p_num_args >= 2) && TTSDriver::get_singleton()) {
		TTSDriver::get_singleton()->speak(*p_args[0], *p_args[1]);
	}
	return Variant();
};

//void stop();
Variant GDCALLINGCONV gdn_tts_stop(godot_object *p_instance, void *p_method_data, void *p_user_data, int p_num_args, Variant **p_args) {
	if (TTSDriver::get_singleton()) {
		TTSDriver::get_singleton()->stop();
	}
	return Variant();
};

//bool is_speaking();
Variant GDCALLINGCONV gdn_tts_is_speaking(godot_object *p_instance, void *p_method_data, void *p_user_data, int p_num_args, Variant **p_args) {
	if (TTSDriver::get_singleton()) {
		return TTSDriver::get_singleton()->is_speaking();
	}
	return false;
};

//Array get_voices();
Variant GDCALLINGCONV gdn_tts_get_voices(godot_object *p_instance, void *p_method_data, void *p_user_data, int p_num_args, Variant **p_args) {
	if (TTSDriver::get_singleton()) {
		return TTSDriver::get_singleton()->get_voices();
	}
	return Array();
};

//void set_voice(const String &p_voice);
Variant GDCALLINGCONV gdn_tts_set_voice(godot_object *p_instance, void *p_method_data, void *p_user_data, int p_num_args, Variant **p_args) {
	if ((p_num_args >= 1) && TTSDriver::get_singleton()) {
		TTSDriver::get_singleton()->set_voice(*p_args[0]);
	}
	return Variant();
};

//void set_volume(int p_volume);
Variant GDCALLINGCONV gdn_tts_set_volume(godot_object *p_instance, void *p_method_data, void *p_user_data, int p_num_args, Variant **p_args) {
	if ((p_num_args >= 1) && TTSDriver::get_singleton()) {
		TTSDriver::get_singleton()->set_volume(*p_args[0]);
	}
	return Variant();
};

//int get_volume();
Variant GDCALLINGCONV gdn_tts_get_volume(godot_object *p_instance, void *p_method_data, void *p_user_data, int p_num_args, Variant **p_args) {
	if (TTSDriver::get_singleton()) {
		return TTSDriver::get_singleton()->get_volume();
	}
	return 0;
};

//void set_rate(int p_rate);
Variant GDCALLINGCONV gdn_tts_set_rate(godot_object *p_instance, void *p_method_data, void *p_user_data, int p_num_args, Variant **p_args) {
	if ((p_num_args >= 1) && TTSDriver::get_singleton()) {
		TTSDriver::get_singleton()->set_rate(*p_args[0]);
	}
	return Variant();
};

//int get_rate();
Variant GDCALLINGCONV gdn_tts_get_rate(godot_object *p_instance, void *p_method_data, void *p_user_data, int p_num_args, Variant **p_args) {
	if (TTSDriver::get_singleton()) {
		return TTSDriver::get_singleton()->get_rate();
	}
	return 0;
};

//GDNative registration
void register_nativescript_class(void *p_handle, const char *p_class_name, const char *p_base_class_name, gdn_create_func p_create_func, gdn_destroy_func p_destroy_func, const char *p_docs = NULL) {

	godot_instance_create_func create_func = {
		p_create_func,
		NULL,
		NULL
	};
	godot_instance_destroy_func destroy_func = {
		p_destroy_func,
		NULL,
		NULL
	};
	nativescript_api->godot_nativescript_register_class(p_handle, p_class_name, p_base_class_name, create_func, destroy_func);
	nativescript_api->godot_nativescript_register_tool_class(p_handle, p_class_name, p_base_class_name, create_func, destroy_func);
	if (nativescript_1_1_api && p_docs) {
		godot_string docs = api->godot_string_chars_to_utf8(p_docs);
		nativescript_1_1_api->godot_nativescript_set_class_documentation(p_handle, p_class_name, docs);
		api->godot_string_destroy(&docs);
	}
};

void register_nativescript_method(void *p_handle, const char *p_class_name, const char *p_method_name, gdn_method p_method, unsigned int p_num_args, godot_method_arg *p_args, const char *p_docs = NULL) {

	godot_instance_method method = {
		p_method,
		NULL,
		NULL
	};
	godot_method_attributes attr = {
		GODOT_METHOD_RPC_MODE_DISABLED
	};
	nativescript_api->godot_nativescript_register_method(p_handle, p_class_name, p_method_name, attr, method);
	if (nativescript_1_1_api && (p_num_args > 0)) {
		nativescript_1_1_api->godot_nativescript_set_method_argument_information(p_handle, p_class_name, p_method_name, p_num_args, p_args);
	}
	if (nativescript_1_1_api && p_docs) {
		godot_string docs = api->godot_string_chars_to_utf8(p_docs);
		nativescript_1_1_api->godot_nativescript_set_method_documentation(p_handle, p_class_name, p_method_name, docs);
		api->godot_string_destroy(&docs);
	}
	for (unsigned int i = 0; i < p_num_args; i++) {
		api->godot_string_destroy(&p_args[i].name);
		api->godot_string_destroy(&p_args[i].hint_string);
	}
};

//Base class

TTSDriver *TTSDriver::get_singleton() {
	return singleton;
};

void TTSDriver::set_singleton() {
	singleton = this;
};

void TTSDriver::register_methods(void *p_handle) {
	register_nativescript_class(p_handle, "TTSDriver", "Reference", reinterpret_cast<gdn_create_func>(&gdn_tts_init), reinterpret_cast<gdn_destroy_func>(&gdn_tts_terminate), "Native text-to-speech interface");
	godot_method_arg gdn_tts_speak_args[] = {
		{
			api->godot_string_chars_to_utf8("text"),
			GODOT_VARIANT_TYPE_STRING,
			GODOT_PROPERTY_HINT_NONE,
			api->godot_string_chars_to_utf8("")
		},
		{
			api->godot_string_chars_to_utf8("interrupt"),
			GODOT_VARIANT_TYPE_BOOL,
			GODOT_PROPERTY_HINT_NONE,
			api->godot_string_chars_to_utf8("")
		}
	};
	register_nativescript_method(p_handle, "TTSDriver", "speak", reinterpret_cast<gdn_method>(&gdn_tts_speak), 2, gdn_tts_speak_args, "Begins speaking synthesized text.");
	register_nativescript_method(p_handle, "TTSDriver", "stop", reinterpret_cast<gdn_method>(&gdn_tts_stop), 0, NULL, "Stops synthesis in progress.");
	register_nativescript_method(p_handle, "TTSDriver", "is_speaking", reinterpret_cast<gdn_method>(&gdn_tts_is_speaking), 0, NULL, "Returns [code]true[/code] if the synthesizer is generating speech, [code]false[/code] otherwise.");
	register_nativescript_method(p_handle, "TTSDriver", "get_voices", reinterpret_cast<gdn_method>(&gdn_tts_get_voices), 0, NULL, "Returns an Array of voice information Dictionaries.\n\"name\" - String, voice identifier\n\"language\" - String, language code");
	godot_method_arg gdn_tts_set_voice_args[] = {
		{
			api->godot_string_chars_to_utf8("voice"),
			GODOT_VARIANT_TYPE_STRING,
			GODOT_PROPERTY_HINT_NONE,
			api->godot_string_chars_to_utf8("")
		}
	};
	register_nativescript_method(p_handle, "TTSDriver", "set_voice", reinterpret_cast<gdn_method>(&gdn_tts_set_voice), 1, gdn_tts_set_voice_args, "Sets the speech synthesizer’s current voice.");
	godot_method_arg gdn_tts_set_volume_args[] = {
		{
			api->godot_string_chars_to_utf8("volume"),
			GODOT_VARIANT_TYPE_INT,
			GODOT_PROPERTY_HINT_NONE,
			api->godot_string_chars_to_utf8("")
		}
	};
	register_nativescript_method(p_handle, "TTSDriver", "set_volume", reinterpret_cast<gdn_method>(&gdn_tts_set_volume), 1, gdn_tts_set_volume_args, "Sets the speech synthesizer’s volume");
	register_nativescript_method(p_handle, "TTSDriver", "get_volume", reinterpret_cast<gdn_method>(&gdn_tts_get_volume), 0, NULL, "Returns the speech synthesizer’s volume");
	godot_method_arg gdn_tts_set_rate_args[] = {
		{
			api->godot_string_chars_to_utf8("rate"),
			GODOT_VARIANT_TYPE_INT,
			GODOT_PROPERTY_HINT_NONE,
			api->godot_string_chars_to_utf8("")
		}
	};
	register_nativescript_method(p_handle, "TTSDriver", "set_rate", reinterpret_cast<gdn_method>(&gdn_tts_set_rate), 1, gdn_tts_set_rate_args, "Sets the speech synthesizer’s rendering rate adjustment.");
	register_nativescript_method(p_handle, "TTSDriver", "get_rate", reinterpret_cast<gdn_method>(&gdn_tts_get_rate), 0, NULL, "Returns the speech synthesizer’s rendering rate adjustment.");
};

TTSDriver::TTSDriver() {
	set_singleton();
};

TTSDriver::~TTSDriver() {};
