#include "pin.h"

Pin::Pin (Gpio::Port prt, uint8_t p , mux mx)
:Gpio(prt)
{
	pin_ = p;
	PORT_PCR_REG(PortBase[Gpio::prt],pin_) = (uint8_t)mx << PORT_PCR_MUX_SHIFT;
}

Pin::Pin (Port prt, uint8_t p )
:Gpio(prt)
{
	pin_ = p;
	PORT_PCR_REG(PortBase[Gpio::prt],pin_) = (uint8_t)Gpio::mux::GPIO << PORT_PCR_MUX_SHIFT;
	FGPIO_PDDR_REG(GpioBase[Gpio::prt]) |= 1 << pin_;
}

Pin::Pin (Port prt, uint8_t p , PP m)
:Gpio(prt)
{
	pin_ = p;
	PORT_PCR_REG(PortBase[Gpio::prt],pin_) = (uint8_t)Gpio::mux::GPIO << PORT_PCR_MUX_SHIFT;
	FGPIO_PDDR_REG(GpioBase[Gpio::prt]) &= ~(1 << pin_);
	PORT_PCR_REG(PortBase[Gpio::prt],pin_) |= PORT_PCR_PE_MASK;
	PORT_PCR_REG(PortBase[Gpio::prt],pin_) &= ~PORT_PCR_PS_MASK;
	PORT_PCR_REG(PortBase[Gpio::prt],pin_) |= (uint8_t)m << PORT_PCR_PS_SHIFT;
}

void Pin::direction (mode m)
{
	FGPIO_PDDR_REG(GpioBase[prt]) &= ~(1 << pin_);
	FGPIO_PDDR_REG(GpioBase[prt]) |= ((uint8_t)m << pin_);
}

void Pin::setIn (PP pp_)
{
	FGPIO_PDDR_REG(GpioBase[prt]) &= ~(1 << pin_);
	PORT_PCR_REG(PortBase[prt],pin_) |= 1 << PORT_PCR_PE_MASK;
	PORT_PCR_REG(PortBase[prt],pin_) &= ~ (1 << PORT_PCR_PS_MASK);
	PORT_PCR_REG(PortBase[prt],pin_) |= (uint8_t)pp_ << PORT_PCR_PS_MASK;
}

void Pin::setOut ()
{
	FGPIO_PDDR_REG(GpioBase[prt]) |= 1 << pin_;
}


void Pin::set()
{
	FGPIO_PSOR_REG(GpioBase[prt]) |= 1 << pin_;
}

void Pin::set (bool st)
{
	FGPIO_PCOR_REG(GpioBase[prt]) |= 1 << pin_;
	FGPIO_PSOR_REG(GpioBase[prt]) |= st << pin_;
}

void Pin::clear ()
{
	FGPIO_PCOR_REG(GpioBase[prt]) |= 1 << pin_;
}

void Pin::togle ()
{
	FGPIO_PTOR_REG(GpioBase[prt]) |= 1 << pin_;
}

bool Pin::state ()
{
	return (FGPIO_PDIR_REG(GpioBase[prt])&(1 << pin_));
}

PORT_MemMapPtr & Pin::getPort ()
{
	return PortBase[prt];
}

uint8_t & Pin::getPin ()
{
	return pin_;
}
