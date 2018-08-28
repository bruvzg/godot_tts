/*************************************************************************/
/*  dictionary.cpp                                                       */
/*************************************************************************/

#include <dictionary.h>
#include <variant.h>

Dictionary::Dictionary() {
	api->godot_dictionary_new(&gdn);
};

Dictionary::Dictionary(const Dictionary & p_dict) {
	api->godot_dictionary_new_copy(&gdn, &p_dict.gdn);
};

Dictionary &Dictionary::operator=(const Dictionary &p_dict) {
	api->godot_dictionary_destroy(&gdn);
	api->godot_dictionary_new_copy(&gdn, &p_dict.gdn);
	return *this;
};

void Dictionary::set(const Variant& p_key, const Variant& p_value) {
	api->godot_dictionary_set(&gdn, reinterpret_cast<const godot_variant *>(&p_key), reinterpret_cast<const godot_variant *>(&p_value));
};

Dictionary::~Dictionary() {
	api->godot_dictionary_destroy(&gdn);
};
