#include "ili9341.h"

#define init1


Ili9341::Ili9341(Spi &d, Gpio::Port po, uint8_t p, Gpio::Port rstpo, uint8_t rstpi)
:dc (po, p), rst (rstpo, rstpi)
{
	driver = &d;
	driver->setCpol(Spi::Cpol::neg);
	driver->setCpha(Spi::Cpha::first);
	driver->setDivision(Spi::Division::div128);
	driver->setFrameSize(Spi::Size::bit8);
	driver->setMode(Spi::Mode::hardware);
	driver->start();
	init ();
	//driver->setDivision(Spi::Division::div256);
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

	/*command (ili9341Commands::memoryWrite);
	driver->setFrameSize(Spi::Size::bit16);
	dma->setIncSource(false);
	dataDma (&color, 76800);
	driver->setFrameSize(Spi::Size::bit8);*/

	/*unsigned int n, i, j;
		i = color >> 8;
		j = color & 0xFF;*/

		setColoumn(10, 100);
		setPage (10, 100);


		command(ili9341Commands::memoryWrite);
		driver->setFrameSize(Spi::Size::bit16);
		dc.set();
		for (uint32_t n = 0; n < 8000; n++) {
			while (!driver->flagSptef());
			driver->putDataDh(color>>8);
			driver->putDataDl(color);
		}
		driver->setFrameSize(Spi::Size::bit8);
}

void Ili9341::setCursor (uint16_t x , uint16_t y)
{

}

void Ili9341::setCursor (uint16_t x1 , uint16_t y1, uint16_t x2, uint16_t y2)
{
	command(ILI9341_COLUMN_ADDR);
	data(x1 >> 8);
	data(x1 & 0xFF);
	data(x2 >> 8);
	data(x2 & 0xFF);

	command(ILI9341_PAGE_ADDR);
	data(y1 >> 8);
	data(y1 & 0xFF);
	data(y2 >> 8);
	data(y2 & 0xFF);

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
	rst.set();
	delay_ms(5);
	rst.clear();
	delay_ms(20);
	rst.set();
	delay_ms(150);

#ifdef init1
	command(0xCB);
	  data(0x39);
	  data(0x2C);
	  data(0x00);
	  data(0x34);
	  data(0x02);
	  command(0xCF);
	  data(0x00);
	  data(0xC1);
	  data(0x30);
	  command(0xE8);
	  data(0x85);
	  data(0x00);
	  data(0x78);
	  command(0xEA);
	  data(0x00);
	  data(0x00);
	  command(0xED);
	  data(0x64);
	  data(0x03);
	  data(0x12);
	  data(0x81);
	  command(0xF7);
	  data(0x20);
	  command(0xC0);
	  data(0x23);
	  command(0xC1);
	  data(0x10);
	  command(0xC5);
	  data(0x3E);
	  data(0x28);
	  command(0xC7);
	  data(0x86);
	  command(0x36);
	  data(0x48);
	  command(0x3A);
	  data(0x55);
	  command(0xB1);
	  data(0x00);
	  data(0x18);
	  command(0xB6);
	  data(0x08);
	  data(0x82);
	  data(0x27);
	  command(0xF2);
	  data(0x00);
	  command(0x26);
	  data(0x01);


	  command(0xE0);    //Set Gamma
	  data(0x0F);
	  data(0x31);
	  data(0x2B);
	  data(0x0C);
	  data(0x0E);
	  data(0x08);
	  data(0x4E);
	  data(0xF1);
	  data(0x37);
	  data(0x07);
	  data(0x10);
	  data(0x03);
	  data(0x0E);
	  data(0x09);
	  data(0x00);

	  command(0XE1);    //Set Gamma
	  data(0x00);
	  data(0x0E);
	  data(0x14);
	  data(0x03);
	  data(0x11);
	  data(0x07);
	  data(0x31);
	  data(0xC1);
	  data(0x48);
	  data(0x08);
	  data(0x0F);
	  data(0x0C);
	  data(0x31);
	  data(0x36);
	  data(0x0F);

	  command(0x11);    //Exit Sleep
	  delay_ms(120);              // 120mS

	  command(0x29);    //Display on
	  command(0x2c);
#endif
#ifdef init2
	command (ili9341Commands::softwareReset);
	delay_ms(1000);
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
	data(0x55);

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
#endif

#ifdef init3
	command(0xCB);
		  data(0x39);
		  data(0x2C);
		  data(0x00);
		  data(0x34);
		  data(0x02);

		  command(0xCF);
		  data(0x00);
		  data(0xC1);
		  data(0x30);

		  command(0xE8);
		  data(0x85);
		  data(0x00);
		  data(0x78);

		  command(0xEA);
		  data(0x00);
		  data(0x00);

		  command(0xED);
		  data(0x64);
		  data(0x03);
		  data(0x12);
		  data(0x81);

		  command(0xF7);
		  data(0x20);

		  command(0xC0);
		  data(0x1B);

		  command(0xC1);
		  data(0x10);

		  command(0xC5);
		  data(0x2D);
		  data(0x33);

		  command(0xC7);
		  data(0xCF);

		  command(0x36);
		  data(0x48);

		  command(0xB1);
		  data(0x00);
		  data(0x1D);

		  command(0xB6);
		  data(0x0A);
		  data(0x02);

		  command(0xF2);
		  data(0x00);

		  command(0x26);
		  data(0x01);


		  command(0xE0);    //Set Gamma
		  data(0x0F);
		  data(0x3A);
		  data(0x36);
		  data(0x0B);
		  data(0x0D);
		  data(0x06);
		  data(0x4C);
		  data(0x91);
		  data(0x31);
		  data(0x08);
		  data(0x10);
		  data(0x04);
		  data(0x11);
		  data(0x0C);
		  data(0x00);

		  command(0xE1);    //Set Gamma
		  data(0x00);
		  data(0x06);
		  data(0x0A);
		  data(0x05);
		  data(0x12);
		  data(0x09);
		  data(0x2C);
		  data(0x92);
		  data(0x3F);
		  data(0x08);
		  data(0x0E);
		  data(0x0B);
		  data(0x2E);
		  data(0x33);
		  data(0x0F);

		  command(0x11);    //Exit Sleep
		  delay_ms(150);              // 120mS

		  command(0x29);    //Display on

#endif

#ifdef init4
	command(0xCF);
		  data(0x00);
		  data(0x81);
		  data(0x30);

		  command(0xED);
		  data(0x64);
		  data(0x03);
		  data(0x12);
		  data(0x81);

		  command(0xE8);
		  data(0x85);
		  data(0x10);
		  data(0x78);

		  command(0xCB);
		  data(0x39);
		  data(0x2C);
		  data(0x00);
		  data(0x34);
		  data(0x02);

		  command(0xF7);
		  data(0x20);

		  command(0xEA);
		  data(0x00);
		  data(0x00);

		  command(0xB1);
		  data(0x00);
		  data(0x1B);

		  command(0xB6);
		  data(0x0A);
		  data(0xA2);

		  command(0xC0);
		  data(0x21);

		  command(0xC1);
		  data(0x11);

		  command(0xC5);
		  data(0x3F);
		  data(0x3C);

		  command(0xC7);
		  data(0xAB);

		  command(0x36);
		  data(0x48);

		  command(0xF2);
		  data(0x00);

		  command(0x26);
		  data(0x01);


		  command(0xE0);    //Set Gamma
		  data(0x0F);
		  data(0x27);
		  data(0x25);
		  data(0x0A);
		  data(0x0E);
		  data(0x09);
		  data(0x56);
		  data(0x98);
		  data(0x49);
		  data(0x07);
		  data(0x10);
		  data(0x03);
		  data(0x31);
		  data(0x30);
		  data(0x00);

		  command(0xE1);    //Set Gamma
		  data(0x00);
		  data(0x18);
		  data(0x1A);
		  data(0x05);
		  data(0x11);
		  data(0x06);
		  data(0x29);
		  data(0x67);
		  data(0x36);
		  data(0x08);
		  data(0x0F);
		  data(0x0C);
		  data(0x0E);
		  data(0x0F);
		  data(0x0F);

		  command(0x11);    //Exit Sleep
		  delay_ms(150);              // 120mS

		  command(0x29);    //Display on

#endif

#ifdef init5
	command(0xCF);
		  data(0x00);
		  data(0x81);
		  data(0x30);

		  command(0xED);
		  data(0x64);
		  data(0x03);
		  data(0x12);
		  data(0x81);

		  command(0xE8);
		  data(0x85);
		  data(0x10);
		  data(0x78);

		  command(0xCB);
		  data(0x39);
		  data(0x2C);
		  data(0x00);
		  data(0x34);
		  data(0x02);

		  command(0xF7);
		  data(0x20);

		  command(0xEA);
		  data(0x00);
		  data(0x00);

		  command(0xB1);
		  data(0x00);
		  data(0x1B);

		  command(0xB6);
		  data(0x0A);
		  data(0xA2);

		  command(0xC0);
		  data(0x21);

		  command(0xC1);
		  data(0x11);

		  command(0xC5);
		  data(0x3F);
		  data(0x3C);

		  command(0xC7);
		  data(0xB5);

		  command(0x36);
		  data(0x48);

		  command(0xF2);
		  data(0x00);

		  command(0x26);
		  data(0x01);


		  command(0xE0);    //Set Gamma
		  data(0x0F);
		  data(0x26);
		  data(0x24);
		  data(0x0B);
		  data(0x0E);
		  data(0x09);
		  data(0x54);
		  data(0xA8);
		  data(0x46);
		  data(0x0C);
		  data(0x17);
		  data(0x09);
		  data(0x0F);
		  data(0x07);
		  data(0x00);

		  command(0xE1);    //Set Gamma
		  data(0x00);
		  data(0x19);
		  data(0x1B);
		  data(0x04);
		  data(0x10);
		  data(0x07);
		  data(0x2A);
		  data(0x47);
		  data(0x39);
		  data(0x03);
		  data(0x06);
		  data(0x06);
		  data(0x30);
		  data(0x38);
		  data(0x0F);

		  command(0x11);    //Exit Sleep
		  delay_ms(150);              // 120mS

		  command(0x29);    //Display on

#endif

#ifdef init6
	command(0xCF);
		  data(0x00);
		  data(0x83);
		  data(0x30);

		  command(0xED);
		  data(0x64);
		  data(0x03);
		  data(0x12);
		  data(0x81);

		  command(0xE8);
		  data(0x85);
		  data(0x01);
		  data(0x79);

		  command(0xCB);
		  data(0x39);
		  data(0x2C);
		  data(0x00);
		  data(0x34);
		  data(0x02);

		  command(0xF7);
		  data(0x20);

		  command(0xEA);
		  data(0x00);
		  data(0x00);

		  command(0xC0);
		  data(0x1D);

		  command(0xC1);
		  data(0x11);

		  command(0xC5);
		  data(0x33);
		  data(0x34);

		  command(0xC7);
		  data(0xBE);

		  command(0x36);
		  data(0x08);

		  command(0xB1);
		  data(0x00);
		  data(0x1B);

		  command(0xB6);
		  data(0x0A);
		  data(0xA2);

		  command(0xF2);
		  data(0x00);

		  command(0x26);
		  data(0x01);

		  command(0xE0);    //Set Gamma
		  data(0x0F);
		  data(0x23);
		  data(0x1F);
		  data(0x09);
		  data(0x0F);
		  data(0x08);
		  data(0x4B);
		  data(0xF2);
		  data(0x38);
		  data(0x09);
		  data(0x13);
		  data(0x03);
		  data(0x12);
		  data(0x07);
		  data(0x04);

		  command(0xE1);    //Set Gamma
		  data(0x00);
		  data(0x1D);
		  data(0x20);
		  data(0x02);
		  data(0x11);
		  data(0x07);
		  data(0x34);
		  data(0x81);
		  data(0x46);
		  data(0x06);
		  data(0x0E);
		  data(0x0C);
		  data(0x32);
		  data(0x38);
		  data(0x0F);

		  command(0x11);    //Exit Sleep
		  delay_ms(150);              // 120mS

		  command(0x29);    //Display on

#endif

}

void Ili9341::setPage (uint16_t y1, uint16_t y2)
{
	command (ili9341Commands::pageAddressSet);
	data (y1>>8);
	data (y1&0xFF);
	data (y2>>8);
	data (y2&0xFF);
}

void Ili9341::setColoumn (uint16_t x1, uint16_t x2)
{
	command (ili9341Commands::coloumnAddressSet);
	data (x1>>8);
	data (x1&0xFF);
	data (x2>>8);
	data (x2&0xFF);
}

void Ili9341::setPosition (uint16_t x, uint16_t y)
{
	setColoumn (x, x);
	setPage (y, y);
	command (ili9341Commands::memoryWrite);
}

void Ili9341::pixel (const uint16_t color)
{
	command (ili9341Commands::memoryWrite);
	driver->setFrameSize(Spi::Size::bit16);
	data16 (color);
	driver->setFrameSize(Spi::Size::bit8);
}

