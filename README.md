# 3D Filament Greenhouse Dryer

An ESP8266-based smart filament dryer designed to keep your 3D printing filaments dry and ready for use. This project combines temperature control, humidity monitoring, and a user-friendly web interface to create the perfect environment for storing and drying hygroscopic filaments like PLA, PETG, Nylon, and TPU.

## Table of Contents
- [Overview](#overview)
- [Features](#features)
- [How It Works](#how-it-works)
- [Hardware Requirements](#hardware-requirements)
- [Software Requirements](#software-requirements)
- [Hardware Assembly](#hardware-assembly)
- [Installation](#installation)
- [Web Interface](#web-interface)
- [Usage](#usage)
- [API Endpoints](#api-endpoints)
- [Folder Structure](#folder-structure)
- [Troubleshooting](#troubleshooting)
- [Contributing](#contributing)
- [License](#license)

## Overview

3D printer filaments are hygroscopic, meaning they absorb moisture from the air. This moisture can cause printing issues like:
- Poor layer adhesion
- Stringing and oozing
- Bubbling and popping during extrusion
- Reduced print quality and strength

This Filament Greenhouse Dryer solves these problems by maintaining a controlled, low-humidity environment with optional heating to actively dry filaments before and during storage.

## Features

- **Real-time Monitoring**: View current temperature and humidity levels
- **Adjustable Temperature Control**: Set target temperature (up to 70°C recommended for most filaments)
- **Timer Function**: Set drying duration (1 minute to 24 hours)
- **Web Portal Interface**: Control everything from your smartphone, tablet, or computer
- **WiFi Connectivity**: Connect to your home network for easy access
- **Automatic Shutoff**: System turns off automatically when timer expires
- **Manual Control**: Turn the heater on/off at any time
- **Temperature Safety**: Prevents overheating with built-in limits
- **Responsive Design**: Web interface works on all devices

## How It Works

### System Architecture

The Filament Greenhouse Dryer uses an ESP8266 microcontroller as its brain, coordinating several components:

1. **ESP8266 WiFi Module**: 
   - Hosts a web server accessible via your local network
   - Processes API requests from the web interface
   - Controls the heating element via relay
   - Reads sensor data and manages the timer

2. **DHT22 Temperature/Humidity Sensor**:
   - Measures ambient temperature (accurate to ±0.5°C)
   - Monitors relative humidity (accurate to ±2%)
   - Provides data every 2 seconds to the ESP8266

3. **Relay Module**:
   - Switches the heating element on/off based on temperature readings
   - Electrically isolates the low-voltage control circuit from high-voltage heater
   - LED indicator shows heater status

4. **Heating Element**:
   - Low-wattage heater (typically 40-60W) safely raises temperature
   - Distributes heat evenly throughout the enclosure
   - Works in conjunction with the relay for temperature control

### Control Logic

The system implements a simple but effective control algorithm:

**Temperature Control (PID-like behavior)**:
```
IF current_temp < (target_temp - 2°C):
    Turn heater ON
ELSE IF current_temp > target_temp:
    Turn heater OFF
ELSE:
    Maintain current state (hysteresis)
```

**Timer Management**:
- User sets desired drying duration
- Timer counts down while system is active
- System automatically shuts off when timer reaches zero
- Timer can be adjusted or canceled at any time

**Safety Features**:
- Maximum temperature limit (75°C hard coded)
- Automatic shutoff if sensor fails
- WiFi connection timeout handling
- Watchdog timer to prevent system hangs

### Web Interface Communication

The web portal uses AJAX to communicate with the ESP8266:

1. Browser loads HTML/CSS/JavaScript interface
2. JavaScript polls the ESP8266 every 2 seconds for status updates
3. User interactions (button clicks, slider changes) send API requests
4. ESP8266 responds with JSON data containing current state
5. Interface updates in real-time without page reload

## Hardware Requirements

### Electronic Components

| Component | Specification | Quantity | Notes |
|-----------|--------------|----------|-------|
| ESP8266 | NodeMCU or Wemos D1 Mini | 1 | WiFi-enabled microcontroller |
| DHT22 Sensor | Temperature/Humidity sensor | 1 | DHT11 works but less accurate |
| Relay Module | 5V, 10A rated | 1 | Single channel, active low/high |
| Heating Element | 40-60W, 12V or mains voltage | 1 | Silicone heater pad recommended |
| Power Supply | 5V 2A (for ESP8266) | 1 | USB power adapter works |
| Power Supply | 12V or mains (for heater) | 1 | Depends on heater choice |
| Resistor | 10kΩ | 1 | Pull-up for DHT22 data line |
| Jumper Wires | - | Several | For connections |
| PCB (Optional) | Custom designed | 1 | See hardware/pcb folder |

### Enclosure Materials

- Clear acrylic or plastic container (food storage container works well)
- Silicone sealant (for making it airtight)
- Desiccant packets (optional, for passive moisture absorption)
- 3D printed mounting brackets (see hardware/enclosure folder)
- Small fan (optional, for air circulation)

## Software Requirements

- **Arduino IDE** (version 1.8.19 or later) or **PlatformIO**
- **ESP8266 Board Package** for Arduino
- Required Libraries:
  - ESP8266WiFi (included with board package)
  - ESP8266WebServer (included with board package)
  - DHT sensor library (by Adafruit)
  - Adafruit Unified Sensor library

## Hardware Assembly

### Wiring Diagram

```
ESP8266 (NodeMCU/Wemos D1 Mini)
├── D4 (GPIO2)  → DHT22 Data Pin
├── 3.3V        → DHT22 VCC
├── GND         → DHT22 GND
├── D1 (GPIO5)  → Relay Signal Pin
├── 5V          → Relay VCC
└── GND         → Relay GND

DHT22 Sensor
├── Pin 1 (VCC) → 3.3V
├── Pin 2 (Data)→ D4 + 10kΩ pull-up to 3.3V
├── Pin 3 (NC)  → Not connected
└── Pin 4 (GND) → GND

Relay Module
├── VCC         → 5V
├── GND         → GND
├── IN          → D1 (GPIO5)
├── COM         → Heater Power In
├── NO (Normally Open) → Heater Positive
└── Heater Negative → Power Supply Negative
```

### Assembly Steps

1. **Prepare the Enclosure**:
   - Drill holes for sensor placement (outside to measure ambient)
   - Mount the heater pad on the bottom or side
   - Create cable entry points with cable glands

2. **Solder Connections** (if using custom PCB):
   - Refer to hardware/pcb/schematic.pdf
   - Solder components as per PCB layout
   - Double-check polarities

3. **Wire Components** (if using breadboard):
   - Follow wiring diagram above
   - Use heat shrink tubing on exposed connections
   - Secure loose wires with zip ties

4. **Mount Electronics**:
   - Use 3D printed brackets from hardware/enclosure
   - Keep ESP8266 accessible for USB programming
   - Position relay away from heat source

5. **Install Sensor**:
   - Mount DHT22 sensor to measure internal temperature
   - Ensure good airflow around sensor
   - Avoid direct contact with heating element

6. **Safety Check**:
   - Verify all connections match diagram
   - Check for short circuits with multimeter
   - Ensure heater is properly insulated
   - Test relay switching before connecting mains voltage

## Installation

### Step 1: Install Arduino IDE and Board Support

1. Download and install [Arduino IDE](https://www.arduino.cc/en/software)
2. Open Arduino IDE
3. Go to **File → Preferences**
4. Add to "Additional Board Manager URLs":
   ```
   http://arduino.esp8266.com/stable/package_esp8266com_index.json
   ```
5. Go to **Tools → Board → Boards Manager**
6. Search for "esp8266" and install "ESP8266 by ESP8266 Community"

### Step 2: Install Required Libraries

1. Go to **Sketch → Include Library → Manage Libraries**
2. Install the following:
   - "DHT sensor library" by Adafruit
   - "Adafruit Unified Sensor" by Adafruit

### Step 3: Configure WiFi Credentials

1. Open `src/FilamentGreenhouse/FilamentGreenhouse.ino`
2. Modify these lines with your WiFi details:
   ```cpp
   const char* ssid = "YOUR_WIFI_SSID";
   const char* password = "YOUR_WIFI_PASSWORD";
   ```

### Step 4: Upload Code

1. Connect ESP8266 to computer via USB
2. Select board: **Tools → Board → ESP8266 Boards → NodeMCU 1.0** (or your specific board)
3. Select port: **Tools → Port → [Your COM/USB port]**
4. Click **Upload** button
5. Wait for upload to complete

### Step 5: Access Web Interface

1. Open Serial Monitor (**Tools → Serial Monitor**) at 115200 baud
2. Press reset button on ESP8266
3. Note the IP address displayed (e.g., 192.168.1.100)
4. Open web browser and navigate to `http://[IP_ADDRESS]`
5. You should see the Filament Greenhouse control panel

## Web Interface

The web portal provides an intuitive interface with the following features:

### Main Dashboard

- **Temperature Display**: Large, easy-to-read current temperature
- **Humidity Display**: Current humidity percentage
- **Target Temperature**: Slider to set desired temperature (0-70°C)
- **Timer Control**: Set drying duration in hours and minutes
- **Status Indicator**: Shows if heater is currently ON or OFF
- **Manual Override**: Emergency stop or manual start button

### Controls

1. **Temperature Slider**: 
   - Drag to set target temperature
   - Real-time update as you drag
   - Displays current selection

2. **Timer Input**:
   - Separate inputs for hours and minutes
   - Set button applies the timer
   - Countdown display shows remaining time

3. **Power Button**:
   - Toggle system on/off
   - Immediate effect
   - Visual feedback

### Auto-Refresh

- Dashboard updates every 2 seconds automatically
- No need to refresh page manually
- Always displays current data

## Usage

### Basic Operation

1. **Power On**: 
   - Ensure ESP8266 and heater are powered
   - Wait for WiFi connection (LED on ESP8266 will stop blinking)

2. **Access Interface**:
   - Open web browser on any device connected to same WiFi
   - Navigate to the ESP8266's IP address

3. **Set Temperature**:
   - Use slider to select target temperature
   - Recommended: 45°C for PLA, 50°C for PETG, 60°C for Nylon

4. **Set Timer**:
   - Enter desired hours and minutes
   - Click "Set Timer" button
   - System starts countdown automatically

5. **Monitor Progress**:
   - Watch temperature rise to target
   - Check remaining time
   - Heater indicator shows active heating

6. **Completion**:
   - System automatically turns off when timer reaches zero
   - Notification displayed on interface
   - Heater relay disengages

### Recommended Drying Times

| Filament Type | Temperature | Duration |
|--------------|-------------|----------|
| PLA | 45°C | 4-6 hours |
| PETG | 50°C | 6-8 hours |
| ABS | 60°C | 4-6 hours |
| Nylon | 70°C | 12-16 hours |
| TPU/TPE | 50°C | 4-6 hours |

### Tips for Best Results

- Start with clean, dry filament for baseline
- Don't exceed recommended temperatures for each filament type
- Use desiccant packets in enclosure for passive moisture control
- Keep enclosure sealed during drying process
- Allow filament to cool before removing (prevents immediate re-absorption)

## API Endpoints

The ESP8266 web server exposes several API endpoints for programmatic control:

### GET /status

Returns current system status as JSON.

**Response:**
```json
{
  "temperature": 23.5,
  "humidity": 45.2,
  "targetTemp": 50,
  "heaterOn": true,
  "timerRunning": true,
  "remainingTime": 14400
}
```

### POST /setTemp

Sets target temperature.

**Request Body:**
```json
{
  "temp": 50
}
```

**Response:**
```json
{
  "success": true,
  "targetTemp": 50
}
```

### POST /setTimer

Sets timer duration in seconds.

**Request Body:**
```json
{
  "seconds": 14400
}
```

**Response:**
```json
{
  "success": true,
  "timerSet": 14400
}
```

### POST /power

Turns system on or off.

**Request Body:**
```json
{
  "on": true
}
```

**Response:**
```json
{
  "success": true,
  "systemOn": true
}
```

## Folder Structure

```
3d-Filament-GreenHouse/
├── README.md                          # This file
├── src/
│   └── FilamentGreenhouse/
│       ├── FilamentGreenhouse.ino     # Main Arduino sketch
│       ├── WebServer.h                # Web server functionality
│       ├── TemperatureControl.h       # Temperature control logic
│       └── WebInterface.h             # HTML/CSS/JS for web portal
├── hardware/
│   ├── pcb/
│   │   ├── gerber/                    # Gerber files for PCB manufacturing
│   │   │   ├── FilamentDryer-F_Cu.gbr
│   │   │   ├── FilamentDryer-B_Cu.gbr
│   │   │   ├── FilamentDryer-F_Mask.gbr
│   │   │   ├── FilamentDryer-B_Mask.gbr
│   │   │   ├── FilamentDryer-F_SilkS.gbr
│   │   │   ├── FilamentDryer-B_SilkS.gbr
│   │   │   ├── FilamentDryer-Edge_Cuts.gbr
│   │   │   └── FilamentDryer-drill.drl
│   │   ├── schematic.pdf              # Circuit schematic
│   │   ├── bom.csv                    # Bill of materials
│   │   └── README.md                  # PCB assembly instructions
│   └── enclosure/
│       ├── base.stl                   # 3D printable base
│       ├── lid.stl                    # 3D printable lid
│       ├── sensor_mount.stl           # DHT22 sensor bracket
│       ├── esp8266_mount.stl          # ESP8266 mounting bracket
│       └── README.md                  # Printing instructions
├── docs/
│   ├── images/                        # Photos and diagrams
│   │   ├── wiring_diagram.png
│   │   ├── assembled_unit.jpg
│   │   └── web_interface.png
│   └── datasheets/                    # Component datasheets
│       ├── ESP8266_datasheet.pdf
│       ├── DHT22_datasheet.pdf
│       └── relay_module_datasheet.pdf
└── examples/
    ├── basic_test/                    # Simple test sketches
    │   ├── wifi_test.ino
    │   ├── sensor_test.ino
    │   └── relay_test.ino
    └── advanced/
        └── mqtt_integration.ino       # MQTT for home automation
```

## Troubleshooting

### ESP8266 Won't Connect to WiFi

- **Check credentials**: Verify SSID and password are correct
- **Signal strength**: Move closer to router or use WiFi extender
- **2.4GHz network**: ESP8266 doesn't support 5GHz WiFi
- **Reset**: Try resetting ESP8266 and router
- **Serial monitor**: Check for error messages at 115200 baud

### Web Page Won't Load

- **IP address**: Verify correct IP from serial monitor
- **Same network**: Ensure device is on same WiFi network as ESP8266
- **Firewall**: Check if firewall is blocking port 80
- **Restart ESP8266**: Power cycle the device

### Temperature Not Changing

- **Sensor connection**: Check DHT22 wiring and pull-up resistor
- **Sensor failure**: Try reading sensor values in serial monitor
- **Heater power**: Verify heater is receiving power through relay
- **Relay clicking**: Listen for relay activation sound
- **Target temperature**: Ensure target is higher than ambient

### Heater Stays On Constantly

- **Target too high**: Lower the target temperature
- **Sensor placement**: Ensure sensor measures correct location
- **Code issue**: Check temperature control logic in code
- **Relay stuck**: Test relay manually or replace

### Timer Not Working

- **Time sync**: ESP8266 loses time on power cycle (expected)
- **Set timer**: Make sure you clicked "Set Timer" button
- **Check remaining time**: View in web interface
- **Code logic**: Verify timer countdown in serial monitor

### Overheating

- **Lower temperature**: Reduce target temperature immediately
- **Poor ventilation**: Ensure some airflow in enclosure
- **Heater too powerful**: Use lower wattage heating element
- **Safety first**: Unplug if temperature exceeds 80°C

## Contributing

Contributions are welcome! Please feel free to submit pull requests or open issues for:

- Bug fixes
- Feature enhancements
- Documentation improvements
- Hardware design variations
- PCB improvements
- Enclosure designs

### Development Guidelines

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/AmazingFeature`)
3. Commit your changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

## License

This project is open source and available under the MIT License. See LICENSE file for details.

## Acknowledgments

- ESP8266 Community for excellent board support
- Adafruit for sensor libraries
- 3D printing community for inspiration and feedback

## Safety Warning

⚠️ **IMPORTANT SAFETY INFORMATION**

- This project involves both low-voltage (5V/3.3V) and potentially high-voltage (mains) circuits
- Always disconnect power before making any changes to wiring
- If using mains-powered heater, ensure proper electrical isolation and safety
- Never exceed recommended temperature limits for your filaments
- Don't leave the device unattended for extended periods
- Ensure adequate ventilation to prevent fire hazards
- Use appropriate fuses and circuit breakers
- If you're not comfortable working with electricity, seek help from a qualified person

---

**Questions or Issues?**

Open an issue on GitHub or contact the maintainers. Happy printing with dry filament! 🎨🖨️