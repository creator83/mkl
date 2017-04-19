#include "tftdriver.h"
#include "ili9341.h"
#include "flashspi.h"


#ifndef LCDFLASH_H
#define LCDFLASH_H


class Lcdflash: public Tftdriver
{
//variables
private:
	Ili9341 * lcdDriver;
	Flash * memory;
//functions
public:
	Lcdflash(Ili9341 &, Flash &);

	void drawArr (uint16_t x , uint16_t y, const uint16_t color, const uint16_t fon, const uint8_t *arr, uint16_t l, uint16_t width) override;
	void drawPic (uint16_t x , uint16_t y, const uint16_t *arr, uint16_t length, uint16_t height) override;
};

#endif
