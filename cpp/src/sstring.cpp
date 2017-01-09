#include "sstring.h"

Sstring::Sstring (uint16_t x_, uint16_t y_, uint16_t color_, uint16_t fon_, char *str_, Ssd1289::sFont * f, uint8_t interval_)
:x(x_), y(y_), color (color_), fon (fon_), interval (interval_)
{
	font_ = f;
	uint8_t l=0;
	while (*str_--)++l;
	str = new char [l+1];
	while (*str++=*str_++);
}

Sstring::~Sstring ()
{
	delete [] str;
}

void Sstring::draw () const
{
	driver->string(x, y, color, fon, str, *font_, interval);
}
