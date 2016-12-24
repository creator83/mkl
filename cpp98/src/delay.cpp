#include "delay.h"

void delay_ms (uint16_t del)
{
	Pit ms (Pit::ch0, del, Pit::ms);
	ms.start();
	while (!(ms.flag_TIF()));
	ms.stop();
	ms.clear_flag();
}
void delay_us (uint16_t del)
{
	Pit us (Pit::ch0, del, Pit::us);
	us.start();
	while (!(us.flag_TIF()));
	us.stop();
	us.clear_flag();
}
