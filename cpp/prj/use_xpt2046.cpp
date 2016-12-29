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

Tact frq;



extern "C" {
	void SysTick_Handler();
	void PIT_CH1_IRQHandler();
	void IRQ_IRQHandler();
}



int main()
{
	Ssd1289 display;
	Shape::driver = &display;
	display.fillScreen(colors16bit::SILVER);
	Ssd1289::sFont num;
	num.font = bigNumbers::numbers;
	num.height = 55;
	num.width = 3;
	num.shift = 0;
	display.symbol (50,50,  colors16bit::BLACK, colors16bit::GRAY, 0, num);

	while (1)
	{

	}
}
