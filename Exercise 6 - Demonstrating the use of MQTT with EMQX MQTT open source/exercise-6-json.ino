#include "WiFiS3.h"
#include <ArduinoMqttClient.h>
#include <ArduinoJson.h>

char ssid[] = "hotspot_name"; // replace placeholder
char pass[] = "hotspot_pass"; // replace placeholder

char mqtt_user[] = "arduino";
char mqtt_pass[] = "pass";


WiFiClient wifiClient;
MqttClient mqttClient(wifiClient);

const char broker[] = "192.168.X.X"; // replace placeholder
int        port     = 1883;
const char subscribe_topic[]  = "/hello";
const char publish_topic[]  = "/hello/world";

void setup() {
  // Create serial connection and wait for it to become available.
  Serial.begin(9600);
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
  doc["time"] = millis();
  doc["sensor_a0"] = analogRead(0);
  char jsonBuffer[512];
  serializeJson(doc, jsonBuffer); // print to client

  mqttClient.beginMessage(publish_topic);
  mqttClient.print(jsonBuffer);
  mqttClient.endMessage();
}
