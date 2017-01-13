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

void Ili9341::setDma (Dma &d)
{
	dma = &d;
	dma->setDsize(Dma::size::bit16);
	dma->setSsize(Dma::size::bit16);
	dma->setDestination((uint32_t)&driver->getSpiPtr()->DL);
	//dma->enableDmaMux()
	driver->setDma(d);
}

void Ili9341::fillScreen (uint16_t color)
{
	command (ili9341Commands::memoryWrite);
	driver->setFrameSize(Spi::Size::bit16);
	dma->setIncSource(false);
	dataDma (&color, 76800);
	driver->setFrameSize(Spi::Size::bit8);
}

void Ili9341::data (uint8_t dta)
{
	dc.set();
	while (!driver->flagSptef());
	driver->putDataDl(dta);
}

void Ili9341::data16 (uint16_t dta)
{
	dc.set();
	while (!driver->flagSptef());
	driver->putDataDh(dta >> 8);
	driver->putDataDl(dta);
}

void Ili9341::dataDma (uint16_t * buf, uint32_t n)
{
	dma->setSource((uint32_t)buf);
	dma->setLength(n);
	driver->enableDma(Spi::dma::transmit);
	while (!dma->flagDone());
	dma->clearFlags();
	driver->disableDma(Spi::dma::transmit);
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

	//orient
	command (ili9341Commands::memryAccessControl);
	data(0x28);

	command (ili9341Commands::pixelFormatSet);
	data(0x05);

	command (ili9341Commands::frameControl);
	data(0x00);
	data(0x18);

	command (ili9341Commands::pixelFormatSet);
	data(0x05);

	command (ili9341Commands::displayFunctionControl);
	data(0x0A);
	data(0x82);
	data(0x27);

	command (ili9341Commands::sleepOut);
	delay_ms(120);

	command (ili9341Commands::displayOn);

}

void Ili9341::setPage (uint16_t y1, uint16_t y2)
{
	command (ili9341Commands::pageAddressSet);
	data (y1>>8);
	data (y1);
	data (y2>>8);
	data (y2);
}

void Ili9341::setColoumn (uint16_t x1, uint16_t x2)
{
	command (ili9341Commands::pageAddressSet);
	data (x1>>8);
	data (x1);
	data (x2>>8);
	data (x2);
}

void Ili9341::pixel (const uint16_t color)
{
	command (ili9341Commands::memoryWrite);
	driver->setFrameSize(Spi::Size::bit16);
	data16 (color);
	driver->setFrameSize(Spi::Size::bit8);
}

