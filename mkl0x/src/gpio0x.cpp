#include "gpio0x.h"

/*
GPIO_MemMapPtr Gpio::GpioBase [2] = {GPIOA, GPIOB};
PORT_MemMapPtr Gpio::PortBase [2] = {PORTA, PORTB};*/
PORT_Type * Gpio::PortBase [2] = {PORTA, PORTB};
GPIO_Type * Gpio::GpioBase [2] = {GPIOA, GPIOB};

Gpio::Gpio ()
{

}

Gpio::Gpio (Port p)
{
  prt = static_cast <uint8_t> (p);
  //takt port
  SIM->SCGC5 |= (0x200 << prt);
}

Gpio::Gpio(uint8_t p )
{
  prt = p;
  //takt port
  SIM->SCGC5 |= (0x200 << prt);
}

uint8_t Gpio::gPort ()
{
	return prt;
}

