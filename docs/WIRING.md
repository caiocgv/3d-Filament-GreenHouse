# Wiring Diagram

## Simple Text-Based Wiring Diagram

```
┌─────────────────────────────────────────────────────────────────────┐
│                    ESP8266 (NodeMCU / Wemos D1 Mini)                │
│                                                                       │
│  ┌──────────────────────────────────────────────────────────────┐   │
│  │  [USB Port]  5V  3.3V  GND  D1(GPIO5)  D4(GPIO2)  RST  EN   │   │
│  └──────────────────────────────────────────────────────────────┘   │
└───────────────┬────┬─────┬──────────┬──────────┬─────────────────────┘
                │    │     │          │          │
                │    │     │          │          │
        ┌───────┘    │     │          │          └──────────┐
        │            │     │          │                     │
        │            │     │          │                     │
    [5V Power]       │     │          │                   ┌─┴─────────┐
                     │     │          │                   │ 10kΩ      │
                     │     │          │                   │ Pull-up   │
                     │     │          │                   │ Resistor  │
                     │     │          │                   └─┬─────────┘
                     │     │          │                     │
        ┌────────────┘     │          └─────────────────────┼──────┐
        │                  │                                │      │
        │                  │                                │      │
   ┌────▼─────┐     ┌──────▼──────┐                  ┌─────▼──────▼───┐
   │  Relay   │     │   Common    │                  │     DHT22      │
   │  Module  │     │   Ground    │                  │ Temp/Humidity  │
   │          │     │   (GND)     │                  │     Sensor     │
   │  ┌───────┤     └──────┬──────┘                  │                │
   │  │VCC(5V)│            │                         │  ┌──────────┐  │
   │  │GND ───┼────────────┘                         │  │ Pin 1:VCC│  │
   │  │Signal │                                      │  │ Pin 2:DAT│  │
   │  └───────┤                                      │  │ Pin 3:NC │  │
   │  COM  NO │                                      │  │ Pin 4:GND│  │
   └─────┬──┬─┘                                      │  └──────────┘  │
         │  │                                        └────────────────┘
         │  │                                                 │
         │  │                                                 │
         │  └─────────────────────┐                          │
         │                        │                          │
    [Heater Power In]    [To Heater Element]       [Connected Above]
```

## Detailed Pin Connections

### ESP8266 Pins
```
NodeMCU Label │ GPIO Pin │ Connection          │ Notes
──────────────┼──────────┼─────────────────────┼────────────────────
D4            │ GPIO2    │ DHT22 Data Pin      │ With 10kΩ pull-up
D1            │ GPIO5    │ Relay Signal        │ Controls relay
3V3           │ 3.3V     │ DHT22 VCC, Pull-up  │ Power for sensor
5V            │ 5V       │ Relay VCC           │ Power for relay
GND           │ GND      │ Common Ground       │ All grounds connect
```

### DHT22 Sensor Pinout (Looking at front)
```
     ┌─────────────┐
     │   DHT22     │
     │  ┌───────┐  │
     │  │ ≡≡≡≡≡ │  │ ← Sensor grille
     │  └───────┘  │
     │             │
     └─┬───┬───┬───┘
       1   2   3   4
       │   │   │   │
       │   │   │   └── Pin 4: GND (Ground)
       │   │   └────── Pin 3: NC (Not Connected)
       │   └────────── Pin 2: DATA (to GPIO2 + 10kΩ to 3.3V)
       └────────────── Pin 1: VCC (to 3.3V)
```

### Relay Module Pinout
```
┌──────────────────┐
│   Relay Module   │
│                  │
│  [LED] [Relay]   │  ← Visual indicators
│                  │
│  VCC GND Signal  │  ← Control side
│                  │
│  COM  NO   NC    │  ← Switch side
└──────────────────┘

Control Side:
- VCC: Connect to 5V
- GND: Connect to GND
- Signal: Connect to GPIO5 (D1)

Switch Side:
- COM (Common): Connect to heater power input
- NO (Normally Open): Connect to heater positive
- NC (Normally Closed): Not used
```

## Wiring Steps

### Step 1: Power Connections
```
1. ESP8266 VCC (3.3V) → DHT22 Pin 1 (VCC)
2. ESP8266 5V → Relay VCC
3. ESP8266 GND → Common Ground Rail
4. DHT22 Pin 4 (GND) → Common Ground Rail
5. Relay GND → Common Ground Rail
```

### Step 2: Signal Connections
```
6. ESP8266 D4 (GPIO2) → DHT22 Pin 2 (DATA)
7. 10kΩ Resistor: Connect between DHT22 Pin 2 (DATA) and 3.3V
8. ESP8266 D1 (GPIO5) → Relay Signal Pin
```

### Step 3: Heater Connections (⚠️ SAFETY CRITICAL)
```
9. Heater Power Source (+) → Relay COM
10. Relay NO → Heater Element (+)
11. Heater Element (-) → Heater Power Source (-)

⚠️ WARNING: If using mains voltage (110V/220V):
   - Use proper insulation
   - Follow electrical codes
   - Have electrician review
   - Use appropriate fuses
   - Never work on live circuits
```

## Breadboard Layout (for prototyping)

```
         Power Rails
         +    -
         │    │
    ┌────┴────┴────────────────────────────────────┐
    │    │    │                                     │
    │    │    │        Breadboard                   │
    │  ┌─┴────┴──────────────────────────────┐     │
    │  │  +                              -   │     │
    │  │                                     │     │
    │  │  [ESP8266 Module]                  │     │
    │  │                                     │     │
    │  │  [10kΩ      [DHT22                 │     │
    │  │   Resistor]  Module]                │     │
    │  │                                     │     │
    │  │  Jumper Wires connecting:           │     │
    │  │  - Power (3.3V, 5V, GND)            │     │
    │  │  - Signal (GPIO2, GPIO5)            │     │
    │  │                                     │     │
    │  │  +                              -   │     │
    │  └─────────────────────────────────────┘     │
    │                                               │
    │  [Relay Module] ← Connected via jumpers      │
    │                                               │
    └───────────────────────────────────────────────┘
```

## Color Code Recommendations

When wiring, use these colors for easy identification:
- **Red**: 5V / 3.3V power
- **Black**: Ground (GND)
- **Yellow**: Signal/Data lines (GPIO2, GPIO5)
- **Orange**: Temperature sensor specific
- **White**: Neutral (if using AC)
- **Green**: Earth/Ground (if using AC)

## Testing Checklist

Before powering on with heater:

- [ ] All power connections correct (3.3V, 5V, GND)
- [ ] DHT22 has 10kΩ pull-up resistor
- [ ] GPIO2 connected to DHT22 Data
- [ ] GPIO5 connected to Relay Signal
- [ ] No short circuits between power rails
- [ ] Relay properly rated for heater voltage/current
- [ ] All connections are secure
- [ ] Heater circuit has proper fuse protection
- [ ] No exposed high-voltage connections

## Voltage Verification

Use multimeter to verify:
1. ESP8266 3.3V pin: Should read 3.2-3.4V
2. ESP8266 5V pin: Should read 4.8-5.2V
3. DHT22 VCC: Should read 3.2-3.4V
4. Relay VCC: Should read 4.8-5.2V
5. GPIO2 with pull-up: Should read ~3.3V
6. GPIO5 when HIGH: Should read ~3.3V
7. GPIO5 when LOW: Should read ~0V

## Common Mistakes to Avoid

❌ **DON'T:**
- Connect DHT22 directly to 5V (use 3.3V)
- Forget the pull-up resistor on DHT22 data line
- Connect relay signal before relay power
- Use GPIO15 without pull-down
- Use GPIO0 without considering boot mode
- Connect mains voltage without proper safety

✅ **DO:**
- Double-check all connections before powering on
- Use proper wire gauge for heater current
- Test with multimeter before applying power
- Keep low voltage and high voltage separated
- Use heat shrink tubing on connections
- Label all wires for future reference

## Additional Resources

- Full project details: See [README.md](../README.md)
- PCB design: See [hardware/pcb/README.md](../hardware/pcb/README.md)
- Component testing: See [examples/basic_test/](../examples/basic_test/)

---

⚠️ **SAFETY REMINDER**: Always prioritize safety. If you're uncertain about any electrical connections, especially those involving mains voltage, consult a qualified electrician.
