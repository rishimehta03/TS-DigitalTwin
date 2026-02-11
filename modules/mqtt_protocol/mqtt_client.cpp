/**************************************************************************/
/*  mqtt_client.cpp - Basic MQTT 3.1.1 Implementation (Part 1)            */
/**************************************************************************/
/*                         TECHNO DIGITAL TWIN STACK                      */
/**************************************************************************/

#include "mqtt_client.h"

#include "core/io/marshalls.h"
#include "core/os/time.h"
#include "core/variant/variant.h"

// MQTT Control Packet Types
#define MQTT_CONNECT 0x10
#define MQTT_CONNACK 0x20
#define MQTT_PUBLISH 0x30
#define MQTT_PUBACK 0x40
#define MQTT_SUBSCRIBE 0x82
#define MQTT_SUBACK 0x90
#define MQTT_PINGREQ 0xC0
#define MQTT_PINGRESP 0xD0
#define MQTT_DISCONNECT 0xE0

void MQTTClient::_bind_methods() {
	// Connection
	ClassDB::bind_method(D_METHOD("connect_to_broker", "address", "port", "client_id"), &MQTTClient::connect_to_broker, DEFVAL(1883), DEFVAL(""));
	ClassDB::bind_method(D_METHOD("disconnect_from_broker"), &MQTTClient::disconnect_from_broker);
	ClassDB::bind_method(D_METHOD("is_connected"), &MQTTClient::is_connected);
	ClassDB::bind_method(D_METHOD("get_connection_state"), &MQTTClient::get_connection_state);

	// Authentication
	ClassDB::bind_method(D_METHOD("set_credentials", "username", "password"), &MQTTClient::set_credentials);

	// Subscription
	ClassDB::bind_method(D_METHOD("subscribe", "topic", "qos"), &MQTTClient::subscribe, DEFVAL(QOS_AT_MOST_ONCE));
	ClassDB::bind_method(D_METHOD("unsubscribe", "topic"), &MQTTClient::unsubscribe);

	// Publishing
	ClassDB::bind_method(D_METHOD("publish", "topic", "payload", "qos", "retain"), &MQTTClient::publish, DEFVAL(QOS_AT_MOST_ONCE), DEFVAL(false));

	// Maintenance
	ClassDB::bind_method(D_METHOD("set_keep_alive", "enabled", "interval"), &MQTTClient::set_keep_alive, DEFVAL(60.0));
	ClassDB::bind_method(D_METHOD("poll"), &MQTTClient::poll);

	// Getters
	ClassDB::bind_method(D_METHOD("get_broker_address"), &MQTTClient::get_broker_address);
	ClassDB::bind_method(D_METHOD("get_broker_port"), &MQTTClient::get_broker_port);

	// Signals
	ADD_SIGNAL(MethodInfo("connected"));
	ADD_SIGNAL(MethodInfo("disconnected"));
	ADD_SIGNAL(MethodInfo("connection_failed", PropertyInfo(Variant::STRING, "reason")));
	ADD_SIGNAL(MethodInfo("message_received", PropertyInfo(Variant::STRING, "topic"), PropertyInfo(Variant::STRING, "payload")));
	ADD_SIGNAL(MethodInfo("subscribed", PropertyInfo(Variant::STRING, "topic")));

	// Enums
	BIND_ENUM_CONSTANT(STATE_DISCONNECTED);
	BIND_ENUM_CONSTANT(STATE_CONNECTING);
	BIND_ENUM_CONSTANT(STATE_CONNECTED);
	BIND_ENUM_CONSTANT(STATE_ERROR);

	BIND_ENUM_CONSTANT(QOS_AT_MOST_ONCE);
	BIND_ENUM_CONSTANT(QOS_AT_LEAST_ONCE);
	BIND_ENUM_CONSTANT(QOS_EXACTLY_ONCE);
}

MQTTClient::MQTTClient() {
	tcp_client.instantiate();
	state = STATE_DISCONNECTED;
	broker_port = 1883;
	packet_id_counter = 1;
	keep_alive_enabled = true;
	keep_alive_interval = 60.0;
	last_activity_time = 0;
}

MQTTClient::~MQTTClient() {
	disconnect_from_broker();
}

Error MQTTClient::connect_to_broker(const String &p_address, int p_port, const String &p_client_id) {
	if (state != STATE_DISCONNECTED) {
		WARN_PRINT("Already connected or connecting to MQTT broker");
		return ERR_ALREADY_IN_USE;
	}

	broker_address = p_address;
	broker_port = p_port;
	client_id = p_client_id.is_empty() ? "tdtstack_" + itos(Time::get_singleton()->get_ticks_usec()) : p_client_id;

	print_line(vformat("MQTT: Connecting to %s:%d as client '%s'", broker_address, broker_port, client_id));

	Error err = tcp_client->connect_to_host(broker_address, broker_port);
	if (err != OK) {
		state = STATE_ERROR;
		emit_signal("connection_failed", "Failed to connect to host");
		return err;
	}

	state = STATE_CONNECTING;
	return OK;
}

void MQTTClient::disconnect_from_broker() {
	if (state == STATE_DISCONNECTED) {
		return;
	}

	if (state == STATE_CONNECTED) {
		// Send DISCONNECT packet
		PackedByteArray disconnect_packet = _build_disconnect_packet();
		tcp_client->put_data(disconnect_packet.ptr(), disconnect_packet.size());
	}

	tcp_client->disconnect_from_host();
	state = STATE_DISCONNECTED;
	subscriptions.clear();
	emit_signal("disconnected");
	print_line("MQTT: Disconnected from broker");
}

bool MQTTClient::is_connected() const {
	return state == STATE_CONNECTED && tcp_client->get_status() == StreamPeerTCP::STATUS_CONNECTED;
}

MQTTClient::ConnectionState MQTTClient::get_connection_state() const {
	return state;
}

void MQTTClient::set_credentials(const String &p_username, const String &p_password) {
	username = p_username;
	password = p_password;
}

Error MQTTClient::subscribe(const String &p_topic, QoS p_qos) {
	if (!is_connected()) {
		WARN_PRINT("Cannot subscribe: not connected to broker");
		return ERR_UNCONFIGURED;
	}

	PackedByteArray subscribe_packet = _build_subscribe_packet(p_topic, p_qos);
	Error err = tcp_client->put_data(subscribe_packet.ptr(), subscribe_packet.size());
	
	if (err == OK) {
		subscriptions[p_topic] = (int)p_qos;
		print_line(vformat("MQTT: Subscribed to topic '%s' with QoS %d", p_topic, p_qos));
	}

	return err;
}

Error MQTTClient::unsubscribe(const String &p_topic) {
	if (!is_connected()) {
		return ERR_UNCONFIGURED;
	}

	if (subscriptions.has(p_topic)) {
		subscriptions.erase(p_topic);
		print_line(vformat("MQTT: Unsubscribed from topic '%s'", p_topic));
	}

	return OK;
}

Error MQTTClient::publish(const String &p_topic, const String &p_payload, QoS p_qos, bool p_retain) {
	if (!is_connected()) {
		WARN_PRINT("Cannot publish: not connected to broker");
		return ERR_UNCONFIGURED;
	}

	PackedByteArray publish_packet = _build_publish_packet(p_topic, p_payload, p_qos, p_retain);
	return tcp_client->put_data(publish_packet.ptr(), publish_packet.size());
}

void MQTTClient::set_keep_alive(bool p_enabled, float p_interval) {
	keep_alive_enabled = p_enabled;
	keep_alive_interval = p_interval;
}



String MQTTClient::get_broker_address() const {
	return broker_address;
}

int MQTTClient::get_broker_port() const {
	return broker_port;
}

// Internal protocol methods
void MQTTClient::_encode_string(PackedByteArray &p_buffer, const String &p_string) {
	CharString utf8 = p_string.utf8();
	uint16_t len = utf8.length();
	p_buffer.push_back((len >> 8) & 0xFF);
	p_buffer.push_back(len & 0xFF);
	for (int i = 0; i < len; i++) {
		p_buffer.push_back(utf8[i]);
	}
}

void MQTTClient::_encode_remaining_length(PackedByteArray &p_buffer, uint32_t p_length) {
	do {
		uint8_t byte = p_length % 128;
		p_length /= 128;
		if (p_length > 0) {
			byte |= 0x80;
		}
		p_buffer.push_back(byte);
	} while (p_length > 0);
}

uint16_t MQTTClient::_get_next_packet_id() {
	uint16_t id = packet_id_counter++;
	if (packet_id_counter == 0) {
		packet_id_counter = 1;
	}
	return id;
}

// Include protocol implementation
#include "mqtt_protocol_impl.inc"

