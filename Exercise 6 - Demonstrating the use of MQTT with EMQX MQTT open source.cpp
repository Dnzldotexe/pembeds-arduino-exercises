// MADE BY: Danziel Cempron
// https://github.com/Dnzldotexe/pembeds-arduino-exercises/

// CODE STARTS HERE
#include "WiFiS3.h"
#include <ArduinoMqttClient.h>
#include <ArduinoJson.h>
#include <DHT.h>

// Define sensors type and pin
#define DHTTYPE DHT11
#define DHTPIN 2
#define WATER_SENSOR A0

// Initialize DHT sensor
DHT dht(DHTPIN, DHTTYPE);

char ssid[] = "my-hotpot";    // your network SSID (name)
char pass[] = "hai-di-lao";    // your network password 

char mqtt_user[] = "arduino";
char mqtt_pass[] = "pass";


WiFiClient wifiClient;
MqttClient mqttClient(wifiClient);

const char broker[] = "192.168.XXX.XXX"; //IP address of the EMQX broker.
int        port     = 1883;
const char subscribe_topic[]  = "/hello";
const char publish_topic[]  = "/hello/world";

void setup() {
  // Create serial connection and wait for it to become available.
  Serial.begin(9600);
  dht.begin();
  while (!Serial) {
    ; 
  }

  // Connect to WiFi
  Serial.print("Attempting to connect to WPA SSID: ");
  Serial.println(ssid);
  while (WiFi.begin(ssid, pass) != WL_CONNECTED) {
    // failed, retry
    Serial.print(".");
    delay(5000);
  }

  Serial.println("You're connected to the network");
  Serial.println();

  // You can provide a username and password for authentication
  mqttClient.setUsernamePassword(mqtt_user, mqtt_pass);

  Serial.print("Attempting to connect to the MQTT broker.");

  if (!mqttClient.connect(broker, port)) {
    Serial.print("MQTT connection failed! Error code = ");
    Serial.println(mqttClient.connectError());

    while (1);
  }

  Serial.println("You're connected to the MQTT broker!");

  mqttClient.onMessage(onMqttMessage);

  Serial.print("Subscribing to topic: ");
  Serial.println(subscribe_topic);

  // subscribe to a topic
  mqttClient.subscribe(subscribe_topic);

  // topics can be unsubscribed using:
  // mqttClient.unsubscribe(topic);

  Serial.print("Waiting for messages on topic: ");
  Serial.println(subscribe_topic);
}

void loop() {
  // call poll() regularly to allow the library to receive MQTT messages and
  // send MQTT keep alives which avoids being disconnected by the broker
  mqttClient.poll();

  // send message, the Print interface can be used to set the message contents
  delay(3000);
  publishMessage();

}

void onMqttMessage(int messageSize) {
  Serial.print("Received a message with topic '");
  Serial.println(mqttClient.messageTopic());
  StaticJsonDocument<256> doc;
  deserializeJson(doc, mqttClient);
  const char* message = doc["message"];
  Serial.print("Message: '");
  Serial.println(message);
}

void publishMessage()
{
  StaticJsonDocument<200> doc;
  
  float temperature = dht.readTemperature(); // Read temperature in Celsius
  float humidity = dht.readHumidity();       // Read humidity
  int water = analogRead(WATER_SENSOR);      // Read water sensor value

  // Check if any readings failed and skip them
  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Add sensor readings to the JSON document
  doc["time"] = millis();
  doc["temperature"] = temperature;
  doc["humidity"] = humidity;
  doc["water_level"] = water;

  char jsonBuffer[512];
  serializeJson(doc, jsonBuffer); // Convert JSON to buffer

  Serial.print("Publishing message: ");
  Serial.println(jsonBuffer); // Optional debug log to Serial Monitor

  mqttClient.beginMessage(publish_topic);
  mqttClient.print(jsonBuffer);
  mqttClient.endMessage();
}
