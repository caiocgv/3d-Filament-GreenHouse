/*
 * Relay Test - LED Status Indication (No Serial)
 * 
 * Tests the relay module using onboard LED for status feedback.
 * No Serial communication - avoids DTR/RTS reset issues.
 * 
 * Hardware:
 * - Relay module connected to GPIO4 (D2 on NodeMCU)
 * - Relay VCC to 5V
 * - Relay GND to GND
 * 
 * LED Status Indicators:
 * - 5 rapid flashes on boot = Setup complete
 * - LED ON (solid) = Relay ON
 * - LED OFF = Relay OFF
 * - Toggles every 2 seconds
 * 
 * What to Check:
 * - Onboard LED should blink with 2-second intervals
 * - Relay should click in sync with LED
 * - Works WITHOUT opening serial monitor
 * 
 * SAFETY: Do NOT connect high voltage to the relay during testing.
 *         Test with relay disconnected from heater/mains power.
 */

#include <Arduino.h>

#define RELAYPIN 4  // Relay connected to GPIO4 (D2)
#define LED_PIN LED_BUILTIN  // Onboard LED to show relay status

void setup() {
  // NO Serial communication - pure LED-based status indication
  
  // Initialize onboard LED first
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);  // LED OFF (most ESP8266 LEDs are active LOW)
  
  // Visual startup sequence - 5 rapid flashes = "Setup Complete"
  // This confirms the board booted and entered setup()
  for (int i = 0; i < 5; i++) {
    digitalWrite(LED_PIN, LOW);   // LED ON
    delay(100);
    digitalWrite(LED_PIN, HIGH);  // LED OFF
    delay(100);
  }
}

void loop() {
  // Relay ON - enable pin as OUTPUT and set HIGH
  pinMode(RELAYPIN, OUTPUT);
  digitalWrite(RELAYPIN, HIGH);
  digitalWrite(LED_PIN, LOW);  // LED ON
  delay(2000);
  
  // Relay OFF - disable pin by setting to INPUT (floating/high-impedance)
  pinMode(RELAYPIN, INPUT);
  digitalWrite(LED_PIN, HIGH);  // LED OFF
  delay(2000);
}

/*
 * Expected Results (LED-Based Status):
 * ------------------------------------
 * 1. On boot: 5 rapid LED flashes (setup complete indicator)
 * 2. Onboard LED blinks: 2 seconds ON, 2 seconds OFF
 * 3. Relay clicks in sync with LED changes
 * 4. Works WITHOUT serial monitor open
 * 
 * LED Status Key:
 * - LED ON (solid) = Relay energized
 * - LED OFF = Relay de-energized
 * - 5 rapid flashes = Board booted successfully
 * 
 * Troubleshooting:
 * ----------------
 * LED doesn't blink at all:
 * - Board not running (power issue)
 * - Try external 5V power supply
 * 
 * LED blinks but relay doesn't click:
 * - Verify relay VCC is connected to 5V (NOT 3.3V)
 * - Verify relay GND is connected to common ground
 * - Check relay signal pin is connected to GPIO4 (D2)
 * - Measure voltage on GPIO4: ~3.3V when LED ON, ~0V when LED OFF
 * 
 * Relay clicks but doesn't switch contacts:
 * - Relay may be faulty
 * - Check relay coil voltage rating (should be 5V)
 * - Try a different relay module
 */
