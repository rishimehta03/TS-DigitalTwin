extends Node

# MQTT + Digital Twin Integration Demo (Debug Version)

var mqtt_client: MQTTClient
var motor_twin: DigitalTwin

func _ready():
	print("=== MQTT + Digital Twin Integration Demo (DEBUG) ===")
	
	# Create a digital twin for a motor
	motor_twin = DigitalTwin.new()
	motor_twin.set_twin_id("MOTOR_001")
	motor_twin.set_twin_name("Industrial Motor")
	motor_twin.set_twin_type("Motor")
	
	# Register with TwinRegistry
	var registry = TwinRegistry
	registry.register_twin(motor_twin)
	print("✓ Digital twin registered")
	
	# Create MQTT client
	mqtt_client = MQTTClient.new()
	
	# Connect signals
	mqtt_client.connect("connected", _on_mqtt_connected)
	mqtt_client.connect("message_received", _on_mqtt_message)
	mqtt_client.connect("connection_failed", _on_connection_failed)
	
	print("✓ Signals connected")
	
	# Connect to local MQTT broker
	print("→ Connecting to local MQTT broker (127.0.0.1:1883)...")
	mqtt_client.connect_to_broker("127.0.0.1", 1883, "tdtstack_demo")

func _process(_delta):
	if mqtt_client:
		mqtt_client.poll()  # Process incoming MQTT messages

func _on_mqtt_connected():
	print("\n🟢 MQTT: Connected successfully!")
	print("   Connection state:", mqtt_client.get_connection_state())
	
	# Subscribe to motor data topics
	print("\n→ Subscribing to topics...")
	mqtt_client.subscribe("tdtstack/demo/motor001/temperature")
	mqtt_client.subscribe("tdtstack/demo/motor001/rpm")
	mqtt_client.subscribe("tdtstack/demo/motor001/voltage")
	
	print("\n✓ Subscribed to motor topics!")
	print("\n📡 Now listening for messages...")
	print("   Test with: mosquitto_pub -h localhost -t \"tdtstack/demo/motor001/temperature\" -m \"92.3\"")

func _on_mqtt_message(topic: String, payload: String):
	print("\n📨 MQTT Message Received!")
	print("   Topic:   ", topic)
	print("   Payload: ", payload)
	
	# Parse topic to get property name
	var parts = topic.split("/")
	print("   Topic parts: ", parts)
	
	if parts.size() >= 4:
		var property_name = parts[3]  # Last part is the property
		
		# Update digital twin property
		var value
		if payload.is_valid_float():
			value = payload.to_float()
			print("   Parsed as float: ", value)
		else:
			value = payload
			print("   Kept as string: ", value)
		
		motor_twin.set_property(property_name, value)
		
		print("   ✓ Updated twin property: ", property_name, " = ", value)
		print("   ✓ All properties: ", motor_twin.get_all_properties())
	else:
		print("   ✗ Invalid topic format (expected 4+ parts)")

func _on_connection_failed(reason: String):
	print("\n🔴 MQTT Connection Failed: ", reason)

func _exit_tree():
	if mqtt_client:
		print("\n→ Disconnecting from broker...")
		mqtt_client.disconnect_from_broker()
		print("✓ Disconnected")
