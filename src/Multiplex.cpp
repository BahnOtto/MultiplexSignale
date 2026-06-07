// Copyright (c) 2025 BahnOtto

#include "Arduino.h"
#include "Multiplex.h"

// LED-Nr. (1-12) wird dem entsprechenden Frameelement zugeordnet
// Hauptsperrsignal:

#define rtR 12
#define rtL 10
#define gn  3
#define ge  9
#define wsR 7
#define wsL 2

struct Charlieplex {
	byte HPin;
	byte LPin;
	};

const Charlieplex  _cp[maxLeds] = {{0,1}, {0,2}, {0,3},
					                         {1,0}, {1,2}, {1,3},
	 					                       {2,0}, {2,1}, {2,3},
						                       {3,0}, {3,1}, {3,2}};

Multiplex::Multiplex(byte pin1, byte pin2, byte pin3, byte pin4)
{
  _pins[0] = pin1;
  _pins[1] = pin2;
  _pins[2] = pin3;
  _pins[3] = pin4;
  for (int i = 0; i < maxLeds; i++) {
	  _frame[i].on = false;
	  _frame[i].brightness = 255;
  	_frame[i].phase = 0;
  	_frame[i].riseTime = 3;  // 2 4
  }
  _frameNr = 0;
  state1 = 1;
  state2 = 1;
  setHp0();
}

void Multiplex::setOn(byte ledNr)
{
	if (ledNr > 0 && ledNr <= maxLeds) _frame[ledNr - 1].on = true;
}

void Multiplex::setOff(byte ledNr)
{
	if (ledNr > 0 && ledNr <= maxLeds) _frame[ledNr - 1].on = false;
}

void Multiplex::ledOn() {
	 Charlieplex cp = _cp[_frameNr];
	 pinMode(_pins[cp.HPin], OUTPUT); digitalWrite(_pins[cp.HPin], HIGH);
	 pinMode(_pins[cp.LPin], OUTPUT); digitalWrite(_pins[cp.LPin], LOW);
}

void Multiplex::setDark() {
	if  (_frame[_frameNr].brightness <= 1) {
		 state1 = state2;
		 setNewState();
	}
	if (_frame[_frameNr].brightness / 32 >= _frame[_frameNr].phase) ledOn();
	 _frame[_frameNr].phase++;
	if (_frame[_frameNr].phase > 7) { //255:32 = 7,9
	   _frame[_frameNr].phase = 0;
	   _frame[_frameNr].brightness -= _frame[_frameNr].riseTime;
	 }
}

void Multiplex::setBright() {
	if (_frame[_frameNr].brightness / 32 >= _frame[_frameNr].phase)	ledOn();
	_frame[_frameNr].phase++;
	if (_frame[_frameNr].phase > 7)	{
	  _frame[_frameNr].phase = 0;
	  _frame[_frameNr].brightness += _frame[_frameNr].riseTime;
	}
}

void Multiplex::frameWriter()
{
  pinMode(_pins[0],INPUT);
  pinMode(_pins[1],INPUT);
  pinMode(_pins[2],INPUT);
  pinMode(_pins[3],INPUT);
  if (_frame[_frameNr].on) {
	  if (state1 == state2) {
		  if (_frame[_frameNr].brightness > 223) ledOn(); // 255-32 = 223
		  else setBright();
	  }
	  else setDark();
  }
  _frameNr++;
  if (_frameNr > maxLeds-1) _frameNr = 0;
}

//------------------------ Signalbefehle ------------------------ //


void Multiplex::setHp0()
{
  setOff(gn);
  setOff(ge);
  setOff(wsL);
  setOff(wsR);
  setOn(rtL);
  setOn(rtR);
}

void Multiplex::setHp1()
{
  setOff(rtL);
  setOff(rtR);
  setOff(ge);
  setOff(wsL);
  setOff(wsR);
  setOn(gn);
}

void Multiplex::setHp2()
{
  setOff(wsL);
  setOff(wsR);
  setOff(rtL);
  setOff(rtR);
  setOn(gn);
  setOn(ge);
}

void Multiplex::setSh1()
{
  setOff(gn);
  setOff(rtR);
  setOff(ge);
  setOn(wsL);
  setOn(wsR);
  setOn(rtL);
}

void Multiplex::setSignal(byte state){
	state2 = state;
}

void Multiplex::setNewState(){
	state1 = state2;
    switch (state1) {
    	case 1: setHp0(); break;
    	case 2: setHp1(); break;
    	case 3: setHp2(); break;
    	case 4: setSh1(); break;
    }
}

