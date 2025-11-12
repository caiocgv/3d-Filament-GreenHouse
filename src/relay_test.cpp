/*
 * Relay Test
 * 
 * This sketch tests the relay module control.
 * Use this to verify your relay is properly connected and working
 * before uploading the full Filament Greenhouse code.
 * 
 * Hardware:
 * - Relay module connected to GPIO5 (D1 on NodeMCU)
 * - Relay VCC to 5V
 * - Relay GND to GND
 * 
 * SAFETY: Do NOT connect high voltage to the relay during testing.
 *         Test with relay disconnected from heater/mains power.
 * 
 * Upload this sketch and open Serial Monitor at 115200 baud to see results.
 * You should hear the relay click on/off every 2 seconds.
 */

#include <Arduino.h>

#define RELAYPIN 5  // Relay connected to GPIO5 (D1)

void setup() {
  Serial.begin(115200);
  delay(100);
  
  Serial.println("\n\n=========================");
  Serial.println("Relay Module Test");
  Serial.println("=========================\n");
  
  // Initialize relay pin
  pinMode(RELAYPIN, OUTPUT);
  digitalWrite(RELAYPIN, LOW);  // Start with relay OFF
  
  Serial.println("Relay pin initialized (GPIO5/D1)");
  Serial.println("\nRelay will toggle every 2 seconds");
  Serial.println("You should hear clicking sounds\n");
  Serial.println("⚠ SAFETY WARNING:");
  Serial.println("Do NOT connect high voltage during testing!");
  Serial.println("Test with relay disconnected from heater\n");
  Serial.println("Time | Relay State | Notes");
  Serial.println("-----------------------------------------------");
  
  delay(2000);
}

void loop() {
  static unsigned long lastToggle = 0;
  static bool relayState = false;
  static int cycleCount = 0;
  
  // Toggle relay every 2 seconds
  if (millis() - lastToggle >= 2000) {
    relayState = !relayState;
    digitalWrite(RELAYPIN, relayState ? HIGH : LOW);
    
    cycleCount++;
    
    // Print status
    unsigned long seconds = millis() / 1000;
    Serial.print(seconds);
    Serial.print("s | ");
    Serial.print(relayState ? "ON  (HIGH)" : "OFF (LOW) ");
    Serial.print(" | ");
    
    if (relayState) {
      Serial.println("Should hear CLICK");
    } else {
      Serial.println("Should hear CLICK");
    }
    
    // Every 10 cycles, provide additional info
    if (cycleCount % 10 == 0) {
      Serial.println("\n--- Test Progress ---");
      Serial.print("Cycles completed: ");
      Serial.println(cycleCount);
      Serial.println("Relay appears to be functioning correctly");
      Serial.println("Check:");
      Serial.println("  ✓ Relay LED indicator changes state");
      Serial.println("  ✓ Audible click sound when switching");
      Serial.println("  ✓ Contacts switch (if measuring with multimeter)");
      Serial.println("--------------------\n");
    }
    
    lastToggle = millis();
  }
  
  delay(10);  // Small delay to prevent excessive CPU usage
}

/*
 * Expected Results:
 * -----------------
 * 1. You should see the relay state change in Serial Monitor every 2 seconds
 * 2. You should HEAR a clicking sound from the relay when it switches
 * 3. If your relay has an LED indicator, it should light up when ON
 * 4. If measuring with a multimeter across COM and NO:
 *    - Should show continuity when relay is ON
 *    - Should show no continuity when relay is OFF
 * 
 * Troubleshooting:
 * ----------------
 * If relay doesn't click:
 * - Verify relay VCC is connected to 5V (NOT 3.3V)
 * - Verify relay GND is connected to common ground
 * - Check relay signal pin is connected to GPIO5
 * - Some relays are active LOW, try inverting the logic
 * - Measure voltage on signal pin (should be ~3.3V when HIGH)
 * 
 * If relay clicks but doesn't switch:
 * - Relay may be faulty
 * - Check relay coil voltage rating (should be 5V)
 * - Try a different relay module
 * 
 * Active HIGH vs Active LOW:
 * --------------------------
 * Most relay modules are active HIGH (signal HIGH = relay ON)
 * Some are active LOW (signal LOW = relay ON)
 * If your relay behavior is inverted, modify the main code accordingly.
 */
