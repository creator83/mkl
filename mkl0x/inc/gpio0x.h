#include "device.h"             // Device header


#ifndef GPIO_H
#define GPIO_H

class Gpio
{
//variables
public:
  enum class Port {A , B};
  enum class mux {Analog, GPIO , Alt2 , Alt3 , Alt4 , Alt5 , Alt6 , Alt7};
  enum class mode {Input, Output};
  enum class PP {PullDown, PullUp};
  enum class state {Off, On};

protected:
  static PORT_MemMapPtr PortBase [2];
  static GPIO_MemMapPtr GpioBase [2];
  uint8_t prt;
private:

//functions
public:
  Gpio ();
  Gpio(Port p );
  void setPort (Port p);
};

#endif



