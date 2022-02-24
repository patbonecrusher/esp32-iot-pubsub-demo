/**
 * Blink
 *
 * Turns on an LED on for one second,
 * then off for one second, repeatedly.
 */
#include "Arduino.h"
#include <math.h>

#include <WiFiClientSecure.h>
#include <MQTTClient.h>
#include <ArduinoJson.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include "secrets.h"
#include "wserver.h"

#ifndef LED_BUILTIN
#define LED_BUILTIN 13
#endif

const int ledPin = 5;

// setting PWM properties
const int freq = 5000;
const int ledChannel = 0;
const int resolution = 8;

const int ledChannel2 = 0;


#define AWS_IOT_PUBLISH_TOPIC   "test/dc/subtopic"
#define AWS_IOT_SUBSCRIBE_TOPIC "test/dc/pubtopic"

void messageHandler(String &topic, String &payload) {
  Serial.print("BOOBAA");
  Serial.println("incoming: " + topic + " - " + payload);

//  StaticJsonDocument<200> doc;
//  deserializeJson(doc, payload);
//  const char* message = doc["message"];
}

unsigned long lastMillis = 0;

WiFiClientSecure net;
MQTTClient client;

void setup_led() {
  // configure LED PWM functionalitites
  ledcSetup(ledChannel, freq, resolution);
  ledcSetup(ledChannel2, freq, resolution);

  // attach the channel to the GPIO to be controlled
  ledcAttachPin(ledPin, ledChannel);
  ledcAttachPin(LED_BUILTIN, ledChannel);
  Serial.println("");
}

void setup_wifi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.println("");
}

void setup_mqtt() {

  net.setCACert(AWS_CERT_CA);
  net.setCertificate(AWS_CERT_CRT);
  net.setPrivateKey(AWS_CERT_PRIVATE);

  client.begin(AWS_IOT_ENDPOINT, 8883, net);
  client.onMessage(messageHandler);

  Serial.println("");
}

void connect() {
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(WIFI_SSID);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("gadget")) {
    Serial.println("MDNS responder started");
  }

  Serial.print("Connecting to AWS IOT\n");

  while (!client.connect(THINGNAME)) {
    Serial.print(".");
    delay(100);
  }

  if(!client.connected()){
    Serial.println("AWS IoT Timeout!");
    return;
  }

  client.subscribe(AWS_IOT_SUBSCRIBE_TOPIC);

  Serial.println("AWS IoT Connected!");

}

void setup() {
  Serial.begin(115200);

  setup_led();
  setup_wifi();
  setup_mqtt();
  setup_web_server();
  connect();
}

void publishMessage()
{
  StaticJsonDocument<200> doc;
  JsonObject root = doc.to<JsonObject>();
  JsonObject state = root.createNestedObject("state");
  JsonObject reported = state.createNestedObject("reported");
  reported["moisture"] = String(esp_random() % 901); //"123";
  reported["temp"] = "234";

  char jsonBuffer[512];
  serializeJson(root, jsonBuffer); // print to client
  Serial.println(jsonBuffer);

  client.publish(AWS_IOT_PUBLISH_TOPIC, jsonBuffer);
}

void loop(){
  client.loop();
  delay(10);

  if (!client.connected()) {
    connect();
  }

	float val = (exp(sin(millis()/2000.0*PI)) - 0.36787944)*108.0;
	//a
	ledcWrite(ledChannel, val);
	ledcWrite(ledChannel2, val);

  if (millis() - lastMillis > 60*1000) {
    lastMillis = millis();
    publishMessage();
	}

  ws_loop();
}