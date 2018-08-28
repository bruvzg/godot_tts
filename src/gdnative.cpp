/*************************************************************************/
/*  gdnative_core.cpp                                                    */
/*************************************************************************/

#include "gdnative.h"
#include "array.h"
#include "dictionary.h"
#include "variant.h"
#include "string.h"
#include "tts_driver.h"

const godot_gdnative_core_api_struct *api = nullptr;
const godot_gdnative_ext_nativescript_api_struct *nativescript_api = nullptr;
const godot_gdnative_ext_nativescript_1_1_api_struct *nativescript_1_1_api = NULL;

extern "C" void GDN_EXPORT godot_gdnative_init(godot_gdnative_init_options *p_options) {
	api = p_options->api_struct;
	for (unsigned int i = 0; i < api->num_extensions; i++) {
		if (api->extensions[i]->type == GDNATIVE_EXT_NATIVESCRIPT) {
			nativescript_api = reinterpret_cast<const godot_gdnative_ext_nativescript_api_struct *>(api->extensions[i]);
			const godot_gdnative_api_struct *next = nativescript_api->next;
			while (next) {
				if ((next->version.major == 1) && (next->version.minor == 1)) {
					nativescript_1_1_api = reinterpret_cast<const godot_gdnative_ext_nativescript_1_1_api_struct *>(next);
				}
				next = next->next;
			}
		}
	}
};

extern "C" void GDN_EXPORT godot_gdnative_terminate(godot_gdnative_terminate_options *p_options) {
	api = NULL;
	nativescript_api = NULL;
	nativescript_1_1_api = NULL;
};

extern "C" void GDN_EXPORT godot_nativescript_init(void *p_handle) {
	TTSDriver::register_methods(p_handle);
	if (TTSDriver::get_singleton()) {
		if (!TTSDriver::get_singleton()->initialize()) {
			DEBUG_PRINT_ERROR("Cannot initialize TTSDriver!");
		}
	} else {
		DEBUG_PRINT_ERROR("No TTSDriver singleton!");
	}
};
