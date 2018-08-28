/*************************************************************************/
/*  array.cpp                                                            */
/*************************************************************************/

#include <array.h>
#include <variant.h>

Array::Array() {
	api->godot_array_new(&gdn);
};

Array::Array(const Array &p_array) {
	api->godot_array_new_copy(&gdn, &p_array.gdn);
};

Array &Array::operator=(const Array &p_array) {
	api->godot_array_destroy(&gdn);
	api->godot_array_new_copy(&gdn, &p_array.gdn);
	return *this;
};

bool Array::empty() const {
	return api->godot_array_empty(&gdn);
};

void Array::erase(const Variant& p_var) {
	api->godot_array_erase(&gdn, reinterpret_cast<const godot_variant *>(&p_var));
};

void Array::push_back(const Variant& p_var) {
	api->godot_array_push_back(&gdn, reinterpret_cast<const godot_variant *>(&p_var));
};

Array::~Array() {
	api->godot_array_destroy(&gdn);
};
