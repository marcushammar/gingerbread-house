/*
 * Gingerbread House
 * Copyright (C) 2017-2018  Marcus Hammar
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

const int potentiometerPin = A0;
const int relayOnePin = 4;
const int relayTwoPin = 7;
const int relayThreePin = 12;
const int relayFourPin = 8;

bool flash = false;
char incomingByte = 0;

void setup() {
  pinMode(relayOnePin, OUTPUT);
  pinMode(relayTwoPin, OUTPUT);
  pinMode(relayThreePin, OUTPUT);
  pinMode(relayFourPin, OUTPUT);
  
  Serial.begin(9600);
}

void loop() {
  int potentiometerValue = analogRead(potentiometerPin);

  if (Serial.available() > 0) {
    incomingByte = Serial.read();
  }

  if (incomingByte == 'A') {
    digitalWrite(relayOnePin, HIGH);
  }
  
  if (incomingByte == 'B') {
    digitalWrite(relayOnePin, LOW);
  }
  
  if (incomingByte == 'C') {
    digitalWrite(relayTwoPin, HIGH);
  }

  if (incomingByte == 'D') {
    digitalWrite(relayTwoPin, LOW);
  }

  if (potentiometerValue > 300) {
    digitalWrite(relayThreePin, HIGH);
  } else {
    digitalWrite(relayThreePin, LOW);
  }

  if (potentiometerValue > 600) {
    digitalWrite(relayFourPin, HIGH);
  } else {
    digitalWrite(relayFourPin, LOW);
  }

  if (potentiometerValue > 1000) {
    if (flash){
      digitalWrite(relayThreePin, HIGH);
      digitalWrite(relayFourPin, HIGH);
      flash = false;
    }else{
      digitalWrite(relayThreePin, LOW);
      digitalWrite(relayFourPin, LOW);
      flash = true;
    }
  }

  delay(200);
}
