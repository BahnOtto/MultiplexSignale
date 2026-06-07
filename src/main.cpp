// Copyright (c) 2025 BahnOtto

#include <Arduino.h>

/* Dieser Decoder steuert:
 *  - Ausfahrsignal N1
 *  - Ausfahrsignal N2
 *  - Ausfahrsignal N3
 */

#include "Multiplex.h"
#include <NmraDcc.h>

#define Hp0 1
#define Sh1 2
#define Hp1 3
#define Hp2 4


// Basiseinstellungen
const int addr1[] = { 80, 81};    // Adressen Ausfahrsignal N1
const int addr2[] = { 78, 79};    // Adressen Ausfahrsignal N2
const int addr3[] = { 74, 75};    // Adressen Ausfahrsignal N3

Multiplex Signal1( 8, 13, 6,  7); // nicht systematisch weiße Markierung beachten
Multiplex Signal2( 4, 13, 5,  3); // Vorwiderstände 120 Ohm 0204 mini
Multiplex Signal3(11, 13, 9, 10);

NmraDcc Dcc;


void setup() {
  // DCC
  Dcc.pin(0, 2, 1); //Interrupt Pin 2
  Dcc.init( MAN_ID_DIY, 10, FLAGS_OUTPUT_ADDRESS_MODE | FLAGS_DCC_ACCESSORY_DECODER, 0);
  pinMode(LED_BUILTIN,OUTPUT);

  //Serial.begin(9600);
  //Serial.println("Start");
}

void loop() {
  // DCC-Kommandos empfangen
  Dcc.process();
  // LEDs aktualisieren
  Signal1.frameWriter();
  //delay(5);
  Signal2.frameWriter();
  //delay(5);
  Signal3.frameWriter();
  //delay(5);
}



void notifyDccAccTurnoutOutput(uint16_t Addr, uint8_t Direction, uint8_t OutputPower) {
	digitalWrite(LED_BUILTIN,!digitalRead(LED_BUILTIN));
	int myAddr = Addr;
	// Hs 1
	if(myAddr == addr1[0]) {
		if(Direction == 1){
//			Serial.println("S1 Hp1 gruen");
			Signal1.setSignal(Hp1);}
		else {
//			Serial.println("S1 Hp0 rot");
			Signal1.setSignal(Hp0);}
		}
	if(myAddr == addr1[1]) {
		if(Direction == 1) {
//			Serial.println("S1 Hp2 gelb");
			Signal1.setSignal(Hp2);}
		else {
//			Serial.println("S2 Sh1 weiss");
			Signal1.setSignal(Sh1);}
	}

	// Hs 2
	if(myAddr == addr2[0]) {
		if(Direction == 1){
//			Serial.println("S2 Hp1 gruen");
			Signal2.setSignal(Hp1);}
		else {
//			Serial.println("S2 Hp0 rot");
			Signal2.setSignal(Hp0);}
		}
	if(myAddr == addr2[1]) {
		if(Direction == 1) {
//			Serial.println("S2 Hp2 gelb");
			Signal2.setSignal(Hp2);}
		else {
//			Serial.println("S2 Sh1 weiss");
			Signal2.setSignal(Sh1);}
	}

	// Hs 3
		if(myAddr == addr3[0]) {
			if(Direction == 1){
//				Serial.println("S3 Hp1 gruen");
				Signal3.setSignal(Hp1);}
			else {
//				Serial.println("S3 Hp0 rot");
				Signal3.setSignal(Hp0);}
						}
		if(myAddr == addr3[1]) {
			if(Direction == 1) {
//				Serial.println("S3 Hp2 gelb");
				Signal3.setSignal(Hp2);}
			else {
//				Serial.println("S3 Sh1 weiss");
				Signal3.setSignal(Sh1);}
		}
	}
