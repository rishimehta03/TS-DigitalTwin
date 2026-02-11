/**************************************************************************/
/*  register_types.cpp                                                     */
/**************************************************************************/
/*                         TECHNO DIGITAL TWIN STACK                      */
/**************************************************************************/

#include "register_types.h"

#include "core/object/class_db.h"
#include "mqtt_client.h"

void initialize_mqtt_protocol_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}

	GDREGISTER_CLASS(MQTTClient);

	print_line("MQTT Protocol module initialized");
}

void uninitialize_mqtt_protocol_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}

	print_line("MQTT Protocol module uninitialized");
}
