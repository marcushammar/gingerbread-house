/*
 * Gingerbread House Controller Four
 * Copyright (C) 2018  Marcus Hammar
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <WiFiNINA.h>
#include <PubSubClient.h>
#include <Servo.h>
#include "Credentials.h"

const int servoPin = 1;

WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);
Servo servo;

bool running = false;

void setup() {
  setupWifi();
  setupMqtt();
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

void setupServo() {
  servo.attach(servoPin);
}

void mqttCallback(char* topic, byte* payload, unsigned int length) {
  if (strcmp(topic, mqttTopic.c_str()) == 0) {
    handleCallback(payload);
  }
}

void handleCallback(byte* payload) {
  if ((char)payload[0] == '1') {
    running = true;
  } else {
    running = false;
  }
}

void mqttReconnect() {
  while (!mqttClient.connected()) {
    if (mqttClient.connect(mqttClientDescription.c_str(), mqttUsername.c_str(), mqttPassword.c_str())) {
      mqttClient.subscribe(mqttTopic.c_str());
    } else {
      delay(10000);
    }
  }
}

void loop() {
  loopMqtt();
  loopServo();
}

void loopMqtt() {
  if (!mqttClient.connected()) {
    mqttReconnect();
  }

  mqttClient.loop();
}

void loopServo() {
  if (running) {
    servo.write(0);
  } else {
    servo.write(90);
  }
}
