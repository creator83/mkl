#include "device.h"                // Device header
#include "pin0x.h"


#ifndef SEGLED_H
#define SEGLED_H


namespace segledDef
{
  //segment A
  const Gpio::Port segAport = Gpio::Port::A;
  const uint8_t segApin = 8;
  //segment B
  const Gpio::Port segBport = Gpio::Port::B;
  const uint8_t segBpin = 12;
  //segment C
  const Gpio::Port segCport = Gpio::Port::B;
  const uint8_t segCpin = 0;
  //segment D
  const Gpio::Port segDport = Gpio::Port::B;
  const uint8_t segDpin = 1;
  //segment E
  const Gpio::Port segEport = Gpio::Port::A;
  const uint8_t segEpin = 11;
  //segment F
  const Gpio::Port segFport = Gpio::Port::A;
  const uint8_t segFpin = 9;
  //segment G
  const Gpio::Port segGport = Gpio::Port::B;
  const uint8_t segGpin = 8;
  //segment Dp
  const Gpio::Port segDpport = Gpio::Port::B;
  const uint8_t segDppin = 5;
  //digit 1
  const Gpio::Port digit1port = Gpio::Port::B;
  const uint8_t digit1pin = 2;
  //digit 2
  const Gpio::Port digit2port = Gpio::Port::A;
  const uint8_t digit2pin = 10;
  //digit 3
  const Gpio::Port digit3port = Gpio::Port::A;
  const uint8_t digit3pin = 13;
  //digit 4
  const Gpio::Port digit4port = Gpio::Port::A;
  const uint8_t digit4pin = 12;
    
}


class Segled
{
//variables
public:	
	
private:
  Pin segA, segB, segC, segD, segE, segF, segG , segDp;
  Pin dig1, dig2, dig3, dig4;
  static uint8_t number [13];
  static uint8_t numberDp [13];
  Pin * segments [8];
  Pin * digits [4];
  uint8_t segPins [8];
  uint8_t digPins [4];
  uint8_t N;
public:	
  Segled (uint8_t nDigit);
	
  void setSegments (char * val);
  void clearSegments ();
        
  void setDigit (uint8_t d);
  void clearDigits ();
        
  void frame (char * val, uint8_t d);
  void blink (char * val, uint8_t d, uint16_t period);

  void value (char * val, uint8_t n);
        
private:
	
	
};


#endif


