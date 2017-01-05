#include "tbutton.h"

Tbutton::Tbutton ( Tgrid & g)
:count (0), first (nullptr), last (nullptr)
{
	grid = &g;
}

Tbutton::~Tbutton ()
{
	Item * current = nullptr;
	Item * next = first;
	while (1)
	{
		current = next;
		next = next->next;
		delete current;
	}
}

void Tbutton::addButton (uint16_t k,  void (*f)())
{
	Item * newItem = new Item (k, f);
	if (last == nullptr)
	{
		first = newItem;
	}
	else
	{
		last->next = newItem;
	}
	last = newItem;
	++count;
}

void Tbutton::calculateTouch (uint16_t x, uint16_t y)
{
	result = x/grid->getXgrid();
	result <<= 1;
	uint8_t temp = y/grid->getYgrid();
	result += temp;
	searchKey(result);
}

uint8_t & Tbutton::getResult ()
{
	return result;
}

void Tbutton::searchKey (uint8_t k)
{
	Item * next = first;
	for (uint16_t i=0;i<count;++i)
	{
		if (next->key == k)
		{
			next->function ();
			return;
		}
		next = next->next;
	}

}

