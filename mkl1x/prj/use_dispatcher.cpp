#include "device.h"                    // Device header
//#include "mcglight.h" 
#include "mcg0x.h" 
#include "dispatcher.h"


Tact frq;


void f1 ();
void f2 ();
void f3 ();
void f4 ();

int main ()
{
	Dispatcher turn;
	turn.addTask (f1, 10);
	turn.addTask (f2, 15);
	turn.addTask (f3, 20);
	turn.addTask (f4, 25);
	while (1)
	{
		turn.tickTask();
		turn.checkTasks();
		turn.checkQueue();
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


