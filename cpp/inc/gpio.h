#include "device.h"              // Device header


#ifndef GPIO_H
#define GPIO_H

class Gpio
{
//variables
public:
  enum class mux {Analog, GPIO , Alt2 , Alt3 , Alt4 , Alt5 , Alt6 , Alt7};
  enum class mode {Input, Output};
  enum class PP {PullDown, PullUp};
  enum class state {Off, On};

protected:
	PORT_Type * portPtr;
	GPIO_Type * gpioPtr;
	uint32_t * clockPortRegister;
private:

//functions
public:
  Gpio ();
Gpio(Device::port p );
  Gpio(uint8_t p );
  void setPort (Device::port p);

  //function for port
  void settingPort (uint32_t val, mux mx = mux::GPIO, mode m = mode::Output);
  void settingPortDirection (uint32_t val, mode m);
  void setValPort (uint32_t value);
  void clearValPort (uint32_t value);
};

#endif



