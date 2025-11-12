/*
 * WiFi Access Point Test
 * 
 * This sketch tests ESP8266 Access Point functionality.
 * Use this to verify your ESP8266 can create its own WiFi network
 * before uploading the full Filament Greenhouse code.
 * 
 * The ESP8266 will create a WiFi network that you can connect to with your phone/computer.
 * 
 * Upload this sketch and open Serial Monitor at 115200 baud to see results.
 */

#include <Arduino.h>
#include <ESP8266WiFi.h>

// Access Point configuration - You can modify these if needed
const char* ap_ssid = "ESP8266-Test";        // Access Point name
const char* ap_password = "test12345";       // Access Point password (min 8 chars)

void setup() {
  Serial.begin(9600);
  delay(100);
  
  Serial.println("\n\n==============================");
  Serial.println("ESP8266 Access Point Test");
  Serial.println("==============================\n");
  
  // Print ESP8266 chip information
  Serial.print("Chip ID: ");
  Serial.println(ESP.getChipId(), HEX);
  Serial.print("Flash Chip ID: ");
  Serial.println(ESP.getFlashChipId(), HEX);
  Serial.print("Flash Size: ");
  Serial.println(ESP.getFlashChipSize());
  Serial.print("MAC Address: ");
  Serial.println(WiFi.macAddress());
  Serial.println();
  
  // Setup Access Point
  WiFi.mode(WIFI_AP);
  Serial.print("Creating Access Point");
  
  bool result = WiFi.softAP(ap_ssid, ap_password);
  
  // Give it a moment to initialize
  delay(1000);
  
  Serial.println();
  
  if (result) {
    Serial.println("\n✓ Access Point created successfully!\n");
    Serial.println("Access Point Information:");
    Serial.println("------------------------");
    Serial.print("Network Name (SSID): ");
    Serial.println(ap_ssid);
    Serial.print("Password: ");
    Serial.println(ap_password);
    Serial.print("IP Address: ");
    Serial.println(WiFi.softAPIP());
    Serial.print("MAC Address: ");
    Serial.println(WiFi.softAPmacAddress());
    Serial.println();
    Serial.println("How to connect:");
    Serial.println("1. On your phone/computer, look for WiFi network: " + String(ap_ssid));
    Serial.println("2. Connect using password: " + String(ap_password));
    Serial.println("3. Open a web browser and go to: http://" + WiFi.softAPIP().toString());
    Serial.println("   (Note: No web server is running in this test, you'll get 'connection refused')");
  } else {
    Serial.println("\n✗ Failed to create Access Point!");
    Serial.println("\nTroubleshooting:");
    Serial.println("1. Try a different SSID name");
    Serial.println("2. Ensure password is at least 8 characters");
    Serial.println("3. Reset the ESP8266 and try again");
    Serial.println("4. Check if ESP8266 has enough power (use quality USB cable)");
  }
}

void loop() {
  // Monitor Access Point status and connected clients
  static unsigned long lastCheck = 0;
  static int lastClientCount = -1;
  
  if (millis() - lastCheck > 5000) {
    int clientCount = WiFi.softAPgetStationNum();
    
    // Report status every 5 seconds
    Serial.println("\n--- Access Point Status ---");
    Serial.print("Time running: ");
    Serial.print(millis() / 1000);
    Serial.println(" seconds");
    
    Serial.print("Connected clients: ");
    Serial.println(clientCount);
    
    if (clientCount != lastClientCount) {
      if (clientCount > lastClientCount && clientCount > 0) {
        Serial.println("✓ New client connected!");
      } else if (clientCount < lastClientCount) {
        Serial.println("✗ Client disconnected!");
      }
      lastClientCount = clientCount;
    }
    
    // Show network info
    Serial.print("Access Point IP: ");
    Serial.println(WiFi.softAPIP());
    Serial.print("Network: ");
    Serial.print(ap_ssid);
    Serial.print(" (Password: ");
    Serial.print(ap_password);
    Serial.println(")");
    
    Serial.println("---------------------------");
    
    lastCheck = millis();
  }
  
  delay(100);
}
