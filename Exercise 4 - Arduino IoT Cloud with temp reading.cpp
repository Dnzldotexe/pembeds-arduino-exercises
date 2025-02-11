#include "thingProperties.h"
#include <DHT.h>

// Define DHT sensor type and pin
#define DHTTYPE DHT11
#define DHTPIN 2

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  // Initialize serial and wait for port to open:
  Serial.begin(9600);
  delay(1500); // Wait for serial monitor to connect

  // Initialize the DHT sensor
  dht.begin();
  
  // Initialize properties and IoT Cloud connection
  initProperties();
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);

  // Set debug message level (optional)
  setDebugMessageLevel(2);
  ArduinoCloud.printDebugInfo();
}

void loop() {
  ArduinoCloud.update();

  // Read temperature and humidity from the DHT11 sensor
  float temp = dht.readTemperature();
  float humidity = dht.readHumidity();

  // Check if the readings are valid
  if (isnan(temp) || isnan(humidity)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Update IoT Cloud variables
  temperature_sensor = temp;
  humidity_sensor = humidity;

  // Print to Serial Monitor for debugging
  Serial.print("Temperature: ");
  Serial.print(temp);
  Serial.println(" °C");
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.println(" %");
  
  delay(2000); // Wait 2 seconds before the next reading
}
