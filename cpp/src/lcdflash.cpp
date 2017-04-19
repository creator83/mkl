#include "lcdflash.h"


Lcdflash::Lcdflash(Ili9341 &lcd, Flash &mem)
{
	lcdDriver = &lcd;
	memory = &mem;
}
