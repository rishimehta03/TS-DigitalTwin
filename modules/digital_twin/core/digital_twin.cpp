/**************************************************************************/
/*  digital_twin.cpp                                                       */
/**************************************************************************/
/*                         TECHNO DIGITAL TWIN STACK                      */
/**************************************************************************/
/* Copyright (c) 2026 Technostacks Infotech Private Limited.             */
/*                                                                        */
/* Portions of this software are derived from Godot Engine,              */
/* Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md).*/
/*                                                                        */
/* Permission is hereby granted, free of charge, to any person obtaining  */
/* a copy of this software and associated documentation files (the        */
/* "Software"), to deal in the Software without restriction, including    */
/* without limitation the rights to use, copy, modify, merge, publish,    */
/* distribute, sublicense, and/or sell copies of the Software, and to     */
/* permit persons to whom the Software is furnished to do so, subject to  */
/* the following conditions:                                              */
/*                                                                        */
/* The above copyright notice and this permission notice shall be         */
/* included in all copies or substantial portions of the Software.        */
/*                                                                        */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,        */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF     */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. */
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY   */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,   */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE      */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                 */
/**************************************************************************/

#include "digital_twin.h"

#include "core/os/time.h"
#include "core/variant/variant.h"

void DigitalTwin::_bind_methods() {
	// Identification
	ClassDB::bind_method(D_METHOD("set_twin_id", "id"), &DigitalTwin::set_twin_id);
	ClassDB::bind_method(D_METHOD("get_twin_id"), &DigitalTwin::get_twin_id);

	ClassDB::bind_method(D_METHOD("set_twin_name", "name"), &DigitalTwin::set_twin_name);
	ClassDB::bind_method(D_METHOD("get_twin_name"), &DigitalTwin::get_twin_name);

	ClassDB::bind_method(D_METHOD("set_twin_type", "type"), &DigitalTwin::set_twin_type);
	ClassDB::bind_method(D_METHOD("get_twin_type"), &DigitalTwin::get_twin_type);

	// Properties
	ClassDB::bind_method(D_METHOD("set_property", "key", "value"), &DigitalTwin::set_property);
	ClassDB::bind_method(D_METHOD("get_property", "key", "default"), &DigitalTwin::get_property, DEFVAL(Variant()));
	ClassDB::bind_method(D_METHOD("has_property", "key"), &DigitalTwin::has_property);
	ClassDB::bind_method(D_METHOD("remove_property", "key"), &DigitalTwin::remove_property);
	ClassDB::bind_method(D_METHOD("get_all_properties"), &DigitalTwin::get_all_properties);
	ClassDB::bind_method(D_METHOD("set_all_properties", "properties"), &DigitalTwin::set_all_properties);

	// Metadata
	ClassDB::bind_method(D_METHOD("set_metadata", "key", "value"), &DigitalTwin::set_metadata);
	ClassDB::bind_method(D_METHOD("get_metadata", "key", "default"), &DigitalTwin::get_metadata, DEFVAL(Variant()));
	ClassDB::bind_method(D_METHOD("get_all_metadata"), &DigitalTwin::get_all_metadata);

	// Synchronization
	ClassDB::bind_method(D_METHOD("set_synchronized", "synchronized"), &DigitalTwin::set_synchronized);
	ClassDB::bind_method(D_METHOD("is_twin_synchronized"), &DigitalTwin::is_twin_synchronized);
	ClassDB::bind_method(D_METHOD("update_timestamp"), &DigitalTwin::update_timestamp);
	ClassDB::bind_method(D_METHOD("get_last_update_time"), &DigitalTwin::get_last_update_time);

	// State
	ClassDB::bind_method(D_METHOD("to_dict"), &DigitalTwin::to_dict);
	ClassDB::bind_method(D_METHOD("from_dict", "dict"), &DigitalTwin::from_dict);
	// Note: to_string() not bound to avoid conflict with Object::to_string()
	// Use _to_string() override instead for GDScript print()

	// Signals
	ADD_SIGNAL(MethodInfo("property_changed", PropertyInfo(Variant::STRING, "property_name"), PropertyInfo(Variant::NIL, "new_value")));
	ADD_SIGNAL(MethodInfo("state_changed"));
	ADD_SIGNAL(MethodInfo("synchronization_changed", PropertyInfo(Variant::BOOL, "is_synchronized")));

	// Properties for inspector
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "twin_id"), "set_twin_id", "get_twin_id");
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "twin_name"), "set_twin_name", "get_twin_name");
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "twin_type"), "set_twin_type", "get_twin_type");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "synchronized"), "set_synchronized", "is_twin_synchronized");
}

DigitalTwin::DigitalTwin() {
	twin_id = "";
	twin_name = "Unnamed Twin";
	twin_type = "Generic";
	is_synchronized = false;
	last_update_time = 0;

	// Initialize metadata with creation timestamp
	metadata["created_at"] = Time::get_singleton()->get_unix_time_from_system();
	update_timestamp();
}

DigitalTwin::~DigitalTwin() {
}

// Identification methods
void DigitalTwin::set_twin_id(const String &p_id) {
	twin_id = p_id;
}

String DigitalTwin::get_twin_id() const {
	return twin_id;
}

void DigitalTwin::set_twin_name(const String &p_name) {
	twin_name = p_name;
}

String DigitalTwin::get_twin_name() const {
	return twin_name;
}

void DigitalTwin::set_twin_type(const String &p_type) {
	twin_type = p_type;
}

String DigitalTwin::get_twin_type() const {
	return twin_type;
}

// Property management
void DigitalTwin::set_property(const String &p_key, const Variant &p_value) {
	properties[p_key] = p_value;
	update_timestamp();
	emit_signal("property_changed", p_key, p_value);
	emit_signal("state_changed");
}

Variant DigitalTwin::get_property(const String &p_key, const Variant &p_default) const {
	if (properties.has(p_key)) {
		return properties[p_key];
	}
	return p_default;
}

bool DigitalTwin::has_property(const String &p_key) const {
	return properties.has(p_key);
}

void DigitalTwin::remove_property(const String &p_key) {
	if (properties.has(p_key)) {
		properties.erase(p_key);
		update_timestamp();
		emit_signal("state_changed");
	}
}

Dictionary DigitalTwin::get_all_properties() const {
	return properties;
}

void DigitalTwin::set_all_properties(const Dictionary &p_properties) {
	properties = p_properties;
	update_timestamp();
	emit_signal("state_changed");
}

// Metadata management
void DigitalTwin::set_metadata(const String &p_key, const Variant &p_value) {
	metadata[p_key] = p_value;
}

Variant DigitalTwin::get_metadata(const String &p_key, const Variant &p_default) const {
	if (metadata.has(p_key)) {
		return metadata[p_key];
	}
	return p_default;
}

Dictionary DigitalTwin::get_all_metadata() const {
	return metadata;
}

// Synchronization
void DigitalTwin::set_synchronized(bool p_synchronized) {
	if (is_synchronized != p_synchronized) {
		is_synchronized = p_synchronized;
		emit_signal("synchronization_changed", is_synchronized);
	}
}

bool DigitalTwin::is_twin_synchronized() const {
	return is_synchronized;
}

void DigitalTwin::update_timestamp() {
	last_update_time = Time::get_singleton()->get_unix_time_from_system();
	metadata["modified_at"] = last_update_time;
}

uint64_t DigitalTwin::get_last_update_time() const {
	return last_update_time;
}

// State serialization
Dictionary DigitalTwin::to_dict() const {
	Dictionary dict;
	dict["twin_id"] = twin_id;
	dict["twin_name"] = twin_name;
	dict["twin_type"] = twin_type;
	dict["properties"] = properties;
	dict["metadata"] = metadata;
	dict["is_synchronized"] = is_synchronized;
	dict["last_update_time"] = last_update_time;
	return dict;
}

void DigitalTwin::from_dict(const Dictionary &p_dict) {
	if (p_dict.has("twin_id")) {
		twin_id = p_dict["twin_id"];
	}
	if (p_dict.has("twin_name")) {
		twin_name = p_dict["twin_name"];
	}
	if (p_dict.has("twin_type")) {
		twin_type = p_dict["twin_type"];
	}
	if (p_dict.has("properties")) {
		properties = p_dict["properties"];
	}
	if (p_dict.has("metadata")) {
		metadata = p_dict["metadata"];
	}
	if (p_dict.has("is_synchronized")) {
		is_synchronized = p_dict["is_synchronized"];
	}
	if (p_dict.has("last_update_time")) {
		last_update_time = p_dict["last_update_time"];
	}
	emit_signal("state_changed");
}

String DigitalTwin::to_string() const {
	return vformat("DigitalTwin(id=%s, name=%s, type=%s, props=%d, synced=%s)",
			twin_id, twin_name, twin_type, properties.size(), is_synchronized ? "true" : "false");
}
