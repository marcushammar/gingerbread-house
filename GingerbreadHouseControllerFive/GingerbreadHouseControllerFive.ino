/*
* Gingerbread House Controller Five
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

const int relayOnePin = 1;
const int relayTwoPin = 2;
const int relayThreePin = 3;
const int relayFourPin = 4;

bool relayOneState = false;
bool relayTwoState = false;
bool relayThreeState = false;
bool relayFourState = false;

WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

void setup() {
  setupWifi();
  setupMqtt();
  setupRelays();
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

void setupRelays() {
  pinMode(relayOnePin, OUTPUT);
  pinMode(relayTwoPin, OUTPUT);
  pinMode(relayThreePin, OUTPUT);
  pinMode(relayFourPin, OUTPUT);
}

void mqttCallback(char* topic, byte* payload, unsigned int length) {
  handleCallback(topic, mqttTopicOne, payload, relayOneState);
  handleCallback(topic, mqttTopicTwo, payload, relayTwoState);
  handleCallback(topic, mqttTopicThree, payload, relayThreeState);
  handleCallback(topic, mqttTopicFour, payload, relayFourState);
}

void handleCallback(char* topic, String mqttTopic, byte* payload, bool& state) {
  if (strcmp(topic, mqttTopic.c_str()) == 0) {
    if ((char)payload[0] == '1') {
      state = true;
    } else {
      state = false;
    }
  }
}

void mqttReconnect() {
  while (!mqttClient.connected()) {
    if (mqttClient.connect(mqttClientDescription.c_str(), mqttUsername.c_str(), mqttPassword.c_str())) {
      mqttClient.subscribe(mqttTopicOne.c_str());
      mqttClient.subscribe(mqttTopicTwo.c_str());
      mqttClient.subscribe(mqttTopicThree.c_str());
      mqttClient.subscribe(mqttTopicFour.c_str());
    } else {
      delay(10000);
    }
  }
}

void loop() {
  loopMqtt();
  loopRelays();
}

void loopMqtt() {
  if (!mqttClient.connected()) {
    mqttReconnect();
  }

  mqttClient.loop();
}

void loopRelays() {
  adjustRelay(relayOneState, relayOnePin);
  adjustRelay(relayTwoState, relayTwoPin);
  adjustRelay(relayThreeState, relayThreePin);
  adjustRelay(relayFourState, relayFourPin);
}

void adjustRelay(bool relayState, int relayPin) {
  if (relayState) {
    digitalWrite(relayPin, HIGH);
  } else {
    digitalWrite(relayPin, LOW);
  }
}
