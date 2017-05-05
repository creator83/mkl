#include "spisoft.h"

Spis::Spis(Pin & cs_, Pin & sck_, Pin & mosi_, Pin & miso_)
:cs(&cs_), sck (&sck_), mosi(&mosi_), miso (&miso_)
{
	cs->set();
	sck->clear();
}

void Spis::transmite (uint8_t data)
{
	cs->clear();
	for (uint8_t i=0;i<8;++i)
	{

		if (data&(1<<(7-i)))
		{
			mosi->set();
		}
		else
		{
			mosi->clear();
		}
		sck->set();
		sck->clear();
	}
	cs->set();
}
