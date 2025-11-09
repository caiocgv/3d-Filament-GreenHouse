# Quick Start Guide

## 🚀 Getting Started in 5 Minutes

### Prerequisites
- ESP8266 board (NodeMCU or Wemos D1 Mini)
- DHT22 temperature/humidity sensor
- 5V relay module
- Arduino IDE with ESP8266 support

### Quick Setup

1. **Install Arduino IDE** (if not already installed)
   - Download from https://www.arduino.cc/en/software

2. **Add ESP8266 Board Support**
   ```
   File → Preferences → Additional Board Manager URLs:
   http://arduino.esp8266.com/stable/package_esp8266com_index.json
   ```

3. **Install Required Libraries**
   ```
   Sketch → Include Library → Manage Libraries
   Search and install:
   - "DHT sensor library" by Adafruit
   - "Adafruit Unified Sensor"
   ```

4. **Wire Up Your Hardware**
   ```
   ESP8266 → DHT22
   - 3.3V → VCC
   - GPIO2 (D4) → Data (with 10kΩ pull-up to 3.3V)
   - GND → GND

   ESP8266 → Relay
   - GPIO5 (D1) → Signal
   - 5V → VCC
   - GND → GND
   ```

5. **Configure WiFi**
   - Open `src/FilamentGreenhouse/FilamentGreenhouse.ino`
   - Change these lines:
   ```cpp
   const char* ssid = "YOUR_WIFI_SSID";
   const char* password = "YOUR_WIFI_PASSWORD";
   ```

6. **Upload Code**
   - Connect ESP8266 to USB
   - Select board: Tools → Board → NodeMCU 1.0
   - Select port: Tools → Port → [Your Port]
   - Click Upload ⬆️

7. **Find Your Device**
   - Open Serial Monitor (115200 baud)
   - Look for IP address (e.g., `192.168.1.100`)

8. **Access Web Interface**
   - Open browser
   - Go to `http://[IP_ADDRESS]`
   - Control your filament dryer! 🎉

## 📱 Using the Web Interface

- **Set Temperature**: Use the slider (0-70°C)
- **Set Timer**: Enter hours and minutes, click "Set Timer"
- **Monitor**: Watch real-time temperature and humidity
- **Power Control**: Use the ON/OFF button

## 🔧 Testing Individual Components

Before full assembly, test each component:

1. **WiFi Test**
   - Upload `examples/basic_test/wifi_test.ino`
   - Verify WiFi connection

2. **Sensor Test**
   - Upload `examples/basic_test/sensor_test.ino`
   - Verify temperature readings

3. **Relay Test**
   - Upload `examples/basic_test/relay_test.ino`
   - Verify relay clicking (WITHOUT heater connected)

## 📖 Full Documentation

- **Complete Guide**: See [README.md](README.md)
- **Hardware Details**: See [hardware/pcb/README.md](hardware/pcb/README.md)
- **3D Enclosure**: See [hardware/enclosure/README.md](hardware/enclosure/README.md)
- **Contributing**: See [CONTRIBUTING.md](CONTRIBUTING.md)

## ⚠️ Safety First!

- Never exceed 70°C for filaments
- Don't leave unattended for extended periods
- Use proper fuses and circuit protection
- If using mains voltage, consult an electrician

## 🐛 Common Issues

**WiFi won't connect?**
- Check SSID and password
- ESP8266 only supports 2.4GHz WiFi

**Sensor not reading?**
- Verify wiring
- Check 10kΩ pull-up resistor

**Web page won't load?**
- Verify IP address in Serial Monitor
- Ensure device is on same WiFi network

## 📞 Need Help?

- Open an issue on GitHub
- Check the [troubleshooting section](README.md#troubleshooting) in README

## 🎯 Recommended Drying Settings

| Filament | Temperature | Time |
|----------|-------------|------|
| PLA      | 45°C        | 4-6h |
| PETG     | 50°C        | 6-8h |
| ABS      | 60°C        | 4-6h |
| Nylon    | 70°C        | 12-16h |
| TPU      | 50°C        | 4-6h |

---

**Happy 3D Printing with Dry Filament! 🎨🖨️**
