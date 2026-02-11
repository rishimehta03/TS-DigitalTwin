/**************************************************************************/
/*  digital_twin.h                                                         */
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

#ifndef DIGITAL_TWIN_H
#define DIGITAL_TWIN_H

#include "core/io/resource.h"
#include "core/object/ref_counted.h"
#include "core/string/ustring.h"
#include "core/variant/dictionary.h"
#include "core/variant/variant.h"

/**
 * @brief Core DigitalTwin class representing a digital replica of a physical asset.
 * 
 * A DigitalTwin maintains real-time state synchronization with its physical counterpart,
 * manages properties, and emits events for state changes. It serves as the foundation
 * for all digital twin functionality in the TDT Stack.
 */
class DigitalTwin : public Resource {
	GDCLASS(DigitalTwin, Resource);

private:
	String twin_id; // Unique identifier (UUID format recommended)
	String twin_name; // Human-readable name
	String twin_type; // Type of asset (e.g., "Motor", "Sensor", "Machine")
	Dictionary properties; // Key-value store for twin properties
	Dictionary metadata; // Additional metadata (created_at, modified_at, etc.)
	bool is_synchronized; // Whether the twin is actively syncing with physical asset
	uint64_t last_update_time; // Timestamp of last state update (Unix time in ms)

protected:
	static void _bind_methods();

public:
	DigitalTwin();
	~DigitalTwin();

	// Identification
	void set_twin_id(const String &p_id);
	String get_twin_id() const;

	void set_twin_name(const String &p_name);
	String get_twin_name() const;

	void set_twin_type(const String &p_type);
	String get_twin_type() const;

	// Property management
	void set_property(const String &p_key, const Variant &p_value);
	Variant get_property(const String &p_key, const Variant &p_default = Variant()) const;
	bool has_property(const String &p_key) const;
	void remove_property(const String &p_key);
	Dictionary get_all_properties() const;
	void set_all_properties(const Dictionary &p_properties);

	// Metadata
	void set_metadata(const String &p_key, const Variant &p_value);
	Variant get_metadata(const String &p_key, const Variant &p_default = Variant()) const;
	Dictionary get_all_metadata() const;

	// Synchronization
	void set_synchronized(bool p_synchronized);
	bool is_twin_synchronized() const;

	void update_timestamp();
	uint64_t get_last_update_time() const;

	// State management
	Dictionary to_dict() const;
	void from_dict(const Dictionary &p_dict);

	// Utility
	String to_string() const;
};

#endif // DIGITAL_TWIN_H
