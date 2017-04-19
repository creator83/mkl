#include "intrpt.h"

//IRQn Intrpt::source [4] {};

Intrpt::Intrpt (Gpio::Port p, uint8_t pi_, mode m, Gpio::PP f)
:pin_(p, pi_, f)
{
	PORT_PCR_REG(pin_.getPort(),pi_)  |= PORT_PCR_IRQC((uint8_t)m);
	if (pin_.getPort()== PORTA_BASE_PTR)
	{
		NVIC_EnableIRQ(PORTA_IRQn);
	}
	else if (pin_.getPort()== PORTC_BASE_PTR||PORTD_BASE_PTR)
	{
		NVIC_EnableIRQ(PORTCD_IRQn);
	}
}

void Intrpt::clearFlag ()
{
	PORT_ISFR_REG(pin_.getPort()) |= 1 << pin_.getPin();
}
