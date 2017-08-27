#include "device.h"                    // Device header
#include "mcglight.h" 
#include "dispatcher.h"


Tact frq;
Dispatcher turn;

void f1 ();
void f2 ();
void f3 ();
void f4 ();

int main ()
{
	turn.addTask (f1, 10);
	turn.addTask (f2, 15);
	turn.addTask (f3, 20);
	turn.addTask (f4, 25);
	while (1)
	{
		
	}
}


void f1 ()
{
}
void f2 ()
{
}
void f3 ()
{
}
void f4 ()
{
}


