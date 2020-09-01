/*
 Name:		Sketch14.ino
 Created:	7/29/2020 9:49:25 AM
 Author:	Janie
*/

// the setup function runs once when you press reset or power the board
#include "SerialCallResponse.h"
void setup() {
	Serial.begin(9600);
}

// the loop function runs over and over again until power down or reset
void loop() {
	Serial.write(analogRead(A0) / 4);
	delay(1);
}

import processing.serial.*;