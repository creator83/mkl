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

Tact frq;


Shape * layer1[3];


extern "C" {
	void SysTick_Handler();
	void PIT_CH1_IRQHandler();
	void IRQ_IRQHandler();
}

const uint8_t ArialBlack_8_data[] = {
0x00,0x00,0x20,0x05,0x08,0x25,0xE2,0x30,0x0A,0x62};

enum class color: uint8_t {red, green, blue};

union rgb24
{
	uint32_t code;
	uint8_t color[3];
};


void mainScreen (Ssd1289 &);

void screen ();
void drawScreen (Shape **, uint8_t n);

const uint16_t colors [] = {colors16bit::BLACK, colors16bit::RED, colors16bit::BLUE,  colors16bit::GREEN, colors16bit::CYAN, colors16bit::MAGENTA,  colors16bit::YELLOW, colors16bit::WHITE,
 colors16bit::GRAY, colors16bit::SKY, colors16bit::ORANGE,  colors16bit::PINK, colors16bit::BROWN,colors16bit::VIOLET, colors16bit::SILVER,
 colors16bit::GOLD,  colors16bit::BEGH, colors16bit::NAVY, colors16bit::DARK_GREEN, colors16bit::DARK_CYAN, colors16bit::MAROON,  colors16bit::PURPLE,
colors16bit::LIGHT_GREY,  colors16bit::DARK_GREY};


int main()
{
	Ssd1289 display;
	Ssd1289::sFont lat;
	Shape::driver = &display;
	 mainScreen (display);
	 uint16_t n = sizeof ArialBlack_8_data/sizeof *ArialBlack_8_data;
	 //display.drawArr(50,50,colors16bit::BLACK, colors16bit::YELLOW, ArialBlack_8_data, n);
	/*display.fillScreen(colors16bit::BLACK);
	display.horLine(50, 0, colors16bit::WHITE, 320, 10);

	Horline hLine (120, 0, colors16bit::WHITE, 320, 5);
	Verline vline (0, 100, colors16bit::WHITE, 200, 5);

		layer1[0] = &hLine;
		layer1[1] = &vline;
		for (uint8_t k=0;k<2;++k)
		{
			layer1[k]->draw();
		}
	uint16_t n = sizeof layer1/sizeof *layer1[0];
	drawScreen (layer1, 2);*/
	while (1)
	{
		/*const uint8_t n = sizeof colors/sizeof *colors;
		for (uint8_t i=0;i<n;++i)
		{
			display.fillScreen(colors [i]);
			delay_ms(1000);
		}*/

	}
}

void mainScreen (Ssd1289 & d)
{
	d.fillScreen(colors16bit::SILVER);



	d.verLine(160, 0, colors16bit::BLACK, 240, 1);
	d.horLine(0, 120, colors16bit::BLACK, 320, 1);


	//gradient
	d.rectangle(5,5, colors16bit::BLACK,150, 110, 1);
	d.rectangle(165,5, colors16bit::BLACK,150, 110, 1);
	d.rectangle(5,125, colors16bit::BLACK,150, 110, 1);
	d.rectangle(165,125, colors16bit::BLACK,150, 110, 1);
	d.horLine(6, 6, colors16bit::GRAY, 149, 109);
	d.horLine(166, 6, colors16bit::GRAY, 149, 109);
	d.horLine(6, 126, colors16bit::GRAY, 149, 109);
	d.horLine(166, 126, colors16bit::GRAY, 149, 109);
}

void screen ()
{
	Horline hLine (120, 0, colors16bit::WHITE, 320, 5);
	Verline vline (0, 100, colors16bit::WHITE, 200, 5);

	layer1[0] = &hLine;
	layer1[1] = &vline;

}

void drawScreen (Shape  ** scr, uint8_t n)
{
	//uint16_t m = sizeof *scr/sizeof **scr[0];
	for (uint8_t i=0 ;i<n;++i)
	{
		(*scr++)->draw();
	}
}




