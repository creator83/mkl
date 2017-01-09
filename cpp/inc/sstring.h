#include "MKL26Z4.h"               // Device header
#include "shape.h"
#include "colors16bit.h"

#ifndef SSTRING_H
#define SSTRING_H

class Sstring : public Shape
{
private:
	uint16_t x, y, color, fon;
	char * str;
	Ssd1289::sFont * font_;
	uint8_t interval;
public:
	Sstring (uint16_t x_, uint16_t y_, uint16_t color_, uint16_t fon_, char *str_, Ssd1289::sFont *f, uint8_t interval_);
	~Sstring ();
	void draw () const override;

};

#endif
