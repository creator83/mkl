#include "MKL26Z4.h"                   // Device header
#include "gpio.h"
#include "pin.h"
#include "port.h"
#include "tact.h"
#include "delay.h"


const char ledPin = 16;

int main ()
{
  Tact frq;
	Pin led (Gpio::A, ledPin);

	while (1)
	{
		led.togle();
		delay_ms(1000);
	}
}
