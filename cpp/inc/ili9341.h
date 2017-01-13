#include "MKL26Z4.h"
#include "delay.h"
#include "spi.h"
#include "pin.h"

#ifndef ILI9341_H
#define ILI9341_H

namespace ili9341Commands
{
	const uint8_t softwareReset = 0x01;
	const uint8_t powerControl1 = 0xC0;
	const uint8_t powerControl2 = 0xC1;
	const uint8_t vcomControl1  = 0xC5;
	const uint8_t vcomControl2  = 0xC7;
	const uint8_t frameControl  = 0xB1;
	const uint8_t verticalScroll2 = 0x0042;

}


class Ili9341
{
//variables
public:
	struct sFont
	{
		const uint8_t * font;
		uint8_t width;
		uint8_t height;
		uint8_t shift;
	};
protected:
private:
	Spi * driver;
	Pin dc;
//functions
public:
	Ili9341(Spi &, Gpio::Port po, uint8_t p);
	void pixel (uint16_t x , uint16_t y, const uint16_t color);
	void fillScreen (uint16_t color);
	void symbol (uint16_t x, uint16_t y, const uint16_t color, const uint16_t fon, const uint8_t ch, sFont & s);
	//void symbol (uint16_t x, uint16_t y, const uint16_t color, const uint16_t fon, uint8_t ch, sFont & s);
	void string (uint16_t x, uint16_t y, const uint16_t color, const uint16_t fon, const char *str, sFont &f, uint8_t interval);
	void string (uint16_t x, uint16_t y, const uint16_t color, const uint16_t fon, const char *str, sFont &f, uint16_t n,uint8_t interval);
	void setCursor (uint16_t x , uint16_t y);
	void drawArr (uint16_t x , uint16_t y, const uint16_t color, const uint16_t fon, const uint8_t *arr, uint16_t l, uint16_t width);
	void drawPic (uint16_t x , uint16_t y, const uint16_t *arr, uint16_t length, uint16_t width);
	void horLine (uint16_t x, uint16_t y, const uint16_t color, uint16_t length, uint8_t thick);
	void verLine (uint16_t x, uint16_t y, const uint16_t color, uint16_t length, uint8_t thick);
	void line (uint16_t x, uint16_t y, uint16_t color, uint16_t length, uint8_t thick);
	void rectangle (uint16_t x, uint16_t y, uint16_t color, uint16_t length, uint8_t width, uint8_t thick);
protected:
private:
	void data (uint8_t);
	void command (uint8_t);
	void write (uint8_t);
	void init ();

}; //ssd1289

#endif //__SSD1289_H__
