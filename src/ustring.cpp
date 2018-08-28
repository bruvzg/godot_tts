/*************************************************************************/
/*  ustring.cpp                                                          */
/*************************************************************************/

#include <ustring.h>

const char *CharString::get_data() const {
	return api->godot_char_string_get_data(&gdn);
};

CharString::~CharString() {
	api->godot_char_string_destroy(&gdn);
};

String::String() {
	api->godot_string_new(&gdn);
};

String::String(const char *p_string) {
	api->godot_string_new(&gdn);
	api->godot_string_parse_utf8(&gdn, p_string);
};

String::String(const wchar_t *p_string) {
	api->godot_string_new_with_wide_string(&gdn, p_string, wcslen(p_string));
};

String::String(const String &p_string) {
	api->godot_string_new_copy(&gdn, &p_string.gdn);
};

const wchar_t *String::c_str() const {
	return api->godot_string_wide_str(&gdn);
};

CharString String::utf8() const {
	godot_char_string ret = api->godot_string_utf8(&gdn);
	return *reinterpret_cast<CharString *>(&ret);
};

String &String::operator=(const String &p_string) {
	api->godot_string_destroy(&gdn);
	api->godot_string_new_copy(&gdn, &p_string.gdn);
	return *this;
};

String String::operator+(const String &p_string) const {
	String ret = *this;
	ret.gdn = api->godot_string_operator_plus(&ret.gdn, &p_string.gdn);
	return ret;
};

bool String::operator==(const String &p_string) const {
	return api->godot_string_operator_equal(&gdn, &p_string.gdn);
};

String::~String() {
	api->godot_string_destroy(&gdn);
};

String operator+(const char *p_string_a, const String &p_string_b) {
	return String(p_string_a) + p_string_b;
};

String operator+(const wchar_t *p_string_a, const String &p_string_b) {
	return String(p_string_a) + p_string_b;
};
