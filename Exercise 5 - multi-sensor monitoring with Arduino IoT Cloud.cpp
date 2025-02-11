// MADE BY: Danziel Cempron
// https://github.com/Dnzldotexe/pembeds-arduino-exercises/

// CODE STARTS HERE
#include "thingProperties.h"
#include <DHT.h>

// Define DHT sensor type and pin
#define DHTTYPE DHT11
#define DHTPIN 2

// Define water level sensor pin
#define WATER_SENSOR A0

// Initialize DHT sensor
DHT dht(DHTPIN, DHTTYPE);

// Timing variables
unsigned long lastTempRead = 0;
unsigned long lastWaterRead = 0;
const unsigned long TEMP_INTERVAL = 5000;  // 5 seconds
const unsigned long WATER_INTERVAL = 8000; // 8 seconds

const unsigned long ONE_DAY = 86400000; // 24 hours in milliseconds (24 * 60 * 60 * 1000)
unsigned long startTime = 0;  // Track starting time

void setup() {
  // Initialize serial and wait for port to open:
  Serial.begin(9600);
  // This delay gives the chance to wait for a Serial Monitor without blocking if none is found
  delay(1500);

  // Initialize DHT sensor
  dht.begin();

  // Defined in thingProperties.h
  initProperties();

  // Connect to Arduino IoT Cloud
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);
  
  setDebugMessageLevel(2);
  ArduinoCloud.printDebugInfo();
}

void loop() {
  ArduinoCloud.update();

  // Track time
  unsigned long currentTime = millis();

  // Check if 24 hours have passed
  if (currentTime - startTime >= ONE_DAY) {
    // Reset timer
    startTime = currentTime;
  }

  // Read temperature and humidity every 5 seconds
  if (currentTime - lastTempRead >= TEMP_INTERVAL) {
    // Read temperature and humidity from the DHT11 sensor
    float temp = dht.readTemperature();
    float humidity = dht.readHumidity();
    
    // Update IoT Cloud variables
    temperature_sensor = temp;
    humidity_sensor = humidity;

    // Store last temp reading
    lastTempRead = currentTime;
  }

  // Read water level every 8 seconds
  if (currentTime - lastWaterRead >= WATER_INTERVAL) {
    // Read analog water level output from the sensor 
    float water_level = analogRead(WATER_SENSOR);
    
    // Update IoT Cloud variables
    water_level_sensor = water_level;

    // Store last water reading
    lastWaterRead = currentTime;
  }

  // Let the sensor rest for a bit :)
  delay(100);
}
