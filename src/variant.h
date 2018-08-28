/*************************************************************************/
/*  variant.h                                                            */
/*************************************************************************/

#pragma once

#include <gdnative.h>
#include <gdnative/variant.h>

class Array;
class Dictionary;
class String;

class Variant {
	godot_variant gdn;

public:
	Variant();
	Variant(const Variant& p_var);
	Variant(bool p_bool);
	Variant(int p_int);
	Variant(const String& p_string);
	Variant(const Dictionary& p_dict);
	Variant(const Array& p_array);

	Variant &operator=(const Variant& p_var);

	operator bool() const;
	operator signed int() const;
	operator String() const;
	operator Dictionary() const;
	operator Array() const;

	~Variant();
};
