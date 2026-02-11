#!/usr/bin/env python3
"""
IoT Device Simulator - Publishes sensor data to MQTT broker
Simulates a motor with temperature, RPM, and voltage sensors
"""

import time
import random
import json
try:
    import paho.mqtt.client as mqtt
except ImportError:
    print("ERROR: paho-mqtt not installed!")
    print("Install with: pip install paho-mqtt")
    exit(1)

# MQTT Configuration
BROKER = "localhost"  # or "127.0.0.1"
PORT = 1883
CLIENT_ID = "iot_simulator"

# Device Configuration
DEVICE_ID = "motor001"
BASE_TOPIC = f"tdtstack/demo/{DEVICE_ID}"

# Sensor simulation parameters
temperature = 70.0  # Starting temperature
rpm = 1500         # Starting RPM
voltage = 220.0    # Starting voltage

def on_connect(client, userdata, flags, rc):
    if rc == 0:
        print(f"✅ Connected to MQTT broker at {BROKER}:{PORT}")
        print(f"📡 Publishing to topics: {BASE_TOPIC}/*")
        print("Press Ctrl+C to stop\n")
    else:
        print(f"❌ Connection failed with code {rc}")

def on_publish(client, userdata, mid):
    pass  # Message published successfully

def simulate_sensor_readings():
    """Simulate realistic sensor behavior with random variations"""
    global temperature, rpm, voltage
    
    # Temperature varies slowly (±0.5°C per update)
    temperature += random.uniform(-0.5, 1.0)
    temperature = max(65.0, min(95.0, temperature))  # Clamp between 65-95°C
    
    # RPM varies (±50 RPM)
    rpm += random.randint(-50, 50)
    rpm = max(1200, min(2000, rpm))  # Clamp between 1200-2000 RPM
    
    # Voltage is mostly stable (±2V)
    voltage += random.uniform(-2, 2)
    voltage = max(210, min(230, voltage))  # Clamp between 210-230V
    
    return {
        "temperature": round(temperature, 1),
        "rpm": rpm,
        "voltage": round(voltage, 1)
    }

def main():
    # Create MQTT client
    client = mqtt.Client(CLIENT_ID)
    client.on_connect = on_connect
    client.on_publish = on_publish
    
    try:
        # Connect to broker
        print(f"🔌 Connecting to MQTT broker at {BROKER}:{PORT}...")
        client.connect(BROKER, PORT, keepalive=60)
        client.loop_start()
        
        # Give connection time to establish
        time.sleep(1)
        
        # Main loop - publish sensor data every 2 seconds
        iteration = 0
        while True:
            iteration += 1
            readings = simulate_sensor_readings()
            
            # Publish individual topics
            client.publish(f"{BASE_TOPIC}/temperature", str(readings["temperature"]))
            client.publish(f"{BASE_TOPIC}/rpm", str(readings["rpm"]))
            client.publish(f"{BASE_TOPIC}/voltage", str(readings["voltage"]))
            
            # Also publish combined JSON payload
            json_payload = json.dumps(readings)
            client.publish(f"{BASE_TOPIC}/status", json_payload)
            
            # Print status
            print(f"[{iteration:04d}] 🌡️  {readings['temperature']}°C  |  "
                  f"⚙️  {readings['rpm']} RPM  |  ⚡ {readings['voltage']}V")
            
            time.sleep(2)  # Publish every 2 seconds
            
    except KeyboardInterrupt:
        print("\n\n⏹️  Stopping simulator...")
    except Exception as e:
        print(f"\n❌ Error: {e}")
    finally:
        client.loop_stop()
        client.disconnect()
        print("👋 Disconnected from broker")

if __name__ == "__main__":
    print("=" * 60)
    print("IoT DEVICE SIMULATOR - Motor Sensor")
    print("=" * 60)
    main()
