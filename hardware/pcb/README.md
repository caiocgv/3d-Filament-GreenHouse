# PCB Design Files

This directory contains the PCB (Printed Circuit Board) design files for the Filament Greenhouse Dryer controller.

## Contents

### Gerber Files
The `gerber/` directory will contain manufacturing files for PCB production:
- **FilamentDryer-F_Cu.gbr** - Front copper layer
- **FilamentDryer-B_Cu.gbr** - Back copper layer
- **FilamentDryer-F_Mask.gbr** - Front solder mask
- **FilamentDryer-B_Mask.gbr** - Back solder mask
- **FilamentDryer-F_SilkS.gbr** - Front silkscreen
- **FilamentDryer-B_SilkS.gbr** - Back silkscreen
- **FilamentDryer-Edge_Cuts.gbr** - Board outline
- **FilamentDryer-drill.drl** - Drill file

### Other Files (To Be Added)
- **schematic.pdf** - Circuit schematic diagram
- **bom.csv** - Bill of Materials (component list)
- **assembly_diagram.pdf** - Component placement diagram

## PCB Specifications

### Recommended Specifications for Manufacturing

- **Board Dimensions**: 80mm x 60mm (approximately)
- **Layers**: 2-layer PCB
- **Thickness**: 1.6mm
- **Copper Weight**: 1 oz (35μm)
- **Surface Finish**: HASL (Hot Air Solder Leveling) or ENIG
- **Solder Mask Color**: Green (or your preference)
- **Silkscreen Color**: White

### Design Features

The PCB design includes:
- **ESP8266 Module Socket**: For easy replacement and programming
- **DHT22 Sensor Connector**: 3-pin header for sensor connection
- **Relay Terminal Block**: Screw terminals for heater connection
- **Power Input**: Barrel jack for 5V power supply
- **Voltage Regulation**: 3.3V regulator for ESP8266 and sensor
- **Pull-up Resistors**: Pre-installed for DHT22 data line
- **LED Indicators**: Power, WiFi status, and heater status LEDs
- **Reset Button**: Manual reset button for ESP8266
- **Flash Button**: Programming mode button

## Circuit Description

### Main Components

1. **ESP8266 Module (ESP-12E/F or NodeMCU)**
   - Main controller
   - WiFi connectivity
   - Pins: GPIO2 (DHT22), GPIO5 (Relay)

2. **DHT22 Temperature/Humidity Sensor**
   - Connected to GPIO2 (D4)
   - 10kΩ pull-up resistor to 3.3V
   - 3-pin connection: VCC, Data, GND

3. **Relay Module Interface**
   - Connected to GPIO5 (D1)
   - Can drive external relay module or on-board relay
   - Flyback diode protection if driving relay coil directly

4. **Power Supply Section**
   - 5V input from external adapter
   - AMS1117-3.3 regulator for 3.3V rail
   - Filtering capacitors for stable operation

5. **Protection Circuits**
   - Reverse polarity protection
   - Overvoltage protection
   - ESD protection on sensor inputs

### Connections

```
ESP8266 Connections:
├── 3.3V      → DHT22 VCC, Pull-up resistor
├── GND       → Common ground
├── GPIO2(D4) → DHT22 Data (with 10kΩ pull-up)
├── GPIO5(D1) → Relay signal
├── EN        → 10kΩ pull-up to 3.3V
├── GPIO0     → Flash button (pull-down to program)
├── GPIO15    → 10kΩ pull-down
└── RST       → Reset button + 10kΩ pull-up

Power Supply:
├── 5V IN     → Barrel jack positive
├── GND       → Barrel jack negative
├── 5V        → Relay VCC, AMS1117 input
└── 3.3V      → ESP8266 VCC, DHT22 VCC

Relay Interface:
├── Signal    → From ESP8266 GPIO5
├── VCC       → 5V
├── GND       → Common ground
├── COM       → Heater power input
└── NO        → Heater positive output
```

## Manufacturing

### Ordering PCBs

You can order PCBs from various manufacturers:

1. **Upload Gerber Files**: Use the gerber/ folder contents
2. **Select Specifications**: Use the recommended specs above
3. **Choose Quantity**: Minimum order is usually 5 pieces
4. **Review and Order**: Double-check board preview before ordering

### Popular PCB Manufacturers

- **JLCPCB** - jlcpcb.com (Low cost, fast shipping)
- **PCBWay** - pcbway.com (Good quality, multiple options)
- **OSH Park** - oshpark.com (USA-based, high quality)
- **Seeed Studio** - seeedstudio.com (Good for prototyping)
- **Eurocircuits** - eurocircuits.com (Europe-based)

### Assembly

#### Required Tools
- Soldering iron (temperature controlled, 350°C)
- Solder (63/37 or lead-free)
- Flux
- Tweezers
- Wire cutters
- Multimeter

#### Assembly Steps

1. **Solder SMD Components First** (if any):
   - Start with smallest components
   - Use flux liberally
   - Check for bridges

2. **Solder Through-Hole Components**:
   - Install voltage regulator
   - Install capacitors
   - Install resistors
   - Install connectors

3. **Install Headers**:
   - ESP8266 socket
   - DHT22 connector
   - Programming header (if included)

4. **Install Terminal Blocks**:
   - Relay connection terminals
   - Power input terminals

5. **Install Indicators**:
   - LED indicators with current-limiting resistors
   - Power LED, Status LED

6. **Quality Check**:
   - Visual inspection for solder bridges
   - Continuity test with multimeter
   - Check power rails for shorts
   - Verify voltage levels (5V and 3.3V)

### Testing

Before connecting the heater:

1. **Power Test**:
   - Apply 5V power
   - Verify 3.3V rail voltage
   - Check for excessive heating

2. **ESP8266 Test**:
   - Install ESP8266 module
   - Upload blink sketch
   - Verify LED operation

3. **Sensor Test**:
   - Connect DHT22
   - Upload sensor test sketch
   - Verify readings in serial monitor

4. **Relay Test**:
   - Upload relay test sketch
   - Verify relay clicking
   - Check voltage at relay terminals (with heater disconnected)

## Safety Notes

⚠️ **Important Safety Information**

- Never work on the board while powered
- Double-check polarity before applying power
- Use appropriate fuses in heater circuit
- Ensure proper insulation for high-voltage sections
- Follow local electrical codes for mains voltage
- If using mains-powered heater, have a qualified electrician review your setup

## Support Files

### Bill of Materials (BOM)

See `bom.csv` for complete component list with:
- Part numbers
- Manufacturers
- Quantities
- Specifications
- Supplier links

### Schematic

See `schematic.pdf` for detailed circuit diagram showing:
- All component connections
- Pin assignments
- Voltage levels
- Component values

## Customization

You can customize the PCB design for:
- Different ESP8266 modules (ESP-01, ESP-12, NodeMCU, etc.)
- Alternative sensors (DHT11, SHT31, BME280)
- On-board relay vs. external relay module
- Additional features (OLED display, buttons, etc.)
- Different power input options (USB, battery, etc.)

## Troubleshooting

### Common Issues

**ESP8266 Won't Boot**:
- Check 3.3V voltage (should be 3.2-3.4V)
- Verify EN and GPIO15 pull-up/down resistors
- Check for shorts on power rails

**Sensor Not Reading**:
- Verify DHT22 connections
- Check pull-up resistor (10kΩ)
- Try different GPIO pin

**Relay Not Switching**:
- Verify GPIO5 voltage when high (should be 3.3V)
- Check relay signal LED
- Test with multimeter

**Overheating**:
- Check voltage regulator heat
- Verify proper current capacity
- Add heatsink if needed

## Contributing

If you improve the PCB design, please submit a pull request with:
- Updated gerber files
- Updated schematic PDF
- Updated BOM
- Description of changes

## License

Hardware design files are released under CERN Open Hardware License v2.
