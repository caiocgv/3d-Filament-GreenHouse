# Filament Greenhouse Dryer - Filesystem Upload Instructions

This project uses LittleFS to store HTML files on the ESP8266's internal flash memory. You need to upload the `data/` folder contents to the ESP8266 filesystem before uploading the main sketch.

## What's in the data/ folder?

- **index.html** - Main web interface (HTML/CSS/JavaScript)

The HTML file is loaded from the filesystem instead of being embedded in the code, making it easier to modify the interface without recompiling the Arduino sketch.

## Method 1: Using Arduino IDE Plugin (Recommended)

### Step 1: Install the LittleFS Upload Plugin

1. Download the ESP8266 LittleFS Data Upload plugin:
   - Go to: https://github.com/earlephilhower/arduino-esp8266littlefs-plugin/releases
   - Download the latest `.zip` file (e.g., `ESP8266LittleFS-2.6.0.zip`)

2. Install the plugin:
   - Close Arduino IDE if it's running
   - Create a `tools` folder in your Arduino sketchbook folder if it doesn't exist:
     - Windows: `C:\Users\[YourUsername]\Documents\Arduino\tools\`
     - Mac: `~/Documents/Arduino/tools/`
     - Linux: `~/Arduino/tools/`
   - Extract the downloaded ZIP into the `tools` folder
   - You should have: `Arduino/tools/ESP8266LittleFS/tool/esp8266littlefs.jar`

3. Restart Arduino IDE

### Step 2: Upload the Filesystem

1. Open the FilamentGreenhouse.ino sketch
2. Make sure the `data/` folder is in the same directory as the `.ino` file:
   ```
   FilamentGreenhouse/
   ├── FilamentGreenhouse.ino
   └── data/
       └── index.html
   ```
3. Connect your ESP8266 via USB
4. Select your board and port in Arduino IDE
5. Go to **Tools → ESP8266 LittleFS Data Upload**
6. Wait for the upload to complete (you'll see progress in the console)
7. Once complete, upload the main sketch as usual

## Method 2: Using PlatformIO

If you're using PlatformIO instead of Arduino IDE:

1. Ensure your `platformio.ini` includes:
   ```ini
   [env:nodemcuv2]
   platform = espressif8266
   board = nodemcuv2
   framework = arduino
   board_build.filesystem = littlefs
   ```

2. Place the `data/` folder in your project root (same level as `src/`)

3. Upload filesystem:
   ```bash
   pio run --target uploadfs
   ```

4. Upload the sketch:
   ```bash
   pio run --target upload
   ```

## Method 3: Using mklittlefs Tool (Advanced)

For manual filesystem image creation:

1. Download `mklittlefs` tool for your OS from:
   https://github.com/earlephilhower/mklittlefs/releases

2. Create filesystem image:
   ```bash
   mklittlefs -c data/ -s 0x100000 -b 4096 -p 256 littlefs.bin
   ```

3. Upload using esptool:
   ```bash
   esptool.py --port COM3 write_flash 0x300000 littlefs.bin
   ```
   (Replace COM3 with your port, and 0x300000 with your filesystem start address)

## Verifying Upload

After uploading both the filesystem and the sketch:

1. Open Serial Monitor (115200 baud)
2. Reset the ESP8266
3. You should see: `LittleFS mounted successfully`
4. Access the web interface at the IP address shown
5. If you see "File not found", the filesystem wasn't uploaded correctly

## Troubleshooting

### "Failed to mount LittleFS"
- Filesystem not uploaded or corrupted
- Try uploading the filesystem again
- Make sure you selected the correct board

### "File not found" in web browser
- Filesystem uploaded but index.html is missing or in wrong location
- Verify data/index.html exists before uploading
- Check Serial Monitor for filesystem errors

### Plugin not showing in Tools menu
- Plugin not installed correctly
- Check tools folder structure: `Arduino/tools/ESP8266LittleFS/tool/esp8266littlefs.jar`
- Restart Arduino IDE
- Make sure you have ESP8266 board support installed

### Upload fails or times out
- Wrong board selected
- Wrong port selected
- ESP8266 not in upload mode (try pressing reset)
- Close Serial Monitor before uploading

## Modifying the Web Interface

To update the web interface:

1. Edit `data/index.html`
2. Re-upload the filesystem using one of the methods above
3. No need to re-upload the main sketch unless you changed the `.ino` file
4. Clear browser cache or open in incognito mode to see changes

## Filesystem Size

The default LittleFS partition is typically:
- **2MB flash**: ~64KB filesystem
- **4MB flash**: ~1MB filesystem (recommended)

The index.html file is about 14KB, leaving plenty of space for future additions like:
- Additional pages
- CSS/JS files
- Images or icons
- Configuration files

## Note on SPIFFS vs LittleFS

This project uses **LittleFS** instead of the older SPIFFS filesystem because:
- Better performance
- More reliable
- Better wear leveling
- Actively maintained
- Same usage API

If you see references to SPIFFS in other projects, the usage is nearly identical - just replace "SPIFFS" with "LittleFS" in most cases.
