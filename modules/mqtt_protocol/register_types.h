/**************************************************************************/
/*  register_types.h                                                       */
/**************************************************************************/
/*                         TECHNO DIGITAL TWIN STACK                      */
/**************************************************************************/

#ifndef MQTT_PROTOCOL_REGISTER_TYPES_H
#define MQTT_PROTOCOL_REGISTER_TYPES_H

#include "modules/register_module_types.h"

void initialize_mqtt_protocol_module(ModuleInitializationLevel p_level);
void uninitialize_mqtt_protocol_module(ModuleInitializationLevel p_level);

#endif // MQTT_PROTOCOL_REGISTER_TYPES_H
