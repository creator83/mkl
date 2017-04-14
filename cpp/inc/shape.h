#include "device.h"                // Device header
#include "tftdriver.h"


#ifndef SHAPE_H
#define SHAPE_H

class Shape
{
public:

	static Tftdriver * driver;
public:
	virtual void draw () const = 0;
	//virtual void setPosition (uint16_t x, uint16_t y)  = 0;
	virtual ~Shape(){}
};




#endif
