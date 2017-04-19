#include "device.h"
#include "pin.h"
#include "spi.h"
#include "intrpt.h"
#include "delay.h"

#ifndef XPT2046_H
#define XPT2046_H

const uint8_t channelY = 0x90;
const uint8_t channelX = 0xD0;


class Xpt2046
{
//variables
	uint16_t x, y;
	uint16_t Xmin, Ymin, dX, dY;
public:
  Pin cs;
  Spi * driver;
  Intrpt irq;

public:

  Xpt2046 (Spi &, Gpio::Port cs_, uint16_t csp, Gpio::Port irq_, uint16_t irqp);
  void getData ();
  uint16_t & getX ();
  uint16_t & getY ();
  uint16_t & getdX ();
  uint16_t & getdY ();

  void clearFlag ();

};


#endif
