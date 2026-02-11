extends Node

# MQTT + Digital Twin Integration Demo

var mqtt_client: MQTTClient
var motor_twin: DigitalTwin

func _ready():
	print("=== MQTT + Digital Twin Integration Demo ===")
	
	# Create a digital twin for a motor
	motor_twin = DigitalTwin.new()
	motor_twin.set_twin_id("MOTOR_001")
	motor_twin.set_twin_name("Industrial Motor")
	motor_twin.set_twin_type("Motor")
	
	# Register with TwinRegistry
	var registry = TwinRegistry
	registry.register_twin(motor_twin)
	
	# Create MQTT client
	mqtt_client = MQTTClient.new()
	
	# Connect signals
	mqtt_client.connect("connected", _on_mqtt_connected)
	mqtt_client.connect("message_received", _on_mqtt_message)
	mqtt_client.connect("connection_failed", _on_connection_failed)
	
	# Connect to local MQTT broker
	print("Connecting to local MQTT broker...")
	mqtt_client.connect_to_broker("127.0.0.1", 1883, "tdtstack_demo")

func _process(_delta):
	if mqtt_client:
		mqtt_client.poll()  # Process incoming MQTT messages

func _on_mqtt_connected():
	print("MQTT: Connected successfully!")
	
	# Subscribe to motor data topics
	mqtt_client.subscribe("tdtstack/demo/motor001/temperature")
	mqtt_client.subscribe("tdtstack/demo/motor001/rpm")
	mqtt_client.subscribe("tdtstack/demo/motor001/voltage")
	
	print("Subscribed to motor topics.")
	print("\n✅ Ready to receive MQTT messages!")
	print("Test with: mosquitto_pub -h localhost -t \"tdtstack/demo/motor001/temperature\" -m \"75.5\"")

func _on_mqtt_message(topic: String, payload: String):
	print("MQTT Message: ", topic, " = ", payload)
	
	# Parse topic to get property name
	var parts = topic.split("/")
	if parts.size() >= 4:
		var property_name = parts[3]  # Last part is the property
		
		# Update digital twin property
		var value
		if payload.is_valid_float():
			value = payload.to_float()
		else:
			value = payload
		motor_twin.set_property(property_name, value)
		
		print("  → Updated twin property: ", property_name, " = ", value)
		print("  → Twin state: ", motor_twin.to_string())

func _on_connection_failed(reason: String):
	print("MQTT Connection Failed: ", reason)

func _exit_tree():
	if mqtt_client:
		mqtt_client.disconnect_from_broker()
