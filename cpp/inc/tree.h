#include "MKL26Z4.h"
#include "list.h"

#ifndef TREE_H
#define TREE_H

class Tree {
struct Item
{
	List * object;
	Item * next;
	Item (List *o, Item * n = nullptr){object = o; next = n;}
};

	Item * first;
	Item * last;
	void (*function)();
	uint16_t count;
public:
	Tree(){count = 0; first = last = nullptr;}
	~Tree();
	List* head () const {return first->object;}
	List* tail () const {return last->object;}
	void addFirst (List *);
	void addLast (List *);
	void iterate ();
	void print (uint16_t);
	uint16_t & getCount (){return count;}
	void setFunction (void (*f)());
};



#endif /* LIST_H_ */
