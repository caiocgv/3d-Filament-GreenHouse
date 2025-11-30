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
#include <DNSServer.h>
#include <LittleFS.h>

// WiFi Configuration - Access Point Mode
const char* ap_ssid = "FilamentDryer";     // Access Point name

// Pin Definitions
#define LM35PIN A0        // LM35 sensor connected to A0 (analog input)
#define RELAYPIN 4        // Relay connected to GPIO4 (D2) - Safe pin, no serial conflicts
#define LED_PIN LED_BUILTIN  // Onboard LED to show heater status

// Temperature Control Parameters
#define MAX_TEMP 75       // Maximum allowed temperature (safety limit)
#define TEMP_HYSTERESIS 2 // Temperature hysteresis in Celsius

// Initialize web server and DNS server for captive portal
ESP8266WebServer server(80);
DNSServer dnsServer;

// Captive portal constants
const byte DNS_PORT = 53;

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

void readSensors() {
  // Read LM35 sensor
  // LM35 outputs 10mV per degree Celsius
  // ESP8266 ADC reads 0-1V as 0-1023 (10-bit ADC)
  // Temperature (°C) = (analogRead * (1.0 / 1023.0)) * 100
  
  int adcValue = analogRead(LM35PIN);
  float voltage = (adcValue / 1023.0) * 3.3;  // Convert to voltage (0-3.3V range)
  float temperature = voltage * 100.0;         // LM35: 10mV/°C = 0.01V/°C
  
  // Validate reading (LM35 range: 0°C to 100°C)
  if (temperature < -10 || temperature > 150) {
    return;  // Invalid reading, skip update
  }
  
  currentTemp = temperature;
}

void controlTemperature() {
  if (!systemOn) {
    // System is off, ensure heater is off
    if (heaterOn) {
      pinMode(RELAYPIN, INPUT);  // Disable pin (floating) to turn relay OFF
      digitalWrite(LED_PIN, HIGH);  // LED OFF (active LOW)
      heaterOn = false;
    }
    return;
  }
  
  // Safety check - never exceed maximum temperature
  if (currentTemp >= MAX_TEMP) {
    pinMode(RELAYPIN, INPUT);  // Disable pin (floating) to turn relay OFF
    digitalWrite(LED_PIN, HIGH);  // LED OFF (active LOW)
    heaterOn = false;
    systemOn = false;
    return;
  }
  
  // Temperature control with hysteresis
  if (currentTemp < (targetTemp - TEMP_HYSTERESIS)) {
    // Temperature too low, turn heater on
    if (!heaterOn) {
      pinMode(RELAYPIN, OUTPUT);
      digitalWrite(RELAYPIN, HIGH);  // Enable pin and set HIGH to turn relay ON
      digitalWrite(LED_PIN, LOW);  // LED ON (active LOW)
      heaterOn = true;
    }
  } else if (currentTemp > targetTemp) {
    // Temperature reached or exceeded, turn heater off
    if (heaterOn) {
      pinMode(RELAYPIN, INPUT);  // Disable pin (floating) to turn relay OFF
      digitalWrite(LED_PIN, HIGH);  // LED OFF (active LOW)
      heaterOn = false;
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
      pinMode(RELAYPIN, INPUT);  // Disable pin (floating) to turn relay OFF
      digitalWrite(LED_PIN, HIGH);  // LED OFF (active LOW)
      heaterOn = false;
    }
    
    String json = "{\"success\":true,\"systemOn\":" + String(systemOn ? "true" : "false") + "}";
    server.send(200, "application/json", json);
    return;
  }
  
  server.send(400, "application/json", "{\"success\":false,\"error\":\"Invalid power command\"}");
}

void handleCaptivePortal() {
  // Simple redirect to the main interface
  IPAddress apIP = WiFi.softAPIP();
  String redirectURL = "http://" + apIP.toString() + "/";
  
  server.sendHeader("Location", redirectURL);
  server.send(302, "text/plain", "");
}

void handleNotFound() {
  // Simple 404 response
  server.send(404, "text/plain", "404: Not Found");
}

void setup() {
  // Initialize hardware FIRST (prevents DTR/RTS issues)
  
  // Initialize relay pin as INPUT (floating) - relay starts OFF
  pinMode(RELAYPIN, INPUT);
  
  // Initialize onboard LED to mirror heater status
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);  // LED OFF (active LOW) - heater starts off
  
  // Visual boot indicator - 3 rapid flashes
  for (int i = 0; i < 3; i++) {
    digitalWrite(LED_PIN, LOW);   // LED ON
    delay(100);
    digitalWrite(LED_PIN, HIGH);  // LED OFF
    delay(100);
  }
  
  // Initialize ADC for LM35 reading
  pinMode(LM35PIN, INPUT);
  
  delay(200);
  
  // Initialize LittleFS
  if (!LittleFS.begin()) {
    // Flash LED rapidly 10 times to indicate filesystem error
    for (int i = 0; i < 10; i++) {
      digitalWrite(LED_PIN, LOW);
      delay(50);
      digitalWrite(LED_PIN, HIGH);
      delay(50);
    }
  }
  
  // Setup Access Point
  WiFi.mode(WIFI_AP);
  
  bool result = WiFi.softAP(ap_ssid, NULL); // No password for open AP
  
  if (result) {
    // Visual indicator - 2 slow blinks = WiFi AP Ready
    for (int i = 0; i < 2; i++) {
      digitalWrite(LED_PIN, LOW);
      delay(300);
      digitalWrite(LED_PIN, HIGH);
      delay(300);
    }
    
    // Start DNS server for captive portal
    dnsServer.start(DNS_PORT, "*", WiFi.softAPIP());
  } else {
    // Visual error - rapid blinking (SOS pattern)
    while (true) {
      for (int i = 0; i < 10; i++) {
        digitalWrite(LED_PIN, LOW);
        delay(100);
        digitalWrite(LED_PIN, HIGH);
        delay(100);
      }
      delay(1000);
    }
  }
  
  // Setup web server routes
  server.on("/", handleRoot);
  server.on("/status", handleStatus);
  server.on("/setTemp", HTTP_POST, handleSetTemp);
  server.on("/setTimer", HTTP_POST, handleSetTimer);
  server.on("/power", HTTP_POST, handlePower);
  
  // Basic captive portal routes
  server.on("/generate_204", handleCaptivePortal);
  server.on("/hotspot-detect.html", handleCaptivePortal);
  server.on("/connecttest.txt", handleCaptivePortal);
  
  server.onNotFound(handleNotFound);
  
  // Start web server
  server.begin();
  
  // Initial sensor reading
  readSensors();
}

void loop() {
  // Handle DNS requests for captive portal
  dnsServer.processNextRequest();
  
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
      systemOn = false;
      timerRunning = false;
      pinMode(RELAYPIN, INPUT);  // Disable pin (floating) to turn relay OFF
      digitalWrite(LED_PIN, HIGH);  // LED OFF (active LOW)
      heaterOn = false;
    }
  }
}