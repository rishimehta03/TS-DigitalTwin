/**************************************************************************/
/*  twin_registry.cpp                                                      */
/**************************************************************************/
/*                         TECHNO DIGITAL TWIN STACK                      */
/**************************************************************************/
/* Copyright (c) 2026 Technostacks Infotech Private Limited.             */
/*--------------------------------------------------------------------------*/

#include "twin_registry.h"

TwinRegistry *TwinRegistry::singleton = nullptr;

void TwinRegistry::_bind_methods() {
	ClassDB::bind_method(D_METHOD("register_twin", "twin"), &TwinRegistry::register_twin);
	ClassDB::bind_method(D_METHOD("unregister_twin", "twin_id"), &TwinRegistry::unregister_twin);
	ClassDB::bind_method(D_METHOD("get_twin", "twin_id"), &TwinRegistry::get_twin);
	ClassDB::bind_method(D_METHOD("has_twin", "twin_id"), &TwinRegistry::has_twin);
	ClassDB::bind_method(D_METHOD("get_all_twins"), &TwinRegistry::get_all_twins);
	ClassDB::bind_method(D_METHOD("get_twins_by_type", "type"), &TwinRegistry::get_twins_by_type);
	ClassDB::bind_method(D_METHOD("get_twin_count"), &TwinRegistry::get_twin_count);
	ClassDB::bind_method(D_METHOD("clear"), &TwinRegistry::clear);
	ClassDB::bind_method(D_METHOD("get_twin_ids"), &TwinRegistry::get_twin_ids);

	ADD_SIGNAL(MethodInfo("twin_registered", PropertyInfo(Variant::OBJECT, "twin", PROPERTY_HINT_RESOURCE_TYPE, "DigitalTwin")));
	ADD_SIGNAL(MethodInfo("twin_unregistered", PropertyInfo(Variant::STRING, "twin_id")));
}

TwinRegistry *TwinRegistry::get_singleton() {
	return singleton;
}

TwinRegistry::TwinRegistry() {
	ERR_FAIL_COND_MSG(singleton != nullptr, "TwinRegistry singleton already exists!");
	singleton = this;
}

TwinRegistry::~TwinRegistry() {
	singleton = nullptr;
}

void TwinRegistry::register_twin(const Ref<DigitalTwin> &p_twin) {
	ERR_FAIL_COND_MSG(p_twin.is_null(), "Cannot register null twin");

	String twin_id = p_twin->get_twin_id();
	ERR_FAIL_COND_MSG(twin_id.is_empty(), "Twin must have a valid ID before registration");

	if (twins.has(twin_id)) {
		WARN_PRINT(vformat("Twin with ID '%s' is already registered. Replacing existing twin.", twin_id));
		// Remove from list first
		twin_list.erase(twins[twin_id]);
	}

	twins[twin_id] = p_twin;
	twin_list.push_back(p_twin);

	emit_signal("twin_registered", p_twin);
	print_line(vformat("Digital Twin registered: %s", p_twin->to_string()));
}

void TwinRegistry::unregister_twin(const String &p_twin_id) {
	if (!twins.has(p_twin_id)) {
		WARN_PRINT(vformat("Attempted to unregister non-existent twin: %s", p_twin_id));
		return;
	}

	Ref<DigitalTwin> twin = twins[p_twin_id];
	twin_list.erase(twin);
	twins.erase(p_twin_id);

	emit_signal("twin_unregistered", p_twin_id);
	print_line(vformat("Digital Twin unregistered: %s", p_twin_id));
}

Ref<DigitalTwin> TwinRegistry::get_twin(const String &p_twin_id) const {
	if (twins.has(p_twin_id)) {
		return twins[p_twin_id];
	}
	return Ref<DigitalTwin>();
}

bool TwinRegistry::has_twin(const String &p_twin_id) const {
	return twins.has(p_twin_id);
}

Array TwinRegistry::get_all_twins() const {
	Array result;
	for (const Ref<DigitalTwin> &twin : twin_list) {
		result.push_back(twin);
	}
	return result;
}

Array TwinRegistry::get_twins_by_type(const String &p_type) const {
	Array result;
	for (const Ref<DigitalTwin> &twin : twin_list) {
		if (twin->get_twin_type() == p_type) {
			result.push_back(twin);
		}
	}
	return result;
}

int TwinRegistry::get_twin_count() const {
	return twin_list.size();
}

void TwinRegistry::clear() {
	twins.clear();
	twin_list.clear();
	print_line("TwinRegistry cleared");
}

Array TwinRegistry::get_twin_ids() const {
	Array ids;
	for (const Ref<DigitalTwin> &twin : twin_list) {
		ids.push_back(twin->get_twin_id());
	}
	return ids;
}
