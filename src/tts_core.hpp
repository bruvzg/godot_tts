/*************************************************************************/
/*  tts_core.hpp                                                         */
/*************************************************************************/

#pragma once

#include <cmath>
#include <cstdlib>
#include <cstring>

#include <nativescript/godot_nativescript.h>
#include <Godot.hpp>

#ifdef WARN_PRINTS
#undef WARN_PRINTS
#endif

#define WARN_PRINTS(m_text) godot::api->godot_print_warning(String(m_text).utf8().get_data(), __FUNCTION__, __FILE__, __LINE__);

#ifdef ERR_PRINTS
#undef ERR_PRINTS
#endif

#define ERR_PRINTS(m_text) godot::api->godot_print_error(String(m_text).utf8().get_data(), __FUNCTION__, __FILE__, __LINE__);
