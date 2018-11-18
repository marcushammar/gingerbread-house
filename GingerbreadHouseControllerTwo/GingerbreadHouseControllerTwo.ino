/*
 * Gingerbread House Controller Two
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

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "Credentials.h"

// Declaration of pins
int pinLedRed = 9;
int pinLedYellow = 10;

WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

void setup() {
  setupPins();
  setupWifi();
  setupMqtt();
  Serial.begin(9600);
}

void setupPins() {
  pinMode(pinLedRed, OUTPUT);
  pinMode(pinLedYellow, OUTPUT);
}

void setupWifi() {
  WiFi.mode(WIFI_STA);
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

void handleCallbackTopic1(byte* payload) {
  if ((char)payload[0] == '1') {
    digitalWrite(pinLedRed,HIGH);
    Serial.write('A');
  } else {
    digitalWrite(pinLedRed,LOW);
    Serial.write('B');
  }
}

void handleCallbackTopic2(byte* payload) {
  if ((char)payload[0] == '1') {
    digitalWrite(pinLedYellow,HIGH);
    Serial.write('C');
  } else {
    digitalWrite(pinLedYellow,LOW);
    Serial.write('D');
  }
}

void loop() {
  if (!mqttClient.connected()) {
    mqttReconnect();
  }

  mqttClient.loop();
}
