#include "MKL26Z4.h"                // Device header
#include "tact.h"
#include "delay.h"
#include "ili9341.h"
#include "shape.h"
#include "pixel.h"
#include "verline.h"
#include "horline.h"
#include "rectangle.h"
#include "colors16bit.h"
#include "image.h"
#include "list.h"
#include "mpic.h"

Tact frq;
Spi spi0 (Spi::SPI_N::SPI_0);
Ili9341 display (spi0, Gpio::Port::E, 20);

int main()
{

	while (1)
	{

	}
}
