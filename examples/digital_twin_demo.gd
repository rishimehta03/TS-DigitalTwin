extends Node

# Example script demonstrating Digital Twin module usage

func _ready():
	print("=== Digital Twin Module Demo ===")
	
	# Access the singleton TwinRegistry
	var registry = TwinRegistry
	print("TwinRegistry singleton available: ", registry != null)
	
	# Create a new Digital Twin for a motor
	var motor_twin = DigitalTwin.new()
	motor_twin.set_twin_id("MOTOR_001")
	motor_twin.set_twin_name("Primary Motor")
	motor_twin.set_twin_type("Motor")
	
	# Set some properties
	motor_twin.set_property("temperature", 75.5)
	motor_twin.set_property("rpm", 1800)
	motor_twin.set_property("voltage", 220)
	motor_twin.set_property("status", "running")
	
	# Register the twin
	registry.register_twin(motor_twin)
	print("Registered twin: ", motor_twin.to_string())
	
	# Create another twin for a sensor
	var sensor_twin = DigitalTwin.new()
	sensor_twin.set_twin_id("SENSOR_001")
	sensor_twin.set_twin_name("Temperature Sensor")
	sensor_twin.set_twin_type("Sensor")
	sensor_twin.set_property("current_reading", 23.8)
	sensor_twin.set_property("unit", "celsius")
	
	registry.register_twin(sensor_twin)
	print("Registered twin: ", sensor_twin.to_string())
	
	# Query the registry
	print("\nTotal twins in registry: ", registry.get_twin_count())
	print("All twin IDs: ", registry.get_twin_ids())
	
	# Get all motors
	var motors = registry.get_twins_by_type("Motor")
	print("Motor twins found: ", motors.size())
	
	# Connect to property change signal
	motor_twin.connect("property_changed", _on_motor_property_changed)
	
	# Update a property (will trigger signal)
	motor_twin.set_property("temperature", 82.3)
	
	# Get twin by ID
	var retrieved_twin = registry.get_twin("MOTOR_001")
	if retrieved_twin:
		print("\nRetrieved twin properties:")
		var props = retrieved_twin.get_all_properties()
		for key in props:
			print("  ", key, ": ", props[key])

func _on_motor_property_changed(property_name, new_value):
	print("Motor property changed: ", property_name, " = ", new_value)
