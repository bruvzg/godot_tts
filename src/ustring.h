/*************************************************************************/
/*  ustring.h                                                            */
/*************************************************************************/

#pragma once

#include <gdnative.h>
#include <gdnative/string.h>
#include <gdnative/pool_arrays.h>

class String;

class CharString {
	godot_char_string gdn;

public:
	const char *get_data() const;

	~CharString();
};

class String {
	godot_string gdn;

public:
	String();
	String(const String &p_string);
	String(const char *p_string);
	String(const wchar_t *p_string);

	const wchar_t *c_str() const;
	CharString utf8() const;

	String &operator=(const String &p_string);

	String operator+(const String &p_string) const;
	bool operator==(const String &p_string) const;

	static String itos(int64_t p_num, godot_int p_base);

	~String();
};

String operator+(const char *p_string_a, const String &p_string_b);

String operator+(const wchar_t *p_string_a, const String &p_string_b);
