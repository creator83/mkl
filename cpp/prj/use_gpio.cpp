#include "MKL26Z4.h"                   // Device header
#include "gpio.h"


const char led = 16;

int main ()
{
	gpio pin (gpio::E);
	pin.setOutPin(led);
	pin.setPin(led);


	while (1)
	{

	}
}
