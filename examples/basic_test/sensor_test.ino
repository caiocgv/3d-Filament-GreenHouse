/*
 * DHT22 Sensor Test
 * 
 * This sketch tests the DHT22 temperature and humidity sensor.
 * Use this to verify your sensor is properly connected and working
 * before uploading the full Filament Greenhouse code.
 * 
 * Hardware:
 * - DHT22 sensor connected to GPIO2 (D4 on NodeMCU)
 * - 10k pull-up resistor from data pin to VCC (3.3V)
 * - VCC to 3.3V
 * - GND to GND
 * 
 * Upload this sketch and open Serial Monitor at 115200 baud to see results.
 */

#include <DHT.h>

#define DHTPIN 2        // DHT22 connected to GPIO2 (D4)
#define DHTTYPE DHT22   // DHT 22 (AM2302)

// Initialize DHT sensor
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  delay(100);
  
  Serial.println("\n\n===========================");
  Serial.println("DHT22 Sensor Test");
  Serial.println("===========================\n");
  
  Serial.println("Initializing DHT22 sensor...");
  dht.begin();
  delay(2000);  // Wait for sensor to stabilize
  
  Serial.println("Sensor initialized!");
  Serial.println("Reading sensor every 2 seconds...\n");
  Serial.println("Temperature | Humidity | Heat Index | Status");
  Serial.println("-----------------------------------------------");
}

void loop() {
  // Wait 2 seconds between measurements
  delay(2000);
  
  // Read humidity
  float humidity = dht.readHumidity();
  
  // Read temperature as Celsius
  float tempC = dht.readTemperature();
  
  // Read temperature as Fahrenheit
  float tempF = dht.readTemperature(true);
  
  // Check if any reads failed
  if (isnan(humidity) || isnan(tempC) || isnan(tempF)) {
    Serial.println("✗ Failed to read from DHT sensor!");
    Serial.println("  Check connections:");
    Serial.println("  - DHT22 VCC to 3.3V");
    Serial.println("  - DHT22 GND to GND");
    Serial.println("  - DHT22 Data to GPIO2 (D4)");
    Serial.println("  - 10k pull-up resistor from Data to 3.3V");
    Serial.println();
    return;
  }
  
  // Compute heat index in Celsius
  float heatIndexC = dht.computeHeatIndex(tempC, humidity, false);
  
  // Print results
  Serial.print(tempC, 1);
  Serial.print("°C");
  Serial.print(" | ");
  Serial.print(humidity, 1);
  Serial.print("%");
  Serial.print(" | ");
  Serial.print(heatIndexC, 1);
  Serial.print("°C");
  Serial.print(" | ");
  
  // Add status indicators
  if (tempC < 0 || tempC > 80) {
    Serial.println("⚠ Temperature out of normal range");
  } else if (humidity < 20 || humidity > 80) {
    Serial.println("⚠ Humidity out of normal range");
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
    Serial.print("Heat Index (F): ");
    Serial.println(dht.computeHeatIndex(tempF, humidity), 1);
    Serial.println("----------------------------\n");
  }
}
