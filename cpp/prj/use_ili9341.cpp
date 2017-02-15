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


int main()
{
	Pin cs (Gpio::Port::E, 16, Gpio::mux::Alt2);
	Pin clk (Gpio::Port::E, 17, Gpio::mux::Alt2);
	Pin mosi (Gpio::Port::E, 18, Gpio::mux::Alt2);
	Pin miso (Gpio::Port::E, 19, Gpio::mux::Alt2);
	Ili9341 display (spi0, Gpio::Port::E, 20, Gpio::Port::E, 21);
	/*Dma dma0 (Dma::dmaChannel::ch0);
	dma0.enableDmaMux(Dma::dmaMux::spi0Tx);
	display.setDma(dma0);*/
	display.fillScreen(colors16bit::BLACK);
	delay_ms(500);
	while (1)
	{
		display.fillScreen(colors16bit::RED);
		delay_ms(500);
		display.fillScreen(colors16bit::GREEN);
		delay_ms(500);

	}
}
