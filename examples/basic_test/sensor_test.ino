/*
 * LM35 Temperature Sensor Test
 * 
 * This sketch tests the LM35 temperature sensor.
 * Use this to verify your sensor is properly connected and working
 * before uploading the full Filament Greenhouse code.
 * 
 * Hardware:
 * - LM35 sensor connected to A0 (analog input)
 * - VCC to 5V or 3.3V
 * - GND to GND
 * - Output to A0
 * 
 * LM35 outputs 10mV per degree Celsius (0.01V/°C)
 * Temperature range: 0°C to 100°C
 * 
 * Upload this sketch and open Serial Monitor at 115200 baud to see results.
 */

#define LM35PIN A0  // LM35 connected to A0 (analog input)

void setup() {
  Serial.begin(115200);
  delay(100);
  
  Serial.println("\n\n===========================");
  Serial.println("LM35 Temperature Sensor Test");
  Serial.println("===========================\n");
  
  Serial.println("Initializing LM35 sensor...");
  pinMode(LM35PIN, INPUT);
  delay(1000);  // Wait for sensor to stabilize
  
  Serial.println("Sensor initialized!");
  Serial.println("Reading sensor every 2 seconds...\n");
  Serial.println("ADC Value | Voltage  | Temperature | Status");
  Serial.println("-----------------------------------------------");
}

void loop() {
  // Wait 2 seconds between measurements
  delay(2000);
  
  // Read analog value from LM35
  int adcValue = analogRead(LM35PIN);
  
  // Convert ADC value to voltage
  // ESP8266 ADC: 0-1023 corresponds to 0-1V (10-bit ADC)
  float voltage = (adcValue / 1023.0) * 1.0;
  
  // Convert voltage to temperature
  // LM35: 10mV per °C = 0.01V per °C
  // Therefore: Temperature (°C) = Voltage (V) * 100
  float tempC = voltage * 100.0;
  
  // Temperature in Fahrenheit
  float tempF = (tempC * 9.0 / 5.0) + 32.0;
  
  // Check if reading is valid
  if (tempC < -10 || tempC > 150) {
    Serial.println("✗ Invalid temperature reading!");
    Serial.println("  Check connections:");
    Serial.println("  - LM35 VCC to 3.3V or 5V");
    Serial.println("  - LM35 GND to GND");
    Serial.println("  - LM35 Output to A0");
    Serial.println("  - Ensure sensor is not damaged");
    Serial.println();
    return;
  }
  
  // Print results
  Serial.print(adcValue);
  Serial.print(" | ");
  Serial.print(voltage, 3);
  Serial.print("V | ");
  Serial.print(tempC, 1);
  Serial.print("°C");
  Serial.print(" | ");
  
  // Add status indicators
  if (tempC < 0 || tempC > 100) {
    Serial.println("⚠ Temperature out of normal LM35 range (0-100°C)");
  } else if (tempC < 15 || tempC > 40) {
    Serial.println("⚠ Temperature outside typical room range");
  } else {
    Serial.println("✓ OK");
  }
  
  // Print additional info every 10 readings
  static int readingCount = 0;
  readingCount++;
  
  if (readingCount % 10 == 0) {
    Serial.println("\n--- Extended Information ---");
    Serial.print("Temperature (F): ");
    Serial.print(tempF, 1);
    Serial.println("°F");
    Serial.print("ADC Resolution: 10-bit (0-1023)");
    Serial.println();
    Serial.print("Voltage Range: 0-1V");
    Serial.println();
    Serial.println("----------------------------\n");
  }
}

/*
 * Expected Results:
 * -----------------
 * At room temperature (~20-25°C), you should see:
 * - ADC Value: ~200-250
 * - Voltage: ~0.20-0.25V
 * - Temperature: ~20-25°C
 * 
 * Troubleshooting:
 * ----------------
 * If readings are incorrect:
 * - Verify VCC is connected (3.3V or 5V)
 * - Check GND connection
 * - Ensure Output pin is connected to A0
 * - LM35 flat side should face you with pins down:
 *   Pin 1 (left): VCC
 *   Pin 2 (middle): Output (to A0)
 *   Pin 3 (right): GND
 * 
 * If temperature seems too high or too low:
 * - Check if sensor is getting hot (possible short circuit)
 * - Verify correct pin connections
 * - Try touching sensor gently - temp should increase
 * 
 * Notes:
 * ------
 * - LM35 is more accurate than DHT22 for temperature
 * - No humidity sensing capability
 * - Direct analog output, no digital protocol needed
 * - ±0.5°C accuracy at 25°C
 * - Linear scale: 10mV/°C
 */
