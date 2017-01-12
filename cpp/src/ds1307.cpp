#include "ds1307.h"

Ds1307::Ds1307 (I2c &d)
:sda(ds1307def::sdaPort, ds1307def::sdaPin, ds1307def::sdaMux),
 sdl (ds1307def::sdlPort, ds1307def::sdlPin, ds1307def::sdlMux)
{
	driver = &d;
}

void Ds1307::write (uint8_t reg, uint8_t val)
{
	driver->start();
	driver->write(selfAddress|writeBit);
	driver->write (reg);
	driver->write (val);
	driver->stop();
}

uint8_t Ds1307::read (uint8_t reg)
{
	driver->start();
	driver->write(selfAddress|writeBit);
	driver->write (reg);
	driver->restart();
	driver->write(selfAddress|readBit);
	uint8_t data = driver->read(false);
	driver->stop();
	return data;
}

void Ds1307::read (uint8_t * buff, uint8_t reg, uint8_t n)
{
	driver->start();
	driver->write(selfAddress|writeBit);
	driver->write (reg);
	driver->restart();
	driver->write(selfAddress|readBit);
	uint8_t temp = n-1;
	while (temp--)
	{
		*buff++ = driver->read(true);
	}
	*buff++ = driver->read(false);
	driver->stop();
}

void Ds1307::stop ()
{
	write (ds1307reg::Seconds, stateOff);
}

void Ds1307::start ()
{
	write (ds1307reg::Seconds, stateOn);
}
