# Setting Up Local MQTT Broker (Mosquitto)

## Step 1: Download Mosquitto

1. **Download** from official site: [https://mosquitto.org/download/](https://mosquitto.org/download/)
2. Choose **Windows 64-bit installer** (mosquitto-X.X.X-install-windows-x64.exe)
3. **Run the installer** as Administrator

## Step 2: Install Mosquitto

1. Accept the license agreement
2. Keep default installation path: `C:\Program Files\mosquitto\`
3. Select components:
   - ✅ Service
   - ✅ Clients (mosquitto_pub, mosquitto_sub)
   - ✅ Dependencies
4. Click **Install**
5. Click **Finish**

## Step 3: Configure Mosquitto

Create/edit the config file to allow anonymous connections for testing:

**File:** `C:\Program Files\mosquitto\mosquitto.conf`

Add these lines at the end:
```conf
listener 1883
allow_anonymous true
```

## Step 4: Start Mosquitto Service

**Option A: As Windows Service (Recommended)**
```powershell
# Open PowerShell as Administrator
Start-Service mosquitto

# Check status
Get-Service mosquitto

# Set to auto-start on boot (optional)
Set-Service -Name mosquitto -StartupType Automatic
```

**Option B: Manual Start (for debugging)**
```powershell
cd "C:\Program Files\mosquitto"
.\mosquitto.exe -v -c mosquitto.conf
```

## Step 5: Test the Broker

**Open a new PowerShell window and test subscribe:**
```powershell
cd "C:\Program Files\mosquitto"
.\mosquitto_sub.exe -h localhost -t "test/topic" -v
```

**Open another PowerShell and publish:**
```powershell
cd "C:\Program Files\mosquitto"
.\mosquitto_pub.exe -h localhost -t "test/topic" -m "Hello MQTT!"
```

You should see "Hello MQTT!" in the subscriber window!

## Step 6: Test with TDT Stack

Update your `mqtt_twin_demo.gd` script:

```gdscript
# Change from public broker to localhost
mqtt_client.connect_to_broker("127.0.0.1", 1883, "tdtstack_demo")
# or
mqtt_client.connect_to_broker("localhost", 1883, "tdtstack_demo")
```

---

## Troubleshooting

**Service won't start:**
```powershell
# Check if port 1883 is in use
netstat -ano | findstr :1883

# View service logs
Get-EventLog -LogName Application -Source mosquitto -Newest 10
```

**Permission errors:**
- Run PowerShell as Administrator
- Check firewall settings (allow port 1883)

**Can't find mosquitto.exe:**
- Add to PATH: `C:\Program Files\mosquitto`
- Or use full paths in commands

---

## Next Steps After Installation

1. ✅ Start Mosquitto service
2. ✅ Test with mosquitto_pub/sub
3. ✅ Update TDT Stack demo to use localhost
4. ✅ Run the scene and test!
5. 🚀 Create IoT device simulator script
