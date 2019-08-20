#include "device.h"                  // Device header
#include "gpio.h"


#ifndef PIN_H
#define PIN_H

class Pin: protected Gpio
{
//variables
public:

private:
	uint8_t pin_;
//functions
public:
	Pin (){}
	Pin (Device::port prt, uint8_t p , mux mx);
	Pin (uint8_t prt, uint8_t p , mux mx);
	Pin (Device::port prt, uint8_t p);
	Pin (Device::port prt, uint8_t p , PP m);
	void direction (mode m);
	void setIn (PP pp_);
	void setOut ();
	void set();
	void set (bool st);
	void clear ();
	void togle ();
	bool state ();
	PORT_Type * getPort ();

	uint8_t & getPin ();
};

#endif



