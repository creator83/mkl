#include "pin.h"

Pin::Pin (Gpio::Port prt, uint8_t p , mux mx)
:Gpio(prt)
{
	pin_ = p;
	PORT_PCR_REG(PortBase[Gpio::prt],pin_) |= mx << PORT_PCR_MUX_SHIFT;
}

Pin::Pin (Port prt, uint8_t p )
{
	pin_ = p;
	PORT_PCR_REG(PortBase[Gpio::prt],pin_) |= Gpio::GPIO << PORT_PCR_MUX_SHIFT;
	GPIO_PDDR_REG(GpioBase[Gpio::prt]) |= 1 << pin_;
}

Pin::Pin (Port prt, uint8_t p , PP m)
{
	pin_ = p;
	PORT_PCR_REG(PortBase[Gpio::prt],pin_) |= Gpio::GPIO << PORT_PCR_MUX_SHIFT;
	GPIO_PDDR_REG(GpioBase[Gpio::prt]) &= ~(1 << pin_);
	PORT_PCR_REG(PortBase[Gpio::prt],pin_) |= PORT_PCR_PE_MASK;
	PORT_PCR_REG(PortBase[Gpio::prt],pin_) &= ~PORT_PCR_PS_MASK;
	PORT_PCR_REG(PortBase[Gpio::prt],pin_) |= m << PORT_PCR_PS_SHIFT;
}

void Pin::direction (mode m)
{
	GPIO_PDDR_REG(GpioBase[prt]) &= ~(1 << pin_);
	GPIO_PDDR_REG(GpioBase[prt]) |= ((uint8_t)m << pin_);
}

void Pin::setIn (PP pp_)
{
	GPIO_PDDR_REG(GpioBase[prt]) &= ~(1 << pin_);
	PORT_PCR_REG(PortBase[prt],pin_) |= 1 << PORT_PCR_PE_MASK;
	PORT_PCR_REG(PortBase[prt],pin_) &= ~ (1 << PORT_PCR_PS_MASK);
	PORT_PCR_REG(PortBase[prt],pin_) |= pp_ << PORT_PCR_PS_MASK;
}

void Pin::setOut ()
{
	GPIO_PDDR_REG(GpioBase[prt]) |= 1 << pin_;
}


void Pin::set()
{
	GPIO_PSOR_REG(GpioBase[prt]) |= 1 << pin_;
}

void Pin::set (bool st)
{
	GPIO_PCOR_REG(GpioBase[prt]) |= 1 << pin_;
	GPIO_PSOR_REG(GpioBase[prt]) |= st << pin_;
}

void Pin::clear ()
{
	GPIO_PCOR_REG(GpioBase[prt]) |= 1 << pin_;
}

void Pin::togle ()
{
	GPIO_PTOR_REG(GpioBase[prt]) |= 1 << pin_;
}

bool Pin::state ()
{
	return (GPIO_PDIR_REG(GpioBase[prt])&(1 << pin_));
}

PORT_MemMapPtr & Pin::getPort ()
{
	return PortBase[prt];
}
