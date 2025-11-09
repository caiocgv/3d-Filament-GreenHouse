/*
 * WiFi Connection Test
 * 
 * This sketch tests basic WiFi connectivity for the ESP8266.
 * Use this to verify your ESP8266 can connect to your WiFi network
 * before uploading the full Filament Greenhouse code.
 * 
 * Upload this sketch and open Serial Monitor at 115200 baud to see results.
 */

#include <ESP8266WiFi.h>

// WiFi credentials - Update these with your network details
const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";

void setup() {
  Serial.begin(115200);
  delay(100);
  
  Serial.println("\n\n=============================");
  Serial.println("ESP8266 WiFi Connection Test");
  Serial.println("=============================\n");
  
  // Print ESP8266 chip information
  Serial.print("Chip ID: ");
  Serial.println(ESP.getChipId(), HEX);
  Serial.print("Flash Chip ID: ");
  Serial.println(ESP.getFlashChipId(), HEX);
  Serial.print("Flash Size: ");
  Serial.println(ESP.getFlashChipSize());
  Serial.println();
  
  // Start WiFi connection
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  
  Serial.print("Connecting to WiFi");
  int attempts = 0;
  
  while (WiFi.status() != WL_CONNECTED && attempts < 30) {
    delay(500);
    Serial.print(".");
    attempts++;
  }
  
  Serial.println();
  
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\n✓ WiFi connected successfully!\n");
    Serial.println("Network Information:");
    Serial.println("-------------------");
    Serial.print("SSID: ");
    Serial.println(WiFi.SSID());
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
    Serial.print("Subnet Mask: ");
    Serial.println(WiFi.subnetMask());
    Serial.print("Gateway: ");
    Serial.println(WiFi.gatewayIP());
    Serial.print("DNS: ");
    Serial.println(WiFi.dnsIP());
    Serial.print("MAC Address: ");
    Serial.println(WiFi.macAddress());
    Serial.print("Signal Strength (RSSI): ");
    Serial.print(WiFi.RSSI());
    Serial.println(" dBm");
  } else {
    Serial.println("\n✗ Failed to connect to WiFi!");
    Serial.println("\nTroubleshooting:");
    Serial.println("1. Check SSID and password are correct");
    Serial.println("2. Ensure you're connecting to a 2.4GHz network (ESP8266 doesn't support 5GHz)");
    Serial.println("3. Move closer to the router");
    Serial.println("4. Check if MAC filtering is enabled on your router");
  }
}

void loop() {
  // Monitor connection status
  static unsigned long lastCheck = 0;
  static bool wasConnected = false;
  
  if (millis() - lastCheck > 5000) {
    bool isConnected = (WiFi.status() == WL_CONNECTED);
    
    if (isConnected && !wasConnected) {
      Serial.println("\n✓ WiFi reconnected!");
      Serial.print("IP: ");
      Serial.println(WiFi.localIP());
    } else if (!isConnected && wasConnected) {
      Serial.println("\n✗ WiFi disconnected!");
    }
    
    if (isConnected) {
      Serial.print("Connected | RSSI: ");
      Serial.print(WiFi.RSSI());
      Serial.println(" dBm");
    }
    
    wasConnected = isConnected;
    lastCheck = millis();
  }
  
  delay(100);
}
