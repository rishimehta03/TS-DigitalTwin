# MQTT Protocol Module

## Overview

The MQTT Protocol module provides industrial IoT connectivity for the TDT Stack Digital Twin platform. It enables real-time bidirectional communication between physical devices and their digital twin representations using the MQTT messaging protocol.

## Features

- **MQTT Client**: Connect to any MQTT broker (Mosquitto, HiveMQ, AWS IoT Core, Azure IoT Hub)
- **Auto-Discovery**: Automatic device discovery via MQTT topics
- **Data Mapping**: Flexible topic-to-property mapping system
- **QoS Support**: Quality of Service levels 0, 1, and 2
- **Reconnection**: Automatic reconnection with exponential backoff
- **Thread-Safe**: Non-blocking async operation

## Architecture

```
IoT Device → MQTT Broker → MQTTClient → DataMapper → DigitalTwin
```

## Usage

```gdscript
# Create MQTT client
var mqtt = MQTTClient.new()
mqtt.connect_to_broker("test.mosquitto.org", 1883)

# Subscribe to device topics
mqtt.subscribe("devices/motor001/+")

# Auto-update digital twin
var protocol_mgr = ProtocolManager.get_singleton()
protocol_mgr.map_topic_to_twin("devices/motor001", "MOTOR_001")

# Now incoming MQTT messages automatically update the twin!
```

## Dependencies

- Eclipse Paho MQTT C++ (included in `thirdparty/`)
- Godot JSON module (built-in)

## License

MIT License - Compatible with Godot Engine
