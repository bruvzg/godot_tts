/*************************************************************************/
/*  gdnative.h                                                      */
/*************************************************************************/

#pragma once

#include <gdnative_api_struct.gen.h>

extern "C" const godot_gdnative_core_api_struct *api;
extern "C" const godot_gdnative_ext_nativescript_api_struct *nativescript_api;
extern "C" const godot_gdnative_ext_nativescript_1_1_api_struct *nativescript_1_1_api;

extern "C" void GDN_EXPORT godot_gdnative_init(godot_gdnative_init_options *p_options);
extern "C" void GDN_EXPORT godot_gdnative_terminate(godot_gdnative_terminate_options *p_options);
extern "C" void GDN_EXPORT godot_nativescript_init(void *p_handle);

typedef GDCALLINGCONV godot_variant (*gdn_method)(godot_object *, void *, void *, int, godot_variant **);
typedef GDCALLINGCONV void *(*gdn_create_func)(godot_object *, void *);
typedef GDCALLINGCONV void (*gdn_destroy_func)(godot_object *, void *, void *);

#define DEBUG_PRINT_WARNING(m_text) api->godot_print_warning(m_text, __FUNCTION__, __FILE__, __LINE__);
#define DEBUG_PRINT_ERROR(m_text) api->godot_print_error(m_text, __FUNCTION__, __FILE__, __LINE__);
