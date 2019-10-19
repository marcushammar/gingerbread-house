#include <Adafruit_NeoPixel.h>
#include <WiFiNINA.h>
#include <PubSubClient.h>
#include <Servo.h>
#include "Credentials.h"

#define LED_PIN 1
#define LED_COUNT 30
#define SERVO_PIN 5
#define LED_POWER 50
#define LED_POWER_HALF 25

Adafruit_NeoPixel leds(LED_COUNT, LED_PIN);
Servo servo;
WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

enum lightColor {
  OFF,
  RED,
  GREEN,
  BLUE,
  YELLOW
};

lightColor lights = OFF;
bool servoRunning = false;

void setup() {
  setupWifi();
  setupMqtt();
  setupLeds();
  setupServo();
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

void setupServo() {
  servo.attach(SERVO_PIN);
}

void mqttCallback(char* topic, byte* payload, unsigned int length) {
  if (strcmp(topic, mqttTopicLights.c_str()) == 0) {
    handleCallbackLights(payload);
  }
  if (strcmp(topic, mqttTopicServo.c_str()) == 0) {
    handleCallbackServo(payload);
  }
}

void handleCallbackLights(byte* payload) {
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
    case '4':
      lights = YELLOW;
      break;
  }
}

void handleCallbackServo(byte* payload) {
  if ((char)payload[0] == '1') {
    servoRunning = true;
  } else {
    servoRunning = false;
  }
}

void mqttReconnect() {
  while (!mqttClient.connected()) {
    if (mqttClient.connect(mqttClientDescription.c_str(), mqttUsername.c_str(), mqttPassword.c_str())) {
      mqttClient.subscribe(mqttTopicLights.c_str());
      mqttClient.subscribe(mqttTopicServo.c_str());
    } else {
      delay(5000);
    }
  }
}

void loop() {
  loopMqtt();
  loopLeds();
  loopServo();
}

void loopLeds() {
  if (lights == RED) {
    changeLights(LED_POWER, 0, 0);
  } else if (lights == GREEN) {
    changeLights(0, LED_POWER, 0);
  } else if (lights == BLUE) {
    changeLights(0, 0, LED_POWER);
  } else if (lights == YELLOW) {
    changeLights(LED_POWER_HALF, LED_POWER_HALF, 0);
  } else {
    changeLights(0, 0, 0);
  }
  delay(100);
}

void loopServo() {
  if (servoRunning) {
    servo.write(0);
  } else {
    servo.write(90);
  }
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
