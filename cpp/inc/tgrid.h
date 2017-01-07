#include "MKL26Z4.h"                 // Device header
#include "xpt2046.h"



#ifndef TGRID_H
#define TGRID_H

class Tgrid
{

//variables
public:
private:
  uint16_t xGrid, yGrid;
  Xpt2046 * driver;
//func
public:
  Tgrid ( Xpt2046 &, uint8_t hor, uint8_t ver);
  uint16_t & getXgrid ();
  uint16_t & getYgrid ();
  Xpt2046 * getDriver ();
};

#endif
