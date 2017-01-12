#include "MKL26Z4.h"
#include "pin.h"
#include "i2c.h"
#include "delay.h"

#ifndef DS1307_H
#define DS1307_H

namespace ds1307def
{
	const Gpio::Port sdaPort = Gpio::Port::A;
	const uint8_t sdaPin = 1;
	const Gpio::mux sdaMux = Gpio::mux::Alt2;

	const Gpio::Port sdlPort = Gpio::Port::A;
	const uint8_t sdlPin = 1;
	const Gpio::mux sdlMux = Gpio::mux::Alt2;
}

namespace ds1307reg
{
const uint8_t Seconds = 0x00;
const uint8_t Minutes = 0x01;
const uint8_t Hours   = 0x02;
const uint8_t Day     = 0x03;
const uint8_t Date    = 0x04;
const uint8_t Month   = 0x05;
const uint8_t Year    = 0x06;
const uint8_t Control = 0x07;
}
const uint8_t selfAddress = 0xD0;
const uint8_t stateOff    = 0x80;
const uint8_t stateOn     = 0x00;

class Ds1307
{
public:

private:
  I2c * driver;
  Pin sda, sdl;

public:
  Ds1307(I2c &);
  void write (uint8_t reg, uint8_t val);
  uint8_t read (uint8_t reg);
  void read (uint8_t * buff, uint8_t reg, uint8_t n);
  void stop ();
  void start ();

};


#endif
