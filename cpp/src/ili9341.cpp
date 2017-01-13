#include "ili9341.h"

Ili9341::Ili9341(Spi &d, Gpio::Port po, uint8_t p)
:dc (po, p)
{
	driver = &d;
	driver->setCpol(Spi::Cpol::neg);
	driver->setCpha(Spi::Cpha::first);
	driver->setDivision(Spi::Division::div32);
	driver->setFrameSize(Spi::Size::bit8);
	driver->setMode(Spi::Mode::hardware);
	driver->start();
}

void Ili9341::data (uint8_t dta)
{
	dc.set();
	while (!driver->flagSptef());
	driver->putDataDl(dta);
}

void Ili9341::command (uint8_t com)
{
	dc.clear();
	while (!driver->flagSptef());
	driver->putDataDl(com);
}

void Ili9341::init ()
{
	command (ili9341Commands::softwareReset);
	delay_ms(1000);

	//
	command (ili9341Commands::powerControl1);
	data(0x25);

	command (ili9341Commands::powerControl2);
	data(0x11);

	command (ili9341Commands::vcomControl1);
	data(0x2B);
	data(0x2B);

	command (ili9341Commands::vcomControl2);
	data(0x86);

	command (ili9341Commands::frameControl);
	data(0x00);
	data(0x18);
}

