/*
 * 3D Filament Greenhouse Dryer
 * ESP8266-based smart filament dryer with web interface
 * 
 * Features:
 * - WiFi web server with real-time monitoring
 * - Temperature and humidity sensing (DHT22)
 * - Adjustable temperature control
 * - Timer functionality
 * - Relay control for heating element
 * 
 * Hardware:
 * - ESP8266 (NodeMCU or Wemos D1 Mini)
 * - DHT22 temperature/humidity sensor
 * - Relay module (5V)
 * - Heating element (40-60W)
 */

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <DHT.h>

// WiFi Configuration
const char* ssid = "YOUR_WIFI_SSID";          // Replace with your WiFi SSID
const char* password = "YOUR_WIFI_PASSWORD";  // Replace with your WiFi password

// Pin Definitions
#define DHTPIN 2          // DHT22 sensor connected to GPIO2 (D4)
#define DHTTYPE DHT22     // DHT 22 (AM2302)
#define RELAYPIN 5        // Relay connected to GPIO5 (D1)

// Temperature Control Parameters
#define MAX_TEMP 75       // Maximum allowed temperature (safety limit)
#define TEMP_HYSTERESIS 2 // Temperature hysteresis in Celsius

// Initialize sensor and web server
DHT dht(DHTPIN, DHTTYPE);
ESP8266WebServer server(80);

// Global Variables
float currentTemp = 0;
float currentHumidity = 0;
float targetTemp = 50;
bool systemOn = false;
bool heaterOn = false;
bool timerRunning = false;
unsigned long timerDuration = 0;        // Timer duration in seconds
unsigned long timerStartTime = 0;       // Timer start time in millis
unsigned long lastSensorRead = 0;
unsigned long lastTempCheck = 0;

void setup() {
  Serial.begin(115200);
  delay(100);
  
  Serial.println("\n\n=================================");
  Serial.println("3D Filament Greenhouse Dryer");
  Serial.println("=================================");
  
  // Initialize pins
  pinMode(RELAYPIN, OUTPUT);
  digitalWrite(RELAYPIN, LOW);  // Start with heater off
  
  // Initialize DHT sensor
  dht.begin();
  Serial.println("DHT22 sensor initialized");
  
  // Connect to WiFi
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 30) {
    delay(500);
    Serial.print(".");
    attempts++;
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nWiFi connected!");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    Serial.print("Access web interface at: http://");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("\nFailed to connect to WiFi!");
    Serial.println("Please check credentials and try again.");
  }
  
  // Setup web server routes
  server.on("/", handleRoot);
  server.on("/status", handleStatus);
  server.on("/setTemp", HTTP_POST, handleSetTemp);
  server.on("/setTimer", HTTP_POST, handleSetTimer);
  server.on("/power", HTTP_POST, handlePower);
  server.onNotFound(handleNotFound);
  
  // Start web server
  server.begin();
  Serial.println("Web server started");
  Serial.println("=================================\n");
  
  // Initial sensor reading
  readSensors();
}

void loop() {
  // Handle web server requests
  server.handleClient();
  
  // Read sensors every 2 seconds
  if (millis() - lastSensorRead > 2000) {
    readSensors();
    lastSensorRead = millis();
  }
  
  // Temperature control check every second
  if (millis() - lastTempCheck > 1000) {
    controlTemperature();
    lastTempCheck = millis();
  }
  
  // Timer management
  if (timerRunning) {
    unsigned long elapsedSeconds = (millis() - timerStartTime) / 1000;
    if (elapsedSeconds >= timerDuration) {
      // Timer expired
      Serial.println("Timer expired - shutting down");
      systemOn = false;
      timerRunning = false;
      digitalWrite(RELAYPIN, LOW);
      heaterOn = false;
    }
  }
}

void readSensors() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  
  // Check if readings are valid
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  
  currentTemp = t;
  currentHumidity = h;
  
  // Print to serial for debugging
  Serial.print("Temperature: ");
  Serial.print(currentTemp);
  Serial.print("°C | Humidity: ");
  Serial.print(currentHumidity);
  Serial.print("% | Heater: ");
  Serial.print(heaterOn ? "ON" : "OFF");
  Serial.print(" | Target: ");
  Serial.print(targetTemp);
  Serial.println("°C");
}

void controlTemperature() {
  if (!systemOn) {
    // System is off, ensure heater is off
    if (heaterOn) {
      digitalWrite(RELAYPIN, LOW);
      heaterOn = false;
    }
    return;
  }
  
  // Safety check - never exceed maximum temperature
  if (currentTemp >= MAX_TEMP) {
    digitalWrite(RELAYPIN, LOW);
    heaterOn = false;
    systemOn = false;
    Serial.println("SAFETY: Maximum temperature exceeded! System shutdown.");
    return;
  }
  
  // Temperature control with hysteresis
  if (currentTemp < (targetTemp - TEMP_HYSTERESIS)) {
    // Temperature too low, turn heater on
    if (!heaterOn) {
      digitalWrite(RELAYPIN, HIGH);
      heaterOn = true;
      Serial.println("Heater turned ON");
    }
  } else if (currentTemp > targetTemp) {
    // Temperature reached or exceeded, turn heater off
    if (heaterOn) {
      digitalWrite(RELAYPIN, LOW);
      heaterOn = false;
      Serial.println("Heater turned OFF");
    }
  }
  // Within hysteresis range: maintain current state
}

void handleRoot() {
  String html = getWebInterface();
  server.send(200, "text/html", html);
}

void handleStatus() {
  unsigned long remainingTime = 0;
  if (timerRunning) {
    unsigned long elapsedSeconds = (millis() - timerStartTime) / 1000;
    if (elapsedSeconds < timerDuration) {
      remainingTime = timerDuration - elapsedSeconds;
    }
  }
  
  String json = "{";
  json += "\"temperature\":" + String(currentTemp, 1) + ",";
  json += "\"humidity\":" + String(currentHumidity, 1) + ",";
  json += "\"targetTemp\":" + String(targetTemp, 1) + ",";
  json += "\"heaterOn\":" + String(heaterOn ? "true" : "false") + ",";
  json += "\"systemOn\":" + String(systemOn ? "true" : "false") + ",";
  json += "\"timerRunning\":" + String(timerRunning ? "true" : "false") + ",";
  json += "\"remainingTime\":" + String(remainingTime);
  json += "}";
  
  server.send(200, "application/json", json);
}

void handleSetTemp() {
  if (server.hasArg("plain")) {
    String body = server.arg("plain");
    
    // Simple parsing for {"temp":50}
    int tempIndex = body.indexOf("\"temp\"");
    if (tempIndex >= 0) {
      int colonIndex = body.indexOf(":", tempIndex);
      int endIndex = body.indexOf("}", colonIndex);
      if (colonIndex >= 0 && endIndex >= 0) {
        String tempStr = body.substring(colonIndex + 1, endIndex);
        tempStr.trim();
        
        float newTemp = tempStr.toFloat();
        if (newTemp >= 0 && newTemp <= 70) {
          targetTemp = newTemp;
          Serial.print("Target temperature set to: ");
          Serial.println(targetTemp);
          
          String json = "{\"success\":true,\"targetTemp\":" + String(targetTemp, 1) + "}";
          server.send(200, "application/json", json);
          return;
        }
      }
    }
  }
  
  server.send(400, "application/json", "{\"success\":false,\"error\":\"Invalid temperature\"}");
}

void handleSetTimer() {
  if (server.hasArg("plain")) {
    String body = server.arg("plain");
    
    // Simple parsing for {"seconds":3600}
    int secondsIndex = body.indexOf("\"seconds\"");
    if (secondsIndex >= 0) {
      int colonIndex = body.indexOf(":", secondsIndex);
      int endIndex = body.indexOf("}", colonIndex);
      if (colonIndex >= 0 && endIndex >= 0) {
        String secondsStr = body.substring(colonIndex + 1, endIndex);
        secondsStr.trim();
        
        unsigned long seconds = secondsStr.toInt();
        if (seconds > 0 && seconds <= 86400) { // Max 24 hours
          timerDuration = seconds;
          timerStartTime = millis();
          timerRunning = true;
          systemOn = true;
          
          Serial.print("Timer set to: ");
          Serial.print(seconds);
          Serial.println(" seconds");
          
          String json = "{\"success\":true,\"timerSet\":" + String(seconds) + "}";
          server.send(200, "application/json", json);
          return;
        }
      }
    }
  }
  
  server.send(400, "application/json", "{\"success\":false,\"error\":\"Invalid timer duration\"}");
}

void handlePower() {
  if (server.hasArg("plain")) {
    String body = server.arg("plain");
    
    // Simple parsing for {"on":true}
    bool turnOn = body.indexOf("true") >= 0;
    
    systemOn = turnOn;
    if (!systemOn) {
      timerRunning = false;
      digitalWrite(RELAYPIN, LOW);
      heaterOn = false;
      Serial.println("System turned OFF");
    } else {
      Serial.println("System turned ON");
    }
    
    String json = "{\"success\":true,\"systemOn\":" + String(systemOn ? "true" : "false") + "}";
    server.send(200, "application/json", json);
    return;
  }
  
  server.send(400, "application/json", "{\"success\":false,\"error\":\"Invalid power command\"}");
}

void handleNotFound() {
  server.send(404, "text/plain", "404: Not Found");
}

String getWebInterface() {
  String html = R"=====(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Filament Greenhouse Dryer</title>
    <style>
        * {
            margin: 0;
            padding: 0;
            box-sizing: border-box;
        }
        
        body {
            font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;
            background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
            min-height: 100vh;
            display: flex;
            justify-content: center;
            align-items: center;
            padding: 20px;
        }
        
        .container {
            background: white;
            border-radius: 20px;
            padding: 30px;
            max-width: 600px;
            width: 100%;
            box-shadow: 0 20px 60px rgba(0,0,0,0.3);
        }
        
        h1 {
            text-align: center;
            color: #333;
            margin-bottom: 10px;
            font-size: 28px;
        }
        
        .subtitle {
            text-align: center;
            color: #666;
            margin-bottom: 30px;
            font-size: 14px;
        }
        
        .readings {
            display: grid;
            grid-template-columns: 1fr 1fr;
            gap: 20px;
            margin-bottom: 30px;
        }
        
        .reading {
            background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
            padding: 20px;
            border-radius: 15px;
            text-align: center;
            color: white;
        }
        
        .reading-label {
            font-size: 14px;
            opacity: 0.9;
            margin-bottom: 5px;
        }
        
        .reading-value {
            font-size: 36px;
            font-weight: bold;
        }
        
        .reading-unit {
            font-size: 18px;
            opacity: 0.9;
        }
        
        .control-group {
            background: #f8f9fa;
            padding: 20px;
            border-radius: 15px;
            margin-bottom: 20px;
        }
        
        .control-label {
            font-size: 16px;
            font-weight: 600;
            color: #333;
            margin-bottom: 10px;
            display: block;
        }
        
        .slider-container {
            margin-bottom: 10px;
        }
        
        input[type="range"] {
            width: 100%;
            height: 8px;
            border-radius: 5px;
            background: #ddd;
            outline: none;
            -webkit-appearance: none;
        }
        
        input[type="range"]::-webkit-slider-thumb {
            -webkit-appearance: none;
            appearance: none;
            width: 24px;
            height: 24px;
            border-radius: 50%;
            background: #667eea;
            cursor: pointer;
        }
        
        input[type="range"]::-moz-range-thumb {
            width: 24px;
            height: 24px;
            border-radius: 50%;
            background: #667eea;
            cursor: pointer;
            border: none;
        }
        
        .slider-value {
            text-align: center;
            font-size: 24px;
            font-weight: bold;
            color: #667eea;
            margin-top: 5px;
        }
        
        .timer-inputs {
            display: grid;
            grid-template-columns: 1fr 1fr auto;
            gap: 10px;
            align-items: end;
        }
        
        .input-group {
            display: flex;
            flex-direction: column;
        }
        
        .input-group label {
            font-size: 12px;
            color: #666;
            margin-bottom: 5px;
        }
        
        input[type="number"] {
            padding: 10px;
            border: 2px solid #ddd;
            border-radius: 8px;
            font-size: 16px;
            text-align: center;
        }
        
        button {
            background: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
            color: white;
            border: none;
            padding: 12px 24px;
            border-radius: 8px;
            font-size: 16px;
            font-weight: 600;
            cursor: pointer;
            transition: transform 0.2s;
        }
        
        button:hover {
            transform: translateY(-2px);
        }
        
        button:active {
            transform: translateY(0);
        }
        
        .power-button {
            width: 100%;
            padding: 15px;
            font-size: 18px;
            margin-top: 10px;
        }
        
        .power-button.off {
            background: linear-gradient(135deg, #f093fb 0%, #f5576c 100%);
        }
        
        .status-indicators {
            display: flex;
            justify-content: space-around;
            margin-bottom: 20px;
        }
        
        .indicator {
            display: flex;
            align-items: center;
            gap: 8px;
            font-size: 14px;
            color: #666;
        }
        
        .indicator-dot {
            width: 12px;
            height: 12px;
            border-radius: 50%;
            background: #ddd;
        }
        
        .indicator-dot.active {
            background: #4caf50;
            animation: pulse 2s infinite;
        }
        
        @keyframes pulse {
            0%, 100% { opacity: 1; }
            50% { opacity: 0.5; }
        }
        
        .timer-display {
            text-align: center;
            font-size: 20px;
            color: #667eea;
            font-weight: 600;
            margin-top: 10px;
        }
        
        @media (max-width: 480px) {
            .readings {
                grid-template-columns: 1fr;
            }
            
            .timer-inputs {
                grid-template-columns: 1fr;
            }
            
            .timer-inputs button {
                width: 100%;
            }
        }
    </style>
</head>
<body>
    <div class="container">
        <h1>🌡️ Filament Greenhouse Dryer</h1>
        <div class="subtitle">Smart Filament Drying System</div>
        
        <div class="readings">
            <div class="reading">
                <div class="reading-label">Temperature</div>
                <div class="reading-value">
                    <span id="temp">--</span>
                    <span class="reading-unit">°C</span>
                </div>
            </div>
            <div class="reading">
                <div class="reading-label">Humidity</div>
                <div class="reading-value">
                    <span id="humidity">--</span>
                    <span class="reading-unit">%</span>
                </div>
            </div>
        </div>
        
        <div class="status-indicators">
            <div class="indicator">
                <div class="indicator-dot" id="systemDot"></div>
                <span>System: <span id="systemStatus">OFF</span></span>
            </div>
            <div class="indicator">
                <div class="indicator-dot" id="heaterDot"></div>
                <span>Heater: <span id="heaterStatus">OFF</span></span>
            </div>
            <div class="indicator">
                <div class="indicator-dot" id="timerDot"></div>
                <span>Timer: <span id="timerStatus">OFF</span></span>
            </div>
        </div>
        
        <div class="control-group">
            <label class="control-label">Target Temperature</label>
            <div class="slider-container">
                <input type="range" id="tempSlider" min="0" max="70" value="50" step="1">
                <div class="slider-value"><span id="targetTemp">50</span>°C</div>
            </div>
        </div>
        
        <div class="control-group">
            <label class="control-label">Drying Timer</label>
            <div class="timer-inputs">
                <div class="input-group">
                    <label>Hours</label>
                    <input type="number" id="hours" min="0" max="24" value="4">
                </div>
                <div class="input-group">
                    <label>Minutes</label>
                    <input type="number" id="minutes" min="0" max="59" value="0">
                </div>
                <button onclick="setTimer()">Set Timer</button>
            </div>
            <div class="timer-display" id="timerDisplay">--:--:--</div>
        </div>
        
        <button class="power-button" id="powerBtn" onclick="togglePower()">
            Turn ON
        </button>
    </div>
    
    <script>
        let systemOn = false;
        
        // Update target temp display when slider moves
        document.getElementById('tempSlider').addEventListener('input', function() {
            document.getElementById('targetTemp').textContent = this.value;
        });
        
        // Update target temp when slider is released
        document.getElementById('tempSlider').addEventListener('change', function() {
            setTemperature(this.value);
        });
        
        function updateStatus() {
            fetch('/status')
                .then(response => response.json())
                .then(data => {
                    document.getElementById('temp').textContent = data.temperature.toFixed(1);
                    document.getElementById('humidity').textContent = data.humidity.toFixed(1);
                    document.getElementById('targetTemp').textContent = data.targetTemp.toFixed(0);
                    document.getElementById('tempSlider').value = data.targetTemp;
                    
                    systemOn = data.systemOn;
                    
                    // Update system status
                    document.getElementById('systemStatus').textContent = data.systemOn ? 'ON' : 'OFF';
                    document.getElementById('systemDot').classList.toggle('active', data.systemOn);
                    
                    // Update heater status
                    document.getElementById('heaterStatus').textContent = data.heaterOn ? 'ON' : 'OFF';
                    document.getElementById('heaterDot').classList.toggle('active', data.heaterOn);
                    
                    // Update timer status
                    document.getElementById('timerStatus').textContent = data.timerRunning ? 'RUNNING' : 'OFF';
                    document.getElementById('timerDot').classList.toggle('active', data.timerRunning);
                    
                    // Update timer display
                    if (data.timerRunning && data.remainingTime > 0) {
                        const hours = Math.floor(data.remainingTime / 3600);
                        const minutes = Math.floor((data.remainingTime % 3600) / 60);
                        const seconds = data.remainingTime % 60;
                        document.getElementById('timerDisplay').textContent = 
                            String(hours).padStart(2, '0') + ':' + 
                            String(minutes).padStart(2, '0') + ':' + 
                            String(seconds).padStart(2, '0');
                    } else {
                        document.getElementById('timerDisplay').textContent = '--:--:--';
                    }
                    
                    // Update power button
                    const powerBtn = document.getElementById('powerBtn');
                    if (data.systemOn) {
                        powerBtn.textContent = 'Turn OFF';
                        powerBtn.classList.add('off');
                    } else {
                        powerBtn.textContent = 'Turn ON';
                        powerBtn.classList.remove('off');
                    }
                })
                .catch(error => console.error('Error fetching status:', error));
        }
        
        function setTemperature(temp) {
            fetch('/setTemp', {
                method: 'POST',
                headers: {'Content-Type': 'application/json'},
                body: JSON.stringify({temp: parseFloat(temp)})
            })
            .then(response => response.json())
            .then(data => {
                if (data.success) {
                    console.log('Temperature set to:', data.targetTemp);
                }
            })
            .catch(error => console.error('Error setting temperature:', error));
        }
        
        function setTimer() {
            const hours = parseInt(document.getElementById('hours').value) || 0;
            const minutes = parseInt(document.getElementById('minutes').value) || 0;
            const seconds = (hours * 3600) + (minutes * 60);
            
            if (seconds <= 0) {
                alert('Please enter a valid time');
                return;
            }
            
            fetch('/setTimer', {
                method: 'POST',
                headers: {'Content-Type': 'application/json'},
                body: JSON.stringify({seconds: seconds})
            })
            .then(response => response.json())
            .then(data => {
                if (data.success) {
                    console.log('Timer set to:', data.timerSet, 'seconds');
                    updateStatus();
                }
            })
            .catch(error => console.error('Error setting timer:', error));
        }
        
        function togglePower() {
            fetch('/power', {
                method: 'POST',
                headers: {'Content-Type': 'application/json'},
                body: JSON.stringify({on: !systemOn})
            })
            .then(response => response.json())
            .then(data => {
                if (data.success) {
                    console.log('System power:', data.systemOn ? 'ON' : 'OFF');
                    updateStatus();
                }
            })
            .catch(error => console.error('Error toggling power:', error));
        }
        
        // Update status every 2 seconds
        setInterval(updateStatus, 2000);
        
        // Initial update
        updateStatus();
    </script>
</body>
</html>
)=====";
  
  return html;
}
