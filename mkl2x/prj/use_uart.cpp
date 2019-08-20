#include "device.h"                    // Device header
#include "pin.h"
#include "uart.h"
#include "spi.h"


int main ()
{
	Gpio portA (Device::port::A);
	Pin uartTx (Device::port::B, 2);
	Pin uartRx (Device::port::B, 3);
	Uart uart2 (Device::uart::uart2, Uart::baud::baud9600, uartTx, uartRx);
	uart2.transmit ("Hello from Freescale!!!");
	Spi spi0 (Device::spi::spi0);
	while (1)
	{
		
		
	}
}
