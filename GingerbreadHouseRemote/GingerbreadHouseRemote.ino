/*
 * Gingerbread House Remote
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
int pinButtonRed = 2;
int pinButtonYellow = 4;
int pinLedYellow = 10;
int pinLedRed = 9;

// Other variables
int valueRed = 0;
int valueYellow = 0;
boolean buttonRedCurrentlyChanging = false;
boolean buttonYellowCurrentlyChanging = false;

WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

void setup() {
  setupPins();
  setupWifi();
  setupMqtt();
}

void setupPins() {
  pinMode(pinLedYellow, OUTPUT);
  pinMode(pinLedRed, OUTPUT);
}

void setupWifi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(wifiSsid.c_str(), wifiPassword.c_str());
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
  }
}

void setupMqtt() {
  mqttClient.setServer(mqttServer.c_str(), mqttServerPort);
}

void loop() {
  checkAndUpdateButton(pinButtonRed, pinLedRed, mqttTopic1, buttonRedCurrentlyChanging, valueRed);
  checkAndUpdateButton(pinButtonYellow, pinLedYellow, mqttTopic2, buttonYellowCurrentlyChanging, valueYellow);

  if (!mqttClient.connected()) {
    mqttReconnect();
  }

  mqttClient.loop();
}

void mqttReconnect() {
  while (!mqttClient.connected()) {
    if (!mqttClient.connect(mqttClientDescription.c_str(), mqttUsername.c_str(), mqttPassword.c_str())) {
      delay(10000);
    }
  }
}

void checkAndUpdateButton(int pinButton, int pinLed, String key, boolean &buttonCurrentlyChanging, int &value) {
  if (digitalRead(pinButton) == 1) {
    if (!buttonCurrentlyChanging) {
      buttonCurrentlyChanging = true;
      if (value == 0) {
        value = 1;
        digitalWrite(pinLed,HIGH);
      } else if (value == 1) {
        value = 0;
        digitalWrite(pinLed,LOW);
      }
      mqttClient.publish(key.c_str(), String(value).c_str());
    }
  } else {
    buttonCurrentlyChanging = false;
  }
  delay(50);
}
