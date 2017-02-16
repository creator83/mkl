#include "MKL26Z4.h"                 // Device header
#include "tgrid.h"




#ifndef TBUTTON_H
#define TBUTTON_H

class Tbutton
{

struct Item
{
	uint16_t key;
	void (*function)();
	Item * next;
	Item (uint16_t k, void (*f)(), Item * n = nullptr){key = k; function = f; next = n;}
};
//variables
public:
private:
  uint16_t xValue, yValue;
  uint8_t hValue, vValue, result;
  Item * first;
  Item * last;
  uint16_t count;
  Tgrid * grid;
  Item * current;
//func
public:
  Tbutton (Tgrid &);
  ~Tbutton ();
  void addButton (uint16_t,  void (*f)());
  void setCount (uint8_t hor, uint8_t ver);
  void calculateTouch ();
  uint8_t & getResult ();
  void setCurrent ();
private:
  void searchKey (uint8_t k);

};

#endif
