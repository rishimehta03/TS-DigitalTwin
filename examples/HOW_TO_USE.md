# How to Use the Digital Twin Module

## Overview
The Digital Twin module is **built into the TDT Stack engine**. It's not a separate panel or plugin - it's accessible through **GDScript** in any project you create.

## Quick Start

### 1. Create a New Project
- Open TDT Stack editor (`tdtstack.windows.editor.x86_64.exe`)
- Create a new project

### 2. Create a Test Scene
1. In your project, create a new scene
2. Add a **Node** as the root (Scene → New Scene → Other Node → Node)
3. Save it as `test_digital_twin.tscn`

### 3. Attach the Demo Script
1. With the Node selected, click "Attach Script"
2. Copy the contents from `examples/digital_twin_demo.gd`
3. Or write your own script (see example below)

### 4. Run the Scene
- Press F6 (or Run Current Scene button)
- Check the **Output** panel at the bottom to see results!

---

## Example Usage

```gdscript
extends Node

func _ready():
    # Access the TwinRegistry singleton
    var registry = TwinRegistry
    
    # Create a digital twin
    var motor = DigitalTwin.new()
    motor.set_twin_id("MOTOR_001")
    motor.set_twin_name("Primary Motor")
    motor.set_twin_type("Motor")
    
    # Set properties
    motor.set_property("temperature", 75.5)
    motor.set_property("rpm", 1800)
    motor.set_property("status", "running")
    
    # Register with the singleton
    registry.register_twin(motor)
    
    # Listen for changes
    motor.connect("property_changed", func(prop, value):
        print("Property changed: ", prop, " = ", value)
    )
    
    # Update properties (triggers signal)
    motor.set_property("temperature", 82.3)
    
    # Query registry
    print("Total twins: ", registry.get_twin_count())
    print("All twins: ", registry.get_all_twins())
```

---

## Classes Available

### `DigitalTwin` - Represents a digital twin instance
**Methods:**
- `set_twin_id(String)` / `get_twin_id()` - Unique ID
- `set_twin_name(String)` / `get_twin_name()` - Human-readable name
- `set_twin_type(String)` / `get_twin_type()` - Type (Motor, Sensor, etc.)
- `set_property(String key, Variant value)` - Set a property
- `get_property(String key, Variant default)` - Get a property
- `has_property(String key)` - Check if property exists
- `get_all_properties()` - Returns Dictionary of all properties
- `to_dict()` / `from_dict(Dictionary)` - State serialization

**Signals:**
- `property_changed(property_name: String, new_value: Variant)`
- `state_changed()`
- `synchronization_changed(is_synchronized: bool)`

### `TwinRegistry` - Singleton for managing all twins
**Methods:**
- `register_twin(DigitalTwin)` - Add a twin to registry
- `unregister_twin(String twin_id)` - Remove a twin
- `get_twin(String twin_id)` - Retrieve a twin by ID
- `has_twin(String twin_id)` - Check if twin exists
- `get_all_twins()` - Get array of all twins
- `get_twins_by_type(String type)` - Filter by type
- `get_twin_count()` - Total number of twins
- `get_twin_ids()` - Array of all IDs

**Signals:**
- `twin_registered(twin: DigitalTwin)`
- `twin_unregistered(twin_id: String)`

---

## Next Steps

Once you're comfortable with the basics:
- **Milestone 1B**: Add MQTT protocol support for real IoT device integration
- **Milestone 1C**: Create 3D visualization and dashboard UI
- **Milestone 1D**: Add physics-based simulation

Happy twinning! 🚀
