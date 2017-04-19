#include "cpic.h"

ColorPicture::ColorPicture (uint16_t x_, uint16_t y_, const uint16_t * pic, uint16_t length_, uint16_t width_)
:x(x_), y (y_), length (length_), width (width_), arr (pic)
{
}


void ColorPicture::draw () const
{
	displayDriver->drawPic (x, y, arr, length, width);
}

void ColorPicture::setPosition (uint16_t x_, uint16_t y_)
{
	x = x_;
	y = y_;
}
