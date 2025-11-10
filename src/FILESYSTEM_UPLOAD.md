# Filament Greenhouse Dryer - Filesystem Upload Instructions (PlatformIO)

This project uses LittleFS to store HTML files on the ESP8266's internal flash memory. You need to upload the `data/` folder contents to the ESP8266 filesystem before uploading the main sketch.

## Project Structure

```
3d-Filament-GreenHouse/
├── platformio.ini          # PlatformIO configuration
├── data/                   # Filesystem files (web interface)
│   └── index.html         # Main web interface (HTML/CSS/JavaScript)
└── src/
    └── FilamentGreenhouse.ino  # Main Arduino code
```

## What's in the data/ folder?

- **index.html** - Main web interface (HTML/CSS/JavaScript)

The HTML file is loaded from the filesystem instead of being embedded in the code, making it easier to modify the interface without recompiling the sketch.

## Method 1: Using PlatformIO (Recommended)

### Prerequisites

1. **Install PlatformIO**: If you haven't already, install PlatformIO:
   - **VS Code**: Install the PlatformIO IDE extension from the VS Code marketplace
   - **Command Line**: Install via pip: `pip install platformio`

2. **Update platformio.ini**: Ensure your `platformio.ini` includes the filesystem configuration:
   ```ini
   [env:esp12e]
   platform = espressif8266
   board = esp12e
   framework = arduino
   board_build.filesystem = littlefs
   ```

### Step-by-Step Upload Process

#### Method A: Using VS Code with PlatformIO Extension

1. **Open the project** in VS Code (open the `3d-Filament-GreenHouse` folder)

2. **Verify data folder structure**: Ensure the `data/` folder is in the project root:
   ```
   3d-Filament-GreenHouse/
   ├── platformio.ini
   ├── data/
   │   └── index.html
   └── src/
       └── FilamentGreenhouse.ino
   ```

3. **Connect your ESP8266** via USB

4. **Build and upload filesystem**:
   - Open the PlatformIO toolbar (alien icon in the left sidebar)
   - Under "PROJECT TASKS" → "esp12e" → "Platform"
   - Click "Build Filesystem Image"
   - Once complete, click "Upload Filesystem Image"

5. **Upload the main code**:
   - Click "Upload" in the same menu, or use Ctrl+Alt+U

#### Method B: Using Command Line

1. **Navigate to project directory**:
   ```bash
   cd path/to/3d-Filament-GreenHouse
   ```

2. **Build filesystem image**:
   ```bash
   pio run --target buildfs
   ```

3. **Upload filesystem**:
   ```bash
   pio run --target uploadfs
   ```

4. **Upload the sketch**:
   ```bash
   pio run --target upload
   ```

#### Method C: Combined Upload (Filesystem + Code)

You can also do everything in one command:
```bash
pio run --target uploadfs --target upload
```

## Method 2: Using Arduino IDE (Alternative)

If you prefer Arduino IDE over PlatformIO:

1. **Install the LittleFS Upload Plugin**:
   - Download from: https://github.com/earlephilhower/arduino-esp8266littlefs-plugin/releases
   - Extract to `Arduino/tools/ESP8266LittleFS/tool/esp8266littlefs.jar`

2. **Copy files to Arduino structure**:
   ```
   FilamentGreenhouse/
   ├── FilamentGreenhouse.ino (copy from src/)
   └── data/
       └── index.html (copy from project root data/)
   ```

3. **Upload**: Tools → ESP8266 LittleFS Data Upload

## Method 3: Manual Upload with esptool (Advanced)

For manual filesystem image creation:

1. **Install mklittlefs**:
   ```bash
   # PlatformIO installs this automatically, or download from:
   # https://github.com/earlephilhower/mklittlefs/releases
   ```

2. **Create filesystem image**:
   ```bash
   mklittlefs -c data/ -s 0x100000 -b 4096 -p 256 littlefs.bin
   ```

3. **Upload using esptool**:
   ```bash
   esptool.py --port COM3 write_flash 0x300000 littlefs.bin
   ```
   (Replace COM3 with your port, and 0x300000 with your filesystem start address)

## Verifying Upload

After uploading both the filesystem and the sketch:

1. **Open Serial Monitor**:
   - **VS Code/PlatformIO**: Click "Monitor" in PlatformIO toolbar, or Ctrl+Alt+M
   - **Command Line**: `pio device monitor`
   - **Baud Rate**: 115200

2. **Reset the ESP8266** (press reset button or power cycle)

3. **Check output**: You should see:
   ```
   3D Filament Greenhouse Dryer
   =================================
   LittleFS mounted successfully
   Setting up Access Point...
   Access Point created successfully!
   Network name (SSID): FilamentDryer
   IP address: 192.168.4.1
   Access web interface at: http://192.168.4.1
   ```

4. **Connect and test**:
   - Connect to "FilamentDryer" WiFi network (password: filament123)
   - Open browser and go to http://192.168.4.1
   - You should see the Filament Greenhouse web interface

5. **If you see "File not found"**, the filesystem wasn't uploaded correctly

## Troubleshooting

### "Failed to mount LittleFS"
- **Cause**: Filesystem not uploaded or corrupted
- **Solution**: 
  - Ensure `board_build.filesystem = littlefs` is in platformio.ini
  - Re-upload filesystem: `pio run --target uploadfs`
  - Check that the correct board is selected in platformio.ini

### "File not found" in web browser
- **Cause**: Filesystem uploaded but index.html is missing or in wrong location
- **Solution**: 
  - Verify `data/index.html` exists in project root
  - Check Serial Monitor for LittleFS mount errors
  - Re-upload filesystem image

### PlatformIO upload fails
- **Cause**: Port, board, or connection issues
- **Solutions**:
  - Check COM port: `pio device list`
  - Ensure ESP8266 is connected and drivers installed
  - Close Serial Monitor before uploading
  - Try pressing reset during upload
  - Check platformio.ini board configuration

### VS Code PlatformIO tasks not showing
- **Cause**: PlatformIO extension not properly loaded
- **Solution**:
  - Reload VS Code window (Ctrl+Shift+P → "Developer: Reload Window")
  - Ensure you opened the project folder (not individual files)
  - Check PlatformIO extension is installed and enabled

### "Access Point not found" 
- **Cause**: ESP8266 not creating access point
- **Solution**:
  - Check Serial Monitor for WiFi setup messages
  - Verify the code was uploaded successfully
  - Reset the ESP8266
  - Check if ESP8266 is powered properly

### Web interface loads but shows old content
- **Cause**: Browser caching
- **Solution**: 
  - Hard refresh (Ctrl+F5)
  - Open incognito/private browsing window
  - Clear browser cache

## Modifying the Web Interface

To update the web interface:

1. **Edit the HTML file**: Modify `data/index.html` in the project root

2. **Re-upload filesystem**:
   - **VS Code**: Use PlatformIO → "Upload Filesystem Image"
   - **Command Line**: `pio run --target uploadfs`

3. **No need to re-upload the main sketch** unless you changed the `.ino` file

4. **View changes**: 
   - Clear browser cache (Ctrl+F5) or use incognito mode
   - The ESP8266 will serve the updated file immediately

## Filesystem Size

The default LittleFS partition is typically:
- **2MB flash**: ~64KB filesystem
- **4MB flash**: ~1MB filesystem (recommended)

The index.html file is about 14KB, leaving plenty of space for future additions like:
- Additional pages
- CSS/JS files
- Images or icons
- Configuration files

## Additional Notes

### PlatformIO vs Arduino IDE
This project is now structured for **PlatformIO**, which offers several advantages:
- **Better dependency management**: Libraries are automatically managed
- **Cross-platform**: Works consistently on Windows, Mac, and Linux  
- **Integrated build system**: No need for separate plugins
- **VS Code integration**: Full IDE experience with debugging support
- **Command line support**: Scriptable builds and uploads

### LittleFS vs SPIFFS
This project uses **LittleFS** instead of the older SPIFFS filesystem because:
- Better performance and reliability
- Better wear leveling and error handling
- Actively maintained and updated
- Same usage API as SPIFFS

### Project Benefits
- **No external WiFi needed**: ESP8266 creates its own access point
- **Direct connection**: Connect devices directly to "FilamentDryer" network
- **Portable setup**: Works anywhere without existing WiFi infrastructure
- **Easy web interface updates**: Modify HTML without recompiling code
