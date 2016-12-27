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
#include "list.h"
#include "mpic.h"
Tact frq;


Horline hLine1 (0, 50, colors16bit::WHITE, 190, 5);
Horline hLine2 (0, 70, colors16bit::WHITE, 190, 5);
Verline vline1 (100, 0, colors16bit::WHITE, 200, 5);
MonoPicture tools (30, 228, colors16bit::GRAY, colors16bit::BLACK, images::support, 12, 96);
MonoPicture settings (30, 108, colors16bit::GRAY, colors16bit::BLACK, images::settings, 12, 96);
MonoPicture watch (190, 228, colors16bit::GRAY, colors16bit::BLACK, images::alarm, 12, 96);
MonoPicture pump (30, 228, colors16bit::GRAY, colors16bit::BLACK, images::pump, 12, 96);
MonoPicture boiler (190, 228, colors16bit::GRAY, colors16bit::BLACK, images::boiler1, 12, 96);
List mScreen;
List equipment;

extern "C" {
	void SysTick_Handler();
	void PIT_CH1_IRQHandler();
	void IRQ_IRQHandler();
}



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

List * Screens [] = {&mScreen, &equipment};
const uint8_t number[] = {
    0xff, 0xff, 0xff,
    0xff, 0xff, 0xff,
    0xff, 0xff, 0xff,
    0xff, 0xff, 0xff,
    0xff, 0xff, 0xff,
    0xff, 0xff, 0xff,
    0xff, 0xff, 0xff,
    0xff, 0xff, 0xff,
    0xff, 0xff, 0xff,
    0xff, 0xff, 0xff,
    0xff, 0xff, 0xff,
    0xff, 0xff, 0xff,
    0xff, 0x83, 0xff,
    0xfe, 0x00, 0xff,
    0xf8, 0x38, 0x3f,
    0xf0, 0x7c, 0x1f,
    0xf0, 0x7e, 0x1f,
    0xe0, 0xfe, 0x0f,
    0xe0, 0xfe, 0x0f,
    0xc0, 0xfe, 0x07,
    0xc0, 0xfe, 0x07,
    0xc0, 0xfe, 0x07,
    0xc0, 0xfe, 0x07,
    0x80, 0xfe, 0x03,
    0x80, 0xfe, 0x03,
    0x80, 0xfe, 0x03,
    0x80, 0xfe, 0x03,
    0x80, 0xfe, 0x03,
    0x80, 0xfe, 0x03,
    0x80, 0xfe, 0x03,
    0x80, 0xfe, 0x03,
    0x80, 0xfe, 0x03,
    0x80, 0xfe, 0x03,
    0xc0, 0xfe, 0x07,
    0xc0, 0xfe, 0x07,
    0xc0, 0xfe, 0x07,
    0xc0, 0xfe, 0x07,
    0xe0, 0xfe, 0x0f,
    0xe0, 0xfe, 0x0f,
    0xf0, 0x7e, 0x1f,
    0xf8, 0x7c, 0x1f,
    0xf8, 0x38, 0x3f,
    0xfe, 0x00, 0xff,
    0xff, 0x83, 0xff,
    0xff, 0xff, 0xff,
    0xff, 0xff, 0xff,
    0xff, 0xff, 0xff,
    0xff, 0xff, 0xff,
    0xff, 0xff, 0xff,
    0xff, 0xff, 0xff,
    0xff, 0xff, 0xff,
    0xff, 0xff, 0xff,
    0xff, 0xff, 0xff,
    0xff, 0xff, 0xff,
    0xff, 0xff, 0xff};
int main()
{
	mScreen.addLast(&tools);
	mScreen.addLast(&settings);
	mScreen.addLast(&watch);
	equipment.addLast(&pump);
	equipment.addLast(&boiler);
	Ssd1289 display;
	Ssd1289::sFont num;
	num.font = number;
	num.height = 55;
	num.width = 3;
	num.shift = 0;
	Shape::driver = &display;
	display.symbol(50,50,  colors16bit::BLACK, colors16bit::GRAY, 0, num);



	while (1)
	{
		for (uint8_t i=0;i<2;++i)
		{
			mainScreen (display);
			Screens [i]->iterate();
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





