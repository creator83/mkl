#include "MKL26Z4.h"                // Device header
#include "ssd1289.h"


#ifndef SHAPE_H
#define SHAPE_H

class Shape
{
public:

	static Ssd1289 * driver;
public:
	virtual void draw () const = 0;
	virtual void setPosition (uint16_t x, uint16_t y)  = 0;
	virtual ~Shape(){}
};




#endif
