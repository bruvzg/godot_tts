/*************************************************************************/
/*  variant.cpp                                                          */
/*************************************************************************/

#include <variant.h>
#include <ustring.h>
#include <array.h>
#include <dictionary.h>

Variant::Variant() {
	api->godot_variant_new_nil(&gdn);
};

Variant::Variant(const Variant& p_var) {
	api->godot_variant_new_copy(&gdn, &p_var.gdn);
};

Variant::Variant(bool p_bool) {
	api->godot_variant_new_bool(&gdn, p_bool);
};

Variant::Variant(int p_int) {
	api->godot_variant_new_int(&gdn, p_int);
};

Variant::Variant(float p_float) {
	api->godot_variant_new_real(&gdn, p_float);
};

Variant::Variant(const String& p_string) {
	api->godot_variant_new_string(&gdn, reinterpret_cast<const godot_string *>(&p_string));
};

Variant::Variant(const Dictionary& p_dict) {
	api->godot_variant_new_dictionary(&gdn, reinterpret_cast<const godot_dictionary *>(&p_dict));
};

Variant::Variant(const Array& p_array) {
	api->godot_variant_new_array(&gdn, reinterpret_cast<const godot_array *>(&p_array));
};

Variant &Variant::operator=(const Variant& p_var) {
	api->godot_variant_destroy(&gdn);
	api->godot_variant_new_copy(&gdn, &p_var.gdn);
	return *this;
};

Variant::operator bool() const {
	return api->godot_variant_booleanize(&gdn);
};

Variant::operator float() const {
	return api->godot_variant_as_real(&gdn);
};

Variant::operator int() const {
	return api->godot_variant_as_int(&gdn);
};

Variant::operator String() const {
	godot_string ret = api->godot_variant_as_string(&gdn);
	return *reinterpret_cast<String *>(&ret);
};

Variant::operator Dictionary() const {
	godot_dictionary ret = api->godot_variant_as_dictionary(&gdn);
	return *reinterpret_cast<Dictionary *>(&ret);
};

Variant::operator Array() const {
	godot_array ret = api->godot_variant_as_array(&gdn);
	return *reinterpret_cast<Array *>(&ret);
};

Variant::~Variant() {
	api->godot_variant_destroy(&gdn);
};
