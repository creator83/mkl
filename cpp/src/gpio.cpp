#include "gpio.h"

FGPIO_MemMapPtr Gpio::GpioBase [5] = {FGPIOA_BASE_PTR , FGPIOB_BASE_PTR , FGPIOC_BASE_PTR, FGPIOD_BASE_PTR, FGPIOE_BASE_PTR};
PORT_MemMapPtr Gpio::PortBase [5] = {PORTA_BASE_PTR, PORTB_BASE_PTR, PORTC_BASE_PTR, PORTD_BASE_PTR, PORTE_BASE_PTR};


Gpio::Gpio ()
{

}

Gpio::Gpio (Port p)
{
  prt = static_cast <uint8_t> (p);
  //takt port
  SIM->SCGC5 |= (0x200 << prt);
}

void Gpio::setPort (Port p)
{
	prt = static_cast <uint8_t> (p);
	//takt port
	SIM->SCGC5 |= (0x200 << prt);
}


void Gpio::setValPort (uint32_t value)
{
	FGPIO_PSOR_REG(GpioBase[prt])  |= value;
}

void Gpio::clearValPort (uint32_t value)
{
	FGPIO_PCOR_REG(GpioBase[prt])  |= value;
}



