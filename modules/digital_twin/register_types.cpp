/**************************************************************************/
/*  register_types.cpp                                                     */
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

#include "register_types.h"

#include "core/config/engine.h"
#include "core/object/class_db.h"

// Include core component headers
#include "core/digital_twin.h"
#include "core/twin_registry.h"

void initialize_digital_twin_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}

	// Register classes
	GDREGISTER_CLASS(DigitalTwin);
	GDREGISTER_CLASS(TwinRegistry);

	// Register singleton
	TwinRegistry *twin_registry = memnew(TwinRegistry);
	Engine::get_singleton()->add_singleton(Engine::Singleton("TwinRegistry", TwinRegistry::get_singleton()));

	print_line("Digital Twin module initialized");
}

void uninitialize_digital_twin_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}

	// Cleanup singleton
	TwinRegistry *twin_registry = TwinRegistry::get_singleton();
	if (twin_registry) {
		memdelete(twin_registry);
	}

	print_line("Digital Twin module uninitialized");
}

