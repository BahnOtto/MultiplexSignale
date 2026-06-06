
#ifndef Multiplex_h
#define Multiplex_h

#include "Arduino.h"
#define maxLeds 12 // = n*(n-1) n=4 Charlieplex

struct ledState {
		   	bool on;
		   	byte brightness;
		   	byte phase;
		   	byte riseTime;
		   };

class Multiplex
{
public:
    Multiplex(byte pin1, byte pin2, byte pin3, byte pin4);
    void frameWriter();
    void setSignal(byte state);

private:
    void setOn (byte ledNr);
    void setOff(byte ledNr);
    void setHp0();
    void setSh1();
    void setHp1();
    void setHp2();
    void setDark();
    void setBright();
    void ledOn();
    void setNewState();

    byte _pins[4];
    ledState  _frame[maxLeds];
    byte state1;
    byte state2;
    byte _frameNr;
};

#endif











