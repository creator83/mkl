#include "systimer0x.h"


Systimer::Systimer(mode m, uint16_t val)
{
	
	switch (static_cast <uint8_t> (m))
	{
		case 0: 
		{
			uint32_t tick = Tact::getFrqCpu()/1000*val;
			SysTick_Config (tick);
		}
		break;
		case 1:
		{
			uint32_t tick = Tact::getFrqCpu()*val;
			SysTick_Config (tick);
		}
		break;
		case 2:
			SysTick_Config (Tact::getFrqCpu()*1000*val);
		break;	
	}
}

void Systimer::interruptEnable ()
{
	
}

void Systimer::interruptDisable ()
{
}
