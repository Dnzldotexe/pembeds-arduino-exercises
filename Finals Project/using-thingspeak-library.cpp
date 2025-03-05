// MADE BY: Danziel Cempron
// https://github.com/Dnzldotexe/pembeds-arduino-exercises/
// https://www.halvorsen.blog/documents/technology/iot/arduino/resources/Arduino%20UNO%20R4%20WiFi%20and%20ThingSpeak.pdf

// Import libraries and files
#include <WiFiS3.h>
#include <DHT.h>
#include "ThingSpeak.h"
#include "secrets.h"

// Define sensors type and pin
#define DHTTYPE DHT11
#define DHTPIN 2
#define WATER_SENSOR_PIN A0

// Define time constants (in milliseconds)
const unsigned long TEMP_READ_INTERVAL = 5000;  // 5 seconds
const unsigned long WATER_READ_INTERVAL = 10000; // 10 seconds

// Function prototypes
void connectToWifi();
void printNetworkInfo();
void readTemperatureAndHumidity();
void readWaterLevel();
void thingSpeakWrite(int channelField, float channelValue);

// Instantiate sensors
DHT dht(DHTPIN, DHTTYPE);
WiFiClient client;

// Wifi credentials from secrets.h
char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASS;
int wifiStatus = WL_IDLE_STATUS;

// Track last sensor read times
unsigned long lastTemperatureReadTime = 0;
unsigned long lastWaterLevelReadTime = 0;

void setup() {
    // Initialize serial communication
    Serial.begin(9600);
    while (!Serial) {
        ; // Wait for serial port to connect
    }
    
    Serial.println("Starting sensor monitoring system...");
    
    // Connect to wifi
    connectToWifi();
    
    // Initialize ThingSpeak
    ThingSpeak.begin(client);
    
    // Initialize DHT sensor
    dht.begin();
    
    Serial.println("System initialized successfully");
}

void loop() {
    // Track time
    unsigned long currentTime = millis();

    // Read temperature and humidity at specified interval
    if (currentTime - lastTemperatureReadTime >= TEMP_READ_INTERVAL) {
        readTemperatureAndHumidity();
        lastTemperatureReadTime = currentTime;
    }

    // Read water level at specified interval
    if (currentTime - lastWaterLevelReadTime >= WATER_READ_INTERVAL) {
        readWaterLevel();
        lastWaterLevelReadTime = currentTime;
    }
}

void readTemperatureAndHumidity() {
    float temperature = dht.readTemperature();
    float humidity = dht.readHumidity();

    // Check if any readings failed
    if (isnan(temperature) || isnan(humidity)) {
        Serial.println("Failed to read from DHT sensor!");
        return;
    }

    // Print sensor values
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.print("°C, Humidity: ");
    Serial.print(humidity);
    Serial.println("%");
    
    // Send data to ThingSpeak
    thingSpeakWrite(1, temperature);
    thingSpeakWrite(2, humidity);
}

void readWaterLevel() {
    int waterLevel = analogRead(WATER_SENSOR_PIN); // Read water sensor value
    
    Serial.print("Water Level: ");
    Serial.println(waterLevel);
    
    thingSpeakWrite(3, waterLevel);
}

void thingSpeakWrite(int channelField, float channelValue) {
    unsigned long myChannelNumber = SECRET_CH_ID;
    const char *myWriteAPIKey = SECRET_WRITE_APIKEY;
    
    int x = ThingSpeak.writeField(myChannelNumber, channelField, channelValue, myWriteAPIKey);
    
    if (x == 0)
    {
        Serial.println("Channel updated successfully.");
    }
    else
    {
        Serial.println("Problem updating channel. HTTP error code " + String(x));
    }
}

void connectToWifi() {
    Serial.println("Initializing WiFi connection...");
    
    // Check for the WiFi module
    if (WiFi.status() == WL_NO_MODULE) {
        Serial.println("Communication with WiFi module failed!");
        while (true)
            ; // Don't continue
    }

    // Check firmware version
    String firmwareVersion = WiFi.firmwareVersion();
    if (firmwareVersion < WIFI_FIRMWARE_LATEST_VERSION) {
        Serial.println("Please upgrade the firmware");
    }

    // Attempt to connect to WiFi network
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    
    while (wifiStatus != WL_CONNECTED) {
        // Connect to WPA/WPA2 network
        wifiStatus = WiFi.begin(ssid, pass);
        
        if (wifiStatus != WL_CONNECTED) {
            Serial.println("Connection failed, retrying...");
            delay(5000); // Wait 5 seconds before retrying
        }
    }

    Serial.println("Connected to WiFi");
    printNetworkInfo();
}

void printNetworkInfo() {
    Serial.print("WiFi Status: ");
    Serial.println(WiFi.status());

    Serial.print("SSID: ");
    Serial.println(WiFi.SSID());

    IPAddress ip = WiFi.localIP();
    Serial.print("IP Address: ");
    Serial.println(ip);
}
