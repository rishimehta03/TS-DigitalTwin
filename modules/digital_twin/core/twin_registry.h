/**************************************************************************/
/*  twin_registry.h                                                        */
/**************************************************************************/
/*                         TECHNO DIGITAL TWIN STACK                      */
/**************************************************************************/
/* Copyright (c) 2026 Technostacks Infotech Private Limited.             */
/*------------------------------------------------------------------------------------------------*/

#ifndef TWIN_REGISTRY_H
#define TWIN_REGISTRY_H

#include "core/object/object.h"
#include "core/templates/hash_map.h"
#include "core/templates/vector.h"
#include "digital_twin.h"

/**
 * @brief Singleton registry for managing all digital twin instances.
 * 
 * The TwinRegistry provides centralized management of digital twins,
 * including registration, lookup, and lifecycle management.
 */
class TwinRegistry : public Object {
	GDCLASS(TwinRegistry, Object);

private:
	static TwinRegistry *singleton;
	HashMap<String, Ref<DigitalTwin>> twins; // Map of twin_id -> DigitalTwin
	Vector<Ref<DigitalTwin>> twin_list; // Ordered list for iteration

protected:
	static void _bind_methods();

public:
	static TwinRegistry *get_singleton();

	TwinRegistry();
	~TwinRegistry();

	// Twin management
	void register_twin(const Ref<DigitalTwin> &p_twin);
	void unregister_twin(const String &p_twin_id);
	Ref<DigitalTwin> get_twin(const String &p_twin_id) const;
	bool has_twin(const String &p_twin_id) const;

	// Queries
	Array get_all_twins() const;
	Array get_twins_by_type(const String &p_type) const;
	int get_twin_count() const;

	// Utility
	void clear();
	Array get_twin_ids() const;
};

#endif // TWIN_REGISTRY_H
