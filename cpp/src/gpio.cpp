#include "gpio.h"

Gpio::Gpio (){
	Device &device = Device::getInstance();	
}

Gpio::Gpio (Device::port p){
	Device &device = Device::getInstance();	
	uint8_t prt = static_cast <uint8_t> (p);
	portPtr = device.getPortPtr(p);
	gpioPtr = device.getGpioPtr(p);
	device.clockPort(p);

  //takt port
	//SIM->SCGC5 |= CLOCK_PORT_MASK[prt];
}


Gpio::Gpio (uint8_t prt){

}
