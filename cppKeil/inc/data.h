#include "MKL26Z4.h"                 // Device header
#include "shape.h"
#include "colors16bit.h"


#ifndef DATA_H
#define DATA_H

class Data: public Shape
{

//variables
public:
private:
	uint16_t value, x, y, color, fon;
	uint8_t * data;
	Ssd1289::sFont * font_;
	uint8_t nElements;
	static const uint8_t point [8];

public:
	Data (uint16_t d, uint8_t n, Ssd1289::sFont *);
	Data (uint16_t x_, uint16_t y_, uint16_t color_,uint16_t fon_, uint16_t d, uint8_t n, Ssd1289::sFont *);


	void convert ();
	void setPosition (uint16_t, uint16_t);
	void setValue (uint16_t);
	void draw () const override;
	~Data ();
};

#endif
