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
#include <ESP8266HTTPClient.h>
#include "Credentials.h"

// Declaration of pins
int pinLedRed = 9;
int pinLedYellow = 10;

char commandRed = 0;
char commandYellow = 0;

void setup() {
  pinMode(pinLedRed, OUTPUT);
  pinMode(pinLedYellow, OUTPUT);

  WiFi.mode(WIFI_STA);
  WiFi.begin(wifiSsid.c_str(), wifiPassword.c_str());
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
  }

  Serial.begin(9600);
}

void loop() {
  if (getString(device, "red") == String(1)) {
    digitalWrite(pinLedRed,HIGH);
    commandRed = 'A';
  } else {
    digitalWrite(pinLedRed,LOW);
    commandRed = 'B';
  }


  if (getString(device, "yellow") == String(1)) {
    digitalWrite(pinLedYellow,HIGH);
    commandYellow = 'C';
  } else {
    digitalWrite(pinLedYellow,LOW);
    commandYellow = 'D';
  }

  Serial.write(commandRed);
  Serial.write(commandYellow);

  delay(1000);
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
