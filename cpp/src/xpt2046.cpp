#include "xpt2046.h"



Xpt2046::Xpt2046 (Spi &d)
:cs(xpt2046Def::csPort, xpt2046Def::csPin), irq (xpt2046Def::irqPort, xpt2046Def::irqPin, Intrpt::mode::fallingEdge),
 x(0), y(0)
{
	driver = &d;
	NVIC_EnableIRQ(PORTA_IRQn);
	driver->setCpol(Spi::Cpol::neg);
	driver->setCpha(Spi::Cpha::first);
	driver->setDivision(Spi::Division::div32);
	driver->setFrameSize(Spi::Size::bit8);
	cs.set ();
	driver->start();
	cs.clear();
	driver->putDataDl(0x80);
	while (!driver->flagSptef());
	driver->putDataDl(0x00);
	while (!driver->flagSptef());
	driver->putDataDl(0x00);
	while (!driver->flagSptef());
	cs.set();
}

void Xpt2046::getData ()
{
	cs.clear();
	while (!driver->flagSptef());
	driver->putDataDl (channelX);
	while (!driver->flagSprf());
	uint8_t dummy = driver->getDataDl();
	while (!driver->flagSptef());
	driver->putDataDl (0);
	while (!driver->flagSprf());
	x = driver->getDataDl() << 8;
	while (!driver->flagSptef());
	driver->putDataDl (0);
	while (!driver->flagSprf());
	x |= driver->getDataDl() >> 4;
	delay_us(100);
	driver->putDataDl (channelY);
	while (!driver->flagSprf());
	dummy = driver->getDataDl();
	while (!driver->flagSptef());
	driver->putDataDl (0);
	while (!driver->flagSprf());
	y = driver->getDataDl() << 8;
	while (!driver->flagSptef());
	driver->putDataDl (0);
	while (!driver->flagSprf());
	y |= driver->getDataDl() >> 4;
}



