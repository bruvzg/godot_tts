/*************************************************************************/
/*  dictionary.h                                                         */
/*************************************************************************/

#pragma once

#include <gdnative.h>
#include <gdnative/dictionary.h>

class Variant;

class Dictionary {
	godot_dictionary gdn;

public:
	Dictionary();
	Dictionary(const Dictionary & p_dict);

	Dictionary &operator=(const Dictionary &p_dict);

	void set(const Variant& p_key, const Variant& p_value);

	~Dictionary();
};
