#include "tbutton.h"

Tbutton::Tbutton ()
:Xmin (300), Ymin (300), dX (3300), dY(3300)
{
}

void Tbutton::setCount (uint8_t hor, uint8_t ver)
{
	hValue = hor;
	vValue = ver;
	xValue = dX/hValue;
	yValue = dY/vValue;
}

void Tbutton::calculateTouch (uint16_t x, uint16_t y)
{
	result = (x-Xmin)/xValue;
	result <<= 1;
	uint8_t temp = (y-Ymin)/yValue;
	result += temp;
}

uint8_t & Tbutton::getResult ()
{
	return result;
}
