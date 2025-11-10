/*
 * 3D Filament Greenhouse Dryer
 * ESP8266-based smart filament dryer with web interface
 * 
 * Features:
 * - WiFi web server with real-time monitoring
 * - Temperature sensing (LM35)
 * - Adjustable temperature control
 * - Timer functionality
 * - Relay control for heating element
 * - HTML pages loaded from SPIFFS filesystem
 * 
 * Hardware:
 * - ESP8266 (NodeMCU or Wemos D1 Mini)
 * - LM35 temperature sensor
 * - Relay module (5V)
 * - Heating element (40-60W)
 */

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <LittleFS.h>

// WiFi Configuration - Access Point Mode
const char* ap_ssid = "FilamentDryer";     // Access Point name

// Pin Definitions
#define LM35PIN A0        // LM35 sensor connected to A0 (analog input)
#define RELAYPIN 5        // Relay connected to GPIO5 (D1)

// Temperature Control Parameters
#define MAX_TEMP 75       // Maximum allowed temperature (safety limit)
#define TEMP_HYSTERESIS 2 // Temperature hysteresis in Celsius

// Initialize web server
ESP8266WebServer server(80);

// Global Variables
float currentTemp = 0;
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
  
  // Initialize LittleFS
  if (!LittleFS.begin()) {
    Serial.println("Failed to mount LittleFS!");
    Serial.println("Please upload filesystem image using:");
    Serial.println("Arduino IDE: Tools -> ESP8266 LittleFS Data Upload");
    Serial.println("Or use the 'mklittlefs' tool");
  } else {
    Serial.println("LittleFS mounted successfully");
  }
  
  // Initialize pins
  pinMode(RELAYPIN, OUTPUT);
  digitalWrite(RELAYPIN, LOW);  // Start with heater off
  
  // Initialize ADC for LM35 reading
  pinMode(LM35PIN, INPUT);
  Serial.println("LM35 temperature sensor initialized");
  
  // Setup Access Point
  WiFi.mode(WIFI_AP);
  Serial.print("Setting up Access Point...");
  
  bool result = WiFi.softAP(ap_ssid, NULL); // No password for open AP
  
  if (result) {
    Serial.println("\nAccess Point created successfully!");
    Serial.print("Network name (SSID): ");
    Serial.println(ap_ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.softAPIP());
    Serial.print("Access web interface at: http://");
    Serial.println(WiFi.softAPIP());
  } else {
    Serial.println("\nFailed to create Access Point!");
    Serial.println("Please check configuration and try again.");
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
  // Read LM35 sensor
  // LM35 outputs 10mV per degree Celsius
  // ESP8266 ADC reads 0-1V as 0-1023 (10-bit ADC)
  // Temperature (°C) = (analogRead * (1.0 / 1023.0)) * 100
  
  int adcValue = analogRead(LM35PIN);
  float voltage = (adcValue / 1023.0) * 1.0;  // Convert to voltage (0-1V range)
  float temperature = voltage * 100.0;         // LM35: 10mV/°C = 0.01V/°C
  
  // Validate reading (LM35 range: 0°C to 100°C)
  if (temperature < -10 || temperature > 150) {
    Serial.println("Failed to read from LM35 sensor! Invalid temperature reading.");
    return;
  }
  
  currentTemp = temperature;
  
  // Print to serial for debugging
  Serial.print("Temperature: ");
  Serial.print(currentTemp);
  Serial.print("°C | Heater: ");
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
  // Serve index.html from LittleFS
  File file = LittleFS.open("/index.html", "r");
  if (!file) {
    server.send(404, "text/plain", "File not found. Please upload filesystem image.");
    return;
  }
  
  server.streamFile(file, "text/html");
  file.close();
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
