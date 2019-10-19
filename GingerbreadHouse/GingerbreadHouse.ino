#include <Adafruit_NeoPixel.h>
#include <WiFiNINA.h>
#include <PubSubClient.h>
#include "Credentials.h"

#define LED_PIN 1
#define LED_COUNT 30

Adafruit_NeoPixel leds(LED_COUNT, LED_PIN);
WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

enum lightColor {
  OFF,
  RED,
  GREEN,
  BLUE
};

lightColor lights = OFF;

void setup() {
  setupWifi();
  setupMqtt();
  setupLeds();
}

void setupWifi() {
  delay(50);
  WiFi.begin(wifiSsid.c_str(), wifiPassword.c_str());

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
  }
}

void setupMqtt() {
  mqttClient.setServer(mqttServer.c_str(), mqttServerPort);
  mqttClient.setCallback(mqttCallback);
}

void setupLeds() {
  leds.begin();
}

void mqttCallback(char* topic, byte* payload, unsigned int length) {
  if (strcmp(topic, mqttTopic.c_str()) == 0) {
    handleCallback(payload);
  }
}

void handleCallback(byte* payload) {
  switch((char)payload[0]) {
    case '0':
      lights = OFF;
      break;
    case '1':
      lights = RED;
      break;
    case '2':
      lights = GREEN;
      break;
    case '3':
      lights = BLUE;
      break;
  }
}

void mqttReconnect() {
  while (!mqttClient.connected()) {
    if (mqttClient.connect(mqttClientDescription.c_str(), mqttUsername.c_str(), mqttPassword.c_str())) {
      mqttClient.subscribe(mqttTopic.c_str());
    } else {
      delay(5000);
    }
  }
}

void loop() {
  loopMqtt();
  loopLeds();
}

void loopLeds() {
  if (lights == RED) {
    changeLights(10, 0, 0);
  } else if (lights == GREEN) {
    changeLights(0, 10, 0);
  } else if (lights == BLUE) {
    changeLights(0, 0, 10);
  } else {
    changeLights(0, 0, 0);
  }
  delay(100);
}

void changeLights(int r, int g, int b) {
  for (int i = 0; i < 30; i++) {
    leds.setPixelColor(i, r, g, b);
  }

  leds.show();
}

void loopMqtt() {
  if (!mqttClient.connected()) {
    mqttReconnect();
  }

  mqttClient.loop();
}
