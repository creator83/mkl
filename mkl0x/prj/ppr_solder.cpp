#include "device.h"                   // Device header
#include "mcg0x.h"
#include "adc0x.h"
#include "segled.h"

Tact frq (Tact::mode::fei);

Segled indicator (4);

extern "C"
{
	//void PIT_IRQHandler();
}


int main()
{


	while (1)
	{

	}
}
