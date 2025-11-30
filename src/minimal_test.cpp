/*
 * Minimal ESP8266 Test - NO SERIAL
 * Tests if board runs independently of serial monitor
 * 
 * IMPORTANT: This version does NOT initialize Serial at all
 * to avoid any DTR/RTS reset issues
 */

#include <Arduino.h>

void setup() {
  // DO NOT initialize Serial - this might be causing the issue!
  // Serial.begin(9600);  // COMMENTED OUT
  
  // Just initialize LED
  pinMode(LED_BUILTIN, OUTPUT);
  
  // Flash LED 5 times rapidly to show setup completed
  for (int i = 0; i < 5; i++) {
    digitalWrite(LED_BUILTIN, LOW);   // ON
    delay(100);
    digitalWrite(LED_BUILTIN, HIGH);  // OFF
    delay(100);
  }
  
  delay(1000);  // Wait before starting main loop
}

void loop() {
  // Slow blink - visible even without serial monitor
  // NO Serial.print calls at all
  
  digitalWrite(LED_BUILTIN, LOW);   // LED ON (active LOW)
  delay(1000);
  
  digitalWrite(LED_BUILTIN, HIGH);  // LED OFF (active LOW)
  delay(1000);
}
