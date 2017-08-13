#include "device.h" // Device header
#include "mcg0x.h"
#include "lptmr.h"
#include "pin0x.h"

Tact frq (Tact::mode::fei);
//Lptmr timer;	

Lptmr timer (Lptmr::source::mcgirclk);
Pin led (Gpio::Port::A, 13);

extern "C"
{
	void SysTick_Handler(void);
	void LPTimer_IRQHandler (void);
}
void LPTimer_IRQHandler ()
{
	timer.clearTCF ();
	led.togle ();
}

void SysTick_Handler()
{


}



int main()
{
	MCG->C1 |=MCG_C1_IRCLKEN_MASK ;
	timer.setComp (10);
	timer.interruptEnable ();
	timer.start ();
	while (1)
	{

	}
}
