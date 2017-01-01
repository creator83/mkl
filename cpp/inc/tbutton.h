#include "MKL26Z4.h"                 // Device header



#ifndef BUTTON_H
#define BUTTON_H

class Tbutton
{
//variables
public:
private:
  uint16_t Xmin, Ymin, dX, dY, xValue, yValue;
  uint8_t hValue, vValue, result;

//func
public:
  Tbutton ();
  void setCount (uint8_t hor, uint8_t ver);
  void calculateTouch (uint16_t x, uint16_t y);
  uint8_t & getResult ();
  void setshortPressAction (void (*f)());
  void setlongPressAction (void (*f)());
};

#endif
