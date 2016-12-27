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
#include "image.h"

Tact frq;


Shape * layer1[3];


extern "C" {
	void SysTick_Handler();
	void PIT_CH1_IRQHandler();
	void IRQ_IRQHandler();
}

const uint8_t numbers[] = {
    0xff,
    0xff,
    0xff,
    0xff,
    0xc7,
    0x93,
    0x39,
    0x39,
    0x39,
    0x39,
    0x39,
    0x39,
    0x39,
    0x93,
    0xc7,
    0xff,
    0xff,
    0xff,
    0xff,

    0xff,
    0xff,
    0xff,
    0xff,
    0x87,
    0xe7,
    0xe7,
    0xe7,
    0xe7,
    0xe7,
    0xe7,
    0xe7,
    0xe7,
    0xe7,
    0x81,
    0xff,
    0xff,
    0xff,
    0xff,

    0xff,
    0xff,
    0xff,
    0xff,
    0xc7,
    0x83,
    0x33,
    0xf3,
    0xf3,
    0xf7,
    0xe7,
    0xef,
    0xdd,
    0x83,
    0x03,
    0xff,
    0xff,
    0xff,
    0xff,

    0xff,
    0xff,
    0xff,
    0xff,
    0xc7,
    0x83,
    0x73,
    0xf3,
    0xe7,
    0xc3,
    0xf1,
    0xf9,
    0xf9,
    0x3b,
    0x07,
    0xff,
    0xff,
    0xff,
    0xff,

    0xff,
    0xff,
    0xff,
    0xff,
    0xfb,
    0xf3,
    0xe3,
    0xe3,
    0xd3,
    0xb3,
    0x73,
    0x01,
    0x01,
    0xf3,
    0xf3,
    0xff,
    0xff,
    0xff,
    0xff,

    0xff,
    0xff,
    0xff,
    0xff,
    0xc1,
    0x83,
    0xbf,
    0x8f,
    0x03,
    0xf1,
    0xf9,
    0xfd,
    0xfd,
    0x1b,
    0x07,
    0xff,
    0xff,
    0xff,
    0xff,

    0xff,
    0xff,
    0xff,
    0xff,
    0xf1,
    0xc7,
    0x9f,
    0x9f,
    0x07,
    0x33,
    0x39,
    0x39,
    0x39,
    0x93,
    0xc7,
    0xff,
    0xff,
    0xff,
    0xff,

    0xff,
    0xff,
    0xff,
    0xff,
    0xc0,
    0x81,
    0xbd,
    0xfd,
    0xfb,
    0xfb,
    0xfb,
    0xf7,
    0xf7,
    0xf7,
    0xef,
    0xff,
    0xff,
    0xff,
    0xff,

    0xff,
    0xff,
    0xff,
    0xff,
    0x83,
    0x39,
    0x39,
    0x19,
    0x83,
    0xc3,
    0xb1,
    0x39,
    0x39,
    0x39,
    0x83,
    0xff,
    0xff,
    0xff,
    0xff,

    0xff,
    0xff,
    0xff,
    0xff,
    0xc7,
    0x93,
    0x39,
    0x39,
    0x39,
    0x99,
    0xc1,
    0xf3,
    0xf3,
    0xc7,
    0x1f,
    0xff,
    0xff,
    0xff,
    0xff
};


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
	const uint8_t * m[]= {images::alarm, images::home, images::idea, images::pump, images::settings, images::support};

int main()
{
	Ssd1289 display;
	Ssd1289::sFont num;
	num.font = numbers;
	num.height = 19;
	num.width = 1;
	num.shift = 0x30;
	Shape::driver = &display;
	 mainScreen (display);

	 //uint16_t n = sizeof ArialBlack_8_data/sizeof *ArialBlack_8_data;
	 display.drawArr (30, 230, colors16bit::BLACK, colors16bit::GRAY, images::alarm, 12, 96);
	 display.symbol(200,200, colors16bit::YELLOW, colors16bit::BLACK, '0', num);
	 delay_ms(2000);
	while (1)
	{

		const uint8_t n = sizeof m/sizeof *m;
		for (uint8_t i=0;i<n;++i)
		{
			display.drawArr (190, 230, colors16bit::BLACK, colors16bit::GRAY, m[i], 12, 96);
			delay_ms(1000);
		}

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




