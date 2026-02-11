/**************************************************************************/
/*  mqtt_client.h                                                          */
/**************************************************************************/
/*                         TECHNO DIGITAL TWIN STACK                      */
/**************************************************************************/

#ifndef MQTT_CLIENT_H
#define MQTT_CLIENT_H

#include "core/io/stream_peer_tcp.h"
#include "core/object/ref_counted.h"
#include "core/string/ustring.h"
#include "core/variant/dictionary.h"

/**
 * @brief Lightweight MQTT 3.1.1 client for IoT device connectivity.
 * 
 * Provides basic MQTT functionality using Godot's StreamPeerTCP for
 * connecting to MQTT brokers, subscribing to topics, and publishing messages.
 */
class MQTTClient : public RefCounted {
	GDCLASS(MQTTClient, RefCounted);

public:
	enum ConnectionState {
		STATE_DISCONNECTED,
		STATE_CONNECTING,
		STATE_CONNECTED,
		STATE_ERROR
	};

	enum QoS {
		QOS_AT_MOST_ONCE = 0,  // Fire and forget
		QOS_AT_LEAST_ONCE = 1, // Acknowledged delivery
		QOS_EXACTLY_ONCE = 2   // Assured delivery
	};

private:
	Ref<StreamPeerTCP> tcp_client;
	ConnectionState state;
	String broker_address;
	int broker_port;
	String client_id;
	String username;
	String password;
	
	uint16_t packet_id_counter;
	bool keep_alive_enabled;
	float keep_alive_interval;
	float last_activity_time;
	
	// Subscribed topics and callbacks
	Dictionary subscriptions; // topic -> QoS
	
	// Internal protocol methods
	PackedByteArray _build_connect_packet();
	PackedByteArray _build_subscribe_packet(const String &p_topic, QoS p_qos);
	PackedByteArray _build_publish_packet(const String &p_topic, const String &p_payload, QoS p_qos, bool p_retain);
	PackedByteArray _build_pingreq_packet();
	PackedByteArray _build_disconnect_packet();
	
	void _encode_string(PackedByteArray &p_buffer, const String &p_string);
	void _encode_remaining_length(PackedByteArray &p_buffer, uint32_t p_length);
	uint16_t _get_next_packet_id();
	
	void _process_incoming_data();
	void _handle_connack(const PackedByteArray &p_data);
	void _handle_publish(const PackedByteArray &p_data);
	void _handle_suback(const PackedByteArray &p_data);
	void _handle_pingresp(const PackedByteArray &p_data);

protected:
	static void _bind_methods();

public:
	MQTTClient();
	~MQTTClient();

	// Connection management
	Error connect_to_broker(const String &p_address, int p_port = 1883, const String &p_client_id = "");
	void disconnect_from_broker();
	bool is_connected() const;
	ConnectionState get_connection_state() const;

	// Authentication
	void set_credentials(const String &p_username, const String &p_password);

	// Subscription
	Error subscribe(const String &p_topic, QoS p_qos = QOS_AT_MOST_ONCE);
	Error unsubscribe(const String &p_topic);

	// Publishing
	Error publish(const String &p_topic, const String &p_payload, QoS p_qos = QOS_AT_MOST_ONCE, bool p_retain = false);

	// Keep-alive
	void set_keep_alive(bool p_enabled, float p_interval = 60.0);

	// Poll for messages (call in _process)
	void poll();

	// Utility
	String get_broker_address() const;
	int get_broker_port() const;
};

VARIANT_ENUM_CAST(MQTTClient::ConnectionState);
VARIANT_ENUM_CAST(MQTTClient::QoS);

#endif // MQTT_CLIENT_H
