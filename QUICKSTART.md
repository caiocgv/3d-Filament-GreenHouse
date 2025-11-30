# Quick Start Guide

## 🚀 Getting Started in 5 Minutes

### Prerequisites
- ESP8266 board (NodeMCU or Wemos D1 Mini)
- LM35 temperature sensor
- 5V relay module
- Arduino IDE with ESP8266 support
- ESP8266 LittleFS Data Upload plugin

### Quick Setup

1. **Install Arduino IDE** (if not already installed)
   - Download from https://www.arduino.cc/en/software

2. **Add ESP8266 Board Support**
   ```
   File → Preferences → Additional Board Manager URLs:
   http://arduino.esp8266.com/stable/package_esp8266com_index.json
   ```

3. **Install LittleFS Upload Plugin**
   ```
   Download from: https://github.com/earlephilhower/arduino-esp8266littlefs-plugin/releases
   Extract to: Arduino/tools/ESP8266LittleFS/tool/
   Restart Arduino IDE
   ```

4. **Wire Up Your Hardware**
   ```
   ESP8266 → LM35
   - 3.3V or 5V → VCC (Pin 1 - left)
   - A0 → Output (Pin 2 - middle)
   - GND → GND (Pin 3 - right)

   ESP8266 → Relay
   - GPIO4 (D2) → Signal (Safe pin, no boot conflicts)
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

6. **Upload Filesystem (HTML)**
   - Connect ESP8266 to USB
   - Select board and port
   - Go to: Tools → ESP8266 LittleFS Data Upload
   - Wait for upload to complete

7. **Upload Code**
   - Keep ESP8266 connected
   - Select board: Tools → Board → NodeMCU 1.0
   - Select port: Tools → Port → [Your Port]
   - Click Upload ⬆️

8. **Find Your Device**
   - Open Serial Monitor (115200 baud)
   - Look for IP address (e.g., `192.168.1.100`)

9. **Access Web Interface**
   - Open browser
   - Go to `http://[IP_ADDRESS]`
   - Control your filament dryer! 🎉

## 📱 Using the Web Interface

- **Set Temperature**: Use the slider (0-70°C)
- **Set Timer**: Enter hours and minutes, click "Set Timer"
- **Monitor**: Watch real-time temperature
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
- Verify wiring (VCC, GND, Output to A0)
- Check LM35 orientation (flat side facing away)

**Web page won't load?**
- Verify IP address in Serial Monitor
- Check if filesystem was uploaded (LittleFS Data Upload)
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
