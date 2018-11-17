/*
 * Gingerbread House Controller Three
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
#include "Credentials.h"

const int mqttTopic1Pin = 1;
const int mqttTopic2Pin = 2;

WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

void setup() {
  pinMode(mqttTopic1Pin, OUTPUT);
  pinMode(mqttTopic2Pin, OUTPUT);
  setupWifi();
  setupMqtt();
}

void setupWifi() {
  delay(50);
  WiFi.begin(wifiSsid.c_str(), wifiPassword.c_str());

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
  }
}

void setupMqtt() {
  mqttClient.setServer(mqttServer.c_str(), 1883);
  mqttClient.setCallback(mqttCallback);
}

void mqttCallback(char* topic, byte* payload, unsigned int length) {
  if (strcmp(topic, mqttTopic1.c_str()) == 0) {
    handleCallbackTopic1(payload);
  }

  if (strcmp(topic, mqttTopic2.c_str()) == 0) {
    handleCallbackTopic2(payload);
  }
}

void handleCallbackTopic1(byte* payload) {
  if ((char)payload[0] == '1') {
    digitalWrite(mqttTopic1Pin, HIGH);
  } else {
    digitalWrite(mqttTopic1Pin, LOW);
  }
}

void handleCallbackTopic2(byte* payload) {
  if ((char)payload[0] == '1') {
    digitalWrite(mqttTopic2Pin, HIGH);
  } else {
    digitalWrite(mqttTopic2Pin, LOW);
  }
}

void mqttReconnect() {
  while (!mqttClient.connected()) {
    if (mqttClient.connect(mqttClientDescription.c_str(), mqttUsername.c_str(), mqttPassword.c_str())) {
      mqttClient.subscribe(mqttTopic1.c_str());
      mqttClient.subscribe(mqttTopic2.c_str());
    } else {
      delay(10000);
    }
  }
}

void loop() {
  if (!mqttClient.connected()) {
    mqttReconnect();
  }

  mqttClient.loop();
}
