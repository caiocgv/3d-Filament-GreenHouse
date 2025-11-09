# 3D Printable Enclosure Files

This directory contains 3D printable files for the Filament Greenhouse Dryer enclosure.

## Overview

The enclosure is designed to house the ESP8266 controller, DHT22 sensor, relay module, and provide mounting points for the heating element. It's optimized for FDM 3D printing and designed to be printed without supports where possible.

## Files

### STL Files (To Be Added)

- **base.stl** - Main enclosure base with mounting points
- **lid.stl** - Enclosure lid with ventilation
- **sensor_mount.stl** - DHT22 sensor mounting bracket
- **esp8266_mount.stl** - ESP8266 controller mounting bracket
- **relay_mount.stl** - Relay module mounting bracket
- **cable_gland.stl** - Cable entry gland/bushing
- **feet.stl** - Non-slip feet for base (print 4x)

### Additional Design Files

- **enclosure.step** - STEP file for CAD modification
- **enclosure.f3d** - Fusion 360 source file (parametric)

## Print Settings

### Recommended Settings

| Setting | Value | Notes |
|---------|-------|-------|
| **Material** | PETG or ABS | Heat resistant |
| **Layer Height** | 0.2mm | Good balance of speed/quality |
| **Infill** | 20% | Sufficient strength |
| **Wall Lines** | 3-4 | Increased strength |
| **Top/Bottom Layers** | 4-5 | Solid surfaces |
| **Print Speed** | 50mm/s | Standard quality |
| **Supports** | Minimal | Designed to minimize need |
| **Adhesion** | Brim or Raft | For larger parts |

### Material Recommendations

1. **PETG (Recommended)**
   - Temperature resistant up to 70°C
   - Good layer adhesion
   - Food-safe options available
   - Easier to print than ABS

2. **ABS**
   - Higher temperature resistance
   - Better chemical resistance
   - Requires heated bed and enclosure
   - May warp during printing

3. **ASA**
   - Similar to ABS but UV resistant
   - Good for outdoor use
   - Heat resistant

4. **PLA** (Not Recommended)
   - Will deform at operating temperatures
   - Only use for non-heated components

## Components

### Base Enclosure (base.stl)

**Dimensions**: Approximately 150mm x 120mm x 60mm

**Features**:
- Internal mounting posts for PCB
- M3 threaded inserts for secure mounting
- Cable routing channels
- Heater mounting surface with ventilation
- Non-slip feet attachment points
- Status LED light pipe

**Printing Notes**:
- Print with flat bottom down
- No supports needed
- Use brim for better adhesion
- Print time: ~6-8 hours

### Lid (lid.stl)

**Features**:
- Snap-fit closure mechanism
- Ventilation slots for airflow
- Access panel for USB programming
- Mounting points for display (optional)
- Sensor access port

**Printing Notes**:
- Print with flat side down
- Minimal supports may be needed for clips
- Print time: ~3-4 hours

### Sensor Mount (sensor_mount.stl)

**Features**:
- Holds DHT22 sensor securely
- Allows airflow around sensor
- Snap-fit or screw mounting
- Cable management clips

**Printing Notes**:
- Small part, prints quickly
- No supports needed
- Print time: ~30 minutes

### ESP8266 Mount (esp8266_mount.stl)

**Features**:
- Fits NodeMCU and Wemos D1 Mini
- USB access cutout
- Snap-fit to base
- M2.5 mounting holes as alternative

**Printing Notes**:
- Print standing up for best strength
- Minimal supports on USB cutout
- Print time: ~45 minutes

### Relay Mount (relay_mount.stl)

**Features**:
- Fits standard 5V relay modules
- Screw terminal access
- Snap-fit mounting
- Wire routing channels

**Printing Notes**:
- Print flat on bed
- No supports needed
- Print time: ~30 minutes

## Assembly Instructions

### Required Hardware

- M3 x 5mm heat-set inserts (8x)
- M3 x 8mm screws (8x)
- M2.5 x 6mm screws (4x) - for ESP8266
- Double-sided foam tape - for mounting heater
- Cable ties - for wire management
- Silicone sealant (optional) - for weatherproofing

### Tools Needed

- Soldering iron (for heat-set inserts)
- Screwdriver (Phillips M3)
- Wire cutters
- Craft knife

### Assembly Steps

#### 1. Prepare Printed Parts

- Remove all support material
- Clean up any stringing or blobs
- Test fit all components
- Sand contact surfaces if needed

#### 2. Install Heat-Set Inserts

- Heat soldering iron to 200-220°C
- Place insert on mounting hole
- Gently press insert with iron tip
- Let cool completely (1-2 minutes)
- Repeat for all 8 inserts in base

#### 3. Mount Components in Base

**A. Install Heating Element**:
- Clean heater mounting surface
- Apply double-sided foam tape
- Position heater centrally
- Press firmly for 30 seconds
- Route heater wires through channel

**B. Mount ESP8266**:
- Snap ESP8266 into mount bracket
- Position bracket in base
- Ensure USB port aligns with access hole
- Secure with M2.5 screws (if using screw mounting)

**C. Mount Relay Module**:
- Slide relay into mounting bracket
- Snap bracket into base
- Verify terminal access
- Secure if needed

**D. Mount DHT22 Sensor**:
- Insert sensor into mount
- Position mount near ventilation
- Snap into base
- Ensure good airflow around sensor

#### 4. Wire Connections

- Connect all components per main README wiring diagram
- Use cable ties for wire management
- Keep high voltage wires separate from low voltage
- Route wires through provided channels
- Leave some slack for service access

#### 5. Testing

- Before closing enclosure, test all functions:
  - Power on and check WiFi connection
  - Verify sensor readings
  - Test relay operation (without heater power)
  - Check web interface access

#### 6. Close Enclosure

- Verify all wires are secured
- Position lid over base
- Press down on snap-fit points
- Should hear/feel clicks as it locks
- Verify all seams are closed

#### 7. Final Assembly

- Attach feet to base (4x)
- Apply silicone sealant around seams (if weatherproofing)
- Connect heater power (⚠️ be careful with voltage!)
- Place in desired location

## Customization

The design can be modified for:

### Size Variations
- Scale for larger/smaller filament containers
- Multi-chamber versions for different filaments
- Stackable designs

### Feature Additions
- LCD display cutout
- External controls (buttons, switches)
- Multiple sensor ports
- Fan mounting for forced air circulation
- Viewing window with transparent acrylic

### Mounting Options
- Wall mounting brackets
- Pegboard hooks
- Drawer slide compatibility
- Stackable interlocking design

## Printing Tips

### First Layer Adhesion
- Clean bed with isopropyl alcohol
- Level bed carefully
- Appropriate bed temperature (PETG: 75-85°C)
- Use glue stick or hairspray if needed

### Warping Prevention
- Use enclosure for ABS/ASA
- Reduce cooling for first layers
- Increase bed temperature
- Use brim or raft

### Quality Improvements
- Calibrate extrusion multiplier
- Tune temperature for your filament
- Reduce print speed for better quality
- Enable "smooth spiralized outer contour" for lids

### Support Optimization
- Orient parts to minimize supports
- Use tree supports if available
- Increase support/model clearance for easier removal
- Consider support interface layers

## Maintenance

### Cleaning
- Wipe exterior with damp cloth
- Avoid harsh chemicals
- Check for dust buildup in ventilation
- Clean with compressed air if needed

### Inspection
- Periodically check for cracks or warping
- Verify mounting screws are tight
- Check that snap-fits are secure
- Inspect near heat sources for deformation

### Modifications
- If deformation occurs, reprint affected parts
- Consider upgrading to higher temp material
- Add additional ventilation if overheating
- Reinforce mounting points if needed

## Safety Considerations

⚠️ **Important**:
- Ensure adequate ventilation
- Don't block air intakes/exhausts
- Keep away from flammable materials
- Don't exceed material temperature ratings
- Regular inspection for damage
- Follow electrical safety guidelines

## Design Files

The parametric design files (Fusion 360) allow you to:
- Adjust dimensions for your components
- Modify mounting hole positions
- Add custom features
- Create variations for different sensors/controllers

### Opening in Fusion 360
1. Import the .f3d file
2. Modify parameters as needed
3. Update components
4. Export new STL files

### Opening in Other CAD Software
- Use the .step file for universal compatibility
- Most CAD programs can import STEP format
- Some features may need recreation

## Troubleshooting

### Print Issues

**Warping**:
- Increase bed temperature
- Use enclosure (for ABS/ASA)
- Add brim or raft
- Reduce cooling fan speed

**Poor Layer Adhesion**:
- Increase print temperature
- Decrease print speed
- Check for drafts
- Verify filament is dry

**Stringing**:
- Reduce print temperature
- Enable retraction
- Increase retraction distance
- Dry filament if hygroscopic

**Supports Difficult to Remove**:
- Increase support clearance
- Reduce support density
- Use tree supports
- Consider support interface layers

### Assembly Issues

**Parts Don't Fit**:
- Check for warping during print
- File or sand contact surfaces
- Adjust printer calibration
- Reprint with adjusted tolerance

**Snap-fits Too Loose/Tight**:
- Adjust in design file or
- Use glue for loose fits
- File/sand for tight fits
- Consider M3 screws as alternative

**Heat-set Inserts Melting Through**:
- Reduce iron temperature
- Press more slowly
- Increase wall thickness in design
- Use lower iron temperature with longer time

## Contributing

Improvements welcome! Please share:
- Enhanced designs
- Alternative mounting options
- Size variations
- Photos of your build
- Print setting optimizations

## License

3D design files released under Creative Commons BY-SA 4.0
