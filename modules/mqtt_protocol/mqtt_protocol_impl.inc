/**************************************************************************/
/*  mqtt_client_protocol.cpp - MQTT Protocol Packet Builders              */
/**************************************************************************/

// Continue from mqtt_client.cpp

PackedByteArray MQTTClient::_build_connect_packet() {
	PackedByteArray packet;

	// Fixed header
	packet.push_back(MQTT_CONNECT);

	// Variable header + payload (calculate length first)
	PackedByteArray variable_header;

	// Protocol name "MQTT"
	_encode_string(variable_header, "MQTT");

	// Protocol level (3.1.1 = 4)
	variable_header.push_back(4);

	// Connect flags
	uint8_t flags = 0x02; // Clean session
	if (!username.is_empty()) {
		flags |= 0x80; // Username flag
		if (!password.is_empty()) {
			flags |= 0x40; // Password flag
		}
	}
	variable_header.push_back(flags);

	// Keep alive (seconds)
	uint16_t keep_alive = (uint16_t)keep_alive_interval;
	variable_header.push_back((keep_alive >> 8) & 0xFF);
	variable_header.push_back(keep_alive & 0xFF);

	// Payload: Client ID
	_encode_string(variable_header, client_id);

	// Username and password if provided
	if (!username.is_empty()) {
		_encode_string(variable_header, username);
		if (!password.is_empty()) {
			_encode_string(variable_header, password);
		}
	}

	// Remaining length
	_encode_remaining_length(packet, variable_header.size());

	// Append variable header
	packet.append_array(variable_header);

	return packet;
}

PackedByteArray MQTTClient::_build_subscribe_packet(const String &p_topic, QoS p_qos) {
	PackedByteArray packet;

	// Fixed header
	packet.push_back(MQTT_SUBSCRIBE);

	// Variable header + payload
	PackedByteArray variable_header;

	// Packet ID
	uint16_t pid = _get_next_packet_id();
	variable_header.push_back((pid >> 8) & 0xFF);
	variable_header.push_back(pid & 0xFF);

	// Topic filter
	_encode_string(variable_header, p_topic);

	// Requested QoS
	variable_header.push_back((uint8_t)p_qos);

	// Remaining length
	_encode_remaining_length(packet, variable_header.size());

	// Append variable header
	packet.append_array(variable_header);

	return packet;
}

PackedByteArray MQTTClient::_build_publish_packet(const String &p_topic, const String &p_payload, QoS p_qos, bool p_retain) {
	PackedByteArray packet;

	// Fixed header with flags
	uint8_t fixed_header = MQTT_PUBLISH;
	if (p_retain) {
		fixed_header |= 0x01;
	}
	fixed_header |= ((uint8_t)p_qos << 1);
	packet.push_back(fixed_header);

	// Variable header + payload
	PackedByteArray content;

	// Topic name
	_encode_string(content, p_topic);

	// Packet ID (only for QoS > 0)
	if (p_qos > QOS_AT_MOST_ONCE) {
		uint16_t pid = _get_next_packet_id();
		content.push_back((pid >> 8) & 0xFF);
		content.push_back(pid & 0xFF);
	}

	// Payload
	CharString payload_utf8 = p_payload.utf8();
	for (int i = 0; i < payload_utf8.length(); i++) {
		content.push_back(payload_utf8[i]);
	}

	// Remaining length
	_encode_remaining_length(packet, content.size());

	// Append content
	packet.append_array(content);

	return packet;
}

PackedByteArray MQTTClient::_build_pingreq_packet() {
	PackedByteArray packet;
	packet.push_back(MQTT_PINGREQ);
	packet.push_back(0); // Remaining length = 0
	return packet;
}

PackedByteArray MQTTClient::_build_disconnect_packet() {
	PackedByteArray packet;
	packet.push_back(MQTT_DISCONNECT);
	packet.push_back(0); // Remaining length = 0
	return packet;
}

void MQTTClient::_process_incoming_data() {
	int available = tcp_client->get_available_bytes();
	if (available <= 0) {
		return;
	}

	// Read all available data
	PackedByteArray data;
	data.resize(available);
	int received;
	Error err = tcp_client->get_partial_data(data.ptrw(), available, received);
	
	if (err != OK || received == 0) {
		return;
	}

	data.resize(received);
	last_activity_time = Time::get_singleton()->get_ticks_msec() / 1000.0;

	uint8_t packet_type = data[0] & 0xF0;

	switch (packet_type) {
		case MQTT_CONNACK:
			_handle_connack(data);
			break;
		case MQTT_PUBLISH:
			_handle_publish(data);
			break;
		case MQTT_SUBACK:
			_handle_suback(data);
			break;
		case MQTT_PINGRESP:
			_handle_pingresp(data);
			break;
		default:
			// Unknown packet type, ignore
			break;
	}
}

void MQTTClient::_handle_connack(const PackedByteArray &p_data) {
	if (p_data.size() < 4) {
		return;
	}

	uint8_t return_code = p_data[3];

	if (return_code == 0) {
		state = STATE_CONNECTED;
		emit_signal("connected");
		print_line("MQTT: Connected to broker successfully");
	} else {
		state = STATE_ERROR;
		String error_msg = "Connection refused (code: " + itos(return_code) + ")";
		emit_signal("connection_failed", error_msg);
		print_line("MQTT: " + error_msg);
	}
}

void MQTTClient::_handle_publish(const PackedByteArray &p_data) {
	if (p_data.size() < 5) {
		return;
	}

	// Skip remaining length bytes
	int offset = 2;

	// Read topic length
	uint16_t topic_len = (p_data[offset] << 8) | p_data[offset + 1];
	offset += 2;

	if (offset + topic_len > p_data.size()) {
		return;
	}

	// Read topic
	String topic;
	for (int i = 0; i < topic_len; i++) {
		topic += String::chr(p_data[offset + i]);
	}
	offset += topic_len;

	// QoS > 0 would have packet ID here, skip for now (assume QoS 0)

	// Read payload
	String payload;
	for (int i = offset; i < p_data.size(); i++) {
		payload += String::chr(p_data[i]);
	}

	emit_signal("message_received", topic, payload);
	print_line(vformat("MQTT: Message received on '%s': %s", topic, payload));
}

void MQTTClient::_handle_suback(const PackedByteArray &p_data) {
	// Subscription acknowledged
	print_line("MQTT: Subscription acknowledged");
}

void MQTTClient::_handle_pingresp(const PackedByteArray &p_data) {
	// Ping response received, connection is alive
}
