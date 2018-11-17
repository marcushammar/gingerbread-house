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
#include <ESP8266HTTPClient.h>
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

void setup() {
  pinMode(pinLedYellow, OUTPUT);
  pinMode(pinLedRed, OUTPUT);

  WiFi.mode(WIFI_STA);
  WiFi.begin(wifiSsid.c_str(), wifiPassword.c_str());
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
  }

  if (getString(device, "red") == String(1)) {
    valueRed = 1;
    digitalWrite(pinLedRed,HIGH);
  }

  if (getString(device, "yellow") == String(1)) {
    valueYellow = 1;
    digitalWrite(pinLedYellow,HIGH);
  }
}

void loop() {
  checkAndUpdateButton(pinButtonRed, pinLedRed, "red", buttonRedCurrentlyChanging, valueRed);
  checkAndUpdateButton(pinButtonYellow, pinLedYellow, "yellow", buttonYellowCurrentlyChanging, valueYellow);
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
      setString(device, key, String(value));
    }
  } else {
    buttonCurrentlyChanging = false;
  }
}

void setString(String device, String key, String value) {
  HTTPClient httpClient;
  httpClient.begin(String("http://") + appEngineApplication + ".appspot.com/?device=" + device + "&key=" + key + "&value=" + value);
  httpClient.GET();
  httpClient.end();
}

String getString(String device, String key) {
  HTTPClient httpClient;
  httpClient.begin(String("http://") + appEngineApplication + ".appspot.com/?device=" + device + "&key=" + key);
  String response;
  if (httpClient.GET() == HTTP_CODE_OK) {
    response = httpClient.getString();
  }
  httpClient.end();
  return response;
}
