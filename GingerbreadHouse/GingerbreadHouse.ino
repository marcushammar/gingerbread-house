#include <Adafruit_NeoPixel.h>
#include <WiFiNINA.h>
#include <PubSubClient.h>
#include <Servo.h>
#include "Credentials.h"

#define LED_PIN 1
#define LED_COUNT 30
#define SERVO_ONE_PIN 4
#define SERVO_TWO_PIN 5
#define LED_POWER_ZERO 0
#define LED_POWER_HALF 25
#define LED_POWER_FULL 50

Adafruit_NeoPixel leds(LED_COUNT, LED_PIN);
Servo servoOne;
Servo servoTwo;
WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

enum lightColor {
  OFF,
  RED,
  GREEN,
  BLUE,
  YELLOW,
  PINK,
  TURQUOISE
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
  servoOne.attach(SERVO_ONE_PIN);
  servoTwo.attach(SERVO_TWO_PIN);
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
    case '5':
      lights = PINK;
      break;
    case '6':
      lights = TURQUOISE;
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
    changeLights(LED_POWER_FULL, LED_POWER_ZERO, LED_POWER_ZERO);
  } else if (lights == GREEN) {
    changeLights(LED_POWER_ZERO, LED_POWER_FULL, LED_POWER_ZERO);
  } else if (lights == BLUE) {
    changeLights(LED_POWER_ZERO, LED_POWER_ZERO, LED_POWER_FULL);
  } else if (lights == YELLOW) {
    changeLights(LED_POWER_HALF, LED_POWER_HALF, LED_POWER_ZERO);
  } else if (lights == PINK) {
    changeLights(LED_POWER_HALF, LED_POWER_ZERO, LED_POWER_HALF);
  } else if (lights == TURQUOISE) {
    changeLights(LED_POWER_ZERO, LED_POWER_HALF, LED_POWER_HALF);
  } else {
    changeLights(LED_POWER_ZERO, LED_POWER_ZERO, LED_POWER_ZERO);
  }
  delay(100);
}

void loopServo() {
  if (servoRunning) {
    servoOneRunning();
    servoTwoRunning();
  } else {
    servoOneNotRunning();
    servoTwoNotRunning();
  }
}

void servoOneRunning() {
  servoOne.write(0);
  delay(500);

  servoOne.write(180);
  delay(1000);

  servoOne.write(90);
  delay(500);
}

void servoOneNotRunning() {
  servoOne.write(90);
}

void servoTwoRunning() {
  servoTwo.write(0);
}

void servoTwoNotRunning() {
  servoTwo.write(90);
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
