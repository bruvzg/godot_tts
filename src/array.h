/*************************************************************************/
/*  array.h                                                              */
/*************************************************************************/

#pragma once

#include <gdnative.h>
#include <gdnative/array.h>

class Variant;

class Array {
	godot_array gdn;

public:
	Array();
	Array(const Array &p_array);

	Array &operator=(const Array &p_array);

	bool empty() const;
	void erase(const Variant& p_var);
	void push_back(const Variant& p_var);

	~Array();
};
