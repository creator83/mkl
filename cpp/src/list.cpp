#include "list.h"

List::~List()
{
	Item * current = nullptr;
	Item * next = first;
	while (next->next != nullptr)
	{
		current = next;
		next = next->next;
		delete current;
	}
	current = last;
	delete current;
}



void List::addFirst (Shape * o)
{
	Item * newItem = new Item (o, first);
	if (first == nullptr)
	{
		last = newItem;
	}
	first = newItem;
	++count;
}

void List::addLast (Shape * o)
{
	Item * newItem = new Item (o);
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

void List::iterate ()
{
	//Item * current = nullptr;
	Item * next = first;
	//uint16_t counter = count;
	function ();
	for (uint16_t i=0;i<count;++i)
	{
		next->object->draw();
		next = next->next;
	}
}

void List::print (uint16_t n)
{
	Item * current = nullptr;
	Item * next = first;
	while (n--)
	{
		current = next;
		next = next->next;
	}
	next->object->draw();
}

void List::setFunction (void (*f)())
{
	function = f;
}

