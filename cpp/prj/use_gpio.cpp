#include "MKL26Z4.h"                   // Device header
#include "gpio.h"
#include "pin.h"
#include "port.h"

const char ledPin = 16;

int main ()
{
	Pin led (Gpio::Port::A, ledPin);
	led.set();
	led.togle();

	while (1)
	{

	}
}
