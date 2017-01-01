#include "MKL26Z4.h"                // Device header
#include "tact.h"
#include "delay.h"
#include "ssd1289.h"
#include "shape.h"
#include "pixel.h"
#include "verline.h"
#include "horline.h"
#include "rectangle.h"
#include "colors16bit.h"
#include "font.h"
#include "list.h"
#include "mpic.h"
#include "shift.h"
#include "spi.h"
#include "xpt2046.h"
#include "buffer.h"
#include "nrf24l01.h"
#include "tbutton.h"

Tact frq;
Spi spi1 (Spi::SPI_N::SPI_1);
Xpt2046 touch (spi1);
Ssd1289::sFont bNum;
Buffer value;
Ssd1289 display;
Tbutton mainScreen;

struct flags
{
  unsigned touch :1;
  unsigned mainScreen :1;
  unsigned subScreen :2;
}flag;

extern "C" {
	void SysTick_Handler();
	void PIT_CH1_IRQHandler();
	void IRQ_IRQHandler();
	void PORTA_IRQHandler();
}

void PORTA_IRQHandler()
{
	touch.clearFlag();
	flag.touch ^= 1;
	if (flag.touch)
	{
		touch.getData();
		value.parsDec16(touch.getX(), 4);
		display.string(150, 200, colors16bit::BLACK, colors16bit::SILVER, value.getElement(1), bNum, 4, 4);
		value.parsDec16(touch.getY(), 4);
		display.string(150, 100, colors16bit::BLACK, colors16bit::SILVER, value.getElement(1), bNum, 4, 4);
		mainScreen.calculateTouch(touch.getX(), touch.getY());
		display.symbol(10,200, colors16bit::BLACK, colors16bit::SILVER, mainScreen.getResult(), bNum);
	}
}


int main()
{
	mainScreen.setCount(3,2);
	value.setFont(Array_dec);

	Shape::driver = &display;
	display.fillScreen(colors16bit::SILVER);

	Ssd1289::sFont mTimes;
	bNum.font = bigNumbers::numbers;
	bNum.height = 55;
	bNum.width = 3;
	bNum.shift = 0;
	mTimes.font = midlleTimesNewRomanRus::rus;
	mTimes.width = 2;
	mTimes.height = 17;
	mTimes.shift = 0;

	//display.symbol (50,50,  colors16bit::BLACK, colors16bit::GRAY);

	spi1.setMode(Spi::Mode::software);
	Pin sck (Gpio::Port::E, 2, Gpio::mux::Alt2);
	Pin mosi (Gpio::Port::E, 1, Gpio::mux::Alt2);
	Pin miso (Gpio::Port::E, 3, Gpio::mux::Alt2);



	while (1)
	{

	}
}
