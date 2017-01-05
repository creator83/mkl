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
#include "spi.h"
#include "xpt2046.h"
#include "systimer.h"
#include "tbutton.h"
#include "pit.h"
#include "tgrid.h"

Tact frq;
Spi spi1 (Spi::SPI_N::SPI_1);
Xpt2046 touch (spi1);
Ssd1289 display;
Tgrid fourArea (touch, 2,2);
Tgrid sixArea (touch,3,2);
Tgrid settingsArea (touch,6,4);
Tbutton tMainScreen (fourArea);
Tbutton tSubScreens (sixArea);
//Pit mainloop (Pit::ch1, 1, Pit::ms);

Horline hLine1 (0, 50, colors16bit::WHITE, 190, 5);
Horline hLine2 (0, 70, colors16bit::WHITE, 190, 5);
Verline vline1 (100, 0, colors16bit::WHITE, 200, 5);

//main screen
MonoPicture tools (30, 228, colors16bit::GRAY, colors16bit::BLACK, bigImages::support, 12, 96);
MonoPicture outside (190, 108, colors16bit::GRAY, colors16bit::BLACK, bigImages::outside, 12, 96);
MonoPicture watch (190, 228, colors16bit::GRAY, colors16bit::BLACK, bigImages::alarm, 12, 96);
MonoPicture livingSmall (25, 113, colors16bit::GRAY, colors16bit::BLACK, smallImages::living, 5, 40);
MonoPicture bathSmall (90, 113, colors16bit::GRAY, colors16bit::BLACK, smallImages::bath, 5, 40);
MonoPicture diningSmall (25, 53, colors16bit::GRAY, colors16bit::BLACK, smallImages::dining, 5, 40);

MonoPicture home (227, 96, colors16bit::GRAY, colors16bit::BLACK, midlleImages::home, 9, 72);
MonoPicture back (227, 216, colors16bit::GRAY, colors16bit::BLACK, midlleImages::back, 9, 72);


//equipment screen
MonoPicture pump (14, 216, colors16bit::GRAY, colors16bit::BLACK, midlleImages::pump, 9, 72);
MonoPicture boiler (120, 216, colors16bit::GRAY, colors16bit::BLACK, midlleImages::level, 9, 72);
MonoPicture fan (14, 96, colors16bit::GRAY, colors16bit::BLACK, midlleImages::fan, 9, 72);
MonoPicture floor (120, 96, colors16bit::GRAY, colors16bit::BLACK, midlleImages::floor, 9, 72);

//rooms screen
MonoPicture livingBig (14, 216, colors16bit::GRAY, colors16bit::BLACK, midlleImages::living, 9, 72);
MonoPicture bathBig (120, 216, colors16bit::GRAY, colors16bit::BLACK, midlleImages::bath, 9, 72);
MonoPicture diningBig (14, 96, colors16bit::GRAY, colors16bit::BLACK, midlleImages::dining, 9, 72);


//setting screen
MonoPicture left (7, 53, colors16bit::GRAY, colors16bit::BLACK, smallImages::left, 5, 40);
MonoPicture right (59, 53, colors16bit::GRAY, colors16bit::BLACK, smallImages::right, 5, 40);
MonoPicture plus (111, 53, colors16bit::GRAY, colors16bit::BLACK, smallImages::plus, 5, 40);
MonoPicture minus (163, 53, colors16bit::GRAY, colors16bit::BLACK, smallImages::minus, 5, 40);


List mScreen;
List equipment;
List rooms;
List settingValue;

struct flags
{
  unsigned touch :1;
  unsigned mainScreen :1;
  unsigned subScreen :2;
  unsigned screens :3;
}flag;

List * Subscreens [] = { &rooms, &equipment} ;

extern "C" {
	void SysTick_Handler();
	void PIT_IRQHandler();
	void PORTA_IRQHandler();

}


void PORTA_IRQHandler()
{
	touch.clearFlag();
	flag.touch ^= 1;
	if (flag.touch)
	{
		touch.getData();
		if (flag.mainScreen)
		{
			tMainScreen.calculateTouch(touch.getX(), touch.getY());
			flag.mainScreen = 0;

			/*flag.subScreen = mainScreen.getResult();
			Subscreens [flag.subScreen]->iterate();*/
		}
		else
		{
			tSubScreens.calculateTouch(touch.getX(), touch.getY());
			flag.mainScreen = 1;
		}


	}
}



enum class color: uint8_t {red, green, blue};

union rgb24
{
	uint32_t code;
	uint8_t color[3];
};


void mainScreenFon ();
void subScreenFon ();
void settingValueFon ();

void drawMainScreen();
void drawRoomScreen();
void drawEqupmentScreen();



const uint16_t colors [] = {colors16bit::BLACK, colors16bit::RED, colors16bit::BLUE,  colors16bit::GREEN, colors16bit::CYAN, colors16bit::MAGENTA,  colors16bit::YELLOW, colors16bit::WHITE,
 colors16bit::GRAY, colors16bit::SKY, colors16bit::ORANGE,  colors16bit::PINK, colors16bit::BROWN,colors16bit::VIOLET, colors16bit::SILVER,
 colors16bit::GOLD,  colors16bit::BEGH, colors16bit::NAVY, colors16bit::DARK_GREEN, colors16bit::DARK_CYAN, colors16bit::MAROON,  colors16bit::PURPLE,
colors16bit::LIGHT_GREY,  colors16bit::DARK_GREY};

List * Screens [] = {&mScreen, &equipment, &rooms};


int main()
{
	tMainScreen.addButton(0, drawRoomScreen);
	tMainScreen.addButton(1, drawEqupmentScreen);
	tSubScreens.addButton(4,drawMainScreen);
	tSubScreens.addButton(5,drawMainScreen);
	flag.mainScreen = 1;

	spi1.setMode(Spi::Mode::software);
	Pin sck (Gpio::Port::E, 2, Gpio::mux::Alt2);
	Pin mosi (Gpio::Port::E, 1, Gpio::mux::Alt2);
	Pin miso (Gpio::Port::E, 3, Gpio::mux::Alt2);

	//main screen
	mScreen.addLast(&tools);
	mScreen.addLast(&watch);
	mScreen.addLast(&outside);
	mScreen.addLast(&livingSmall);
	mScreen.addLast(&diningSmall);
	mScreen.addLast(&bathSmall);
	mScreen.setFunction(mainScreenFon);

	//equipment screen
	equipment.addLast(&pump);
	equipment.addLast(&boiler);
	equipment.addLast(&fan);
	equipment.addLast(&floor);
	equipment.setFunction(subScreenFon);

	//rooms screen
	rooms.addLast(&livingBig);
	rooms.addLast(&diningBig);
	rooms.addLast(&bathBig);
	rooms.setFunction(subScreenFon);

	//settings Value screen
	settingValue.setFunction(settingValueFon);


	Shape::driver = &display;
	//NVIC_EnableIRQ(PIT_IRQn);
	mScreen.iterate();
	settingValue.iterate();
	//mainloop.start();
	//Systimer mainLoop (Systimer::mode::ms, 1000);
	while (1)
	{
		/*for (uint8_t i=0;i<3;++i)
		{
			Screens [i]->iterate();
			delay_ms(1000);
		}*/
	}
}

void mainScreenFon ()
{
	display.fillScreen(colors16bit::SILVER);

	display.verLine(160, 0, colors16bit::BLACK, 240, 2);
	display.horLine(0, 120, colors16bit::BLACK, 320, 2);

	//gradient
	display.rectangle(5,5, colors16bit::BLACK,150, 110, 1);
	display.rectangle(165,5, colors16bit::BLACK,150, 110, 1);
	display.rectangle(5,125, colors16bit::BLACK,150, 110, 1);
	display.rectangle(165,125, colors16bit::BLACK,150, 110, 1);
	display.horLine(6, 6, colors16bit::GRAY, 149, 109);
	display.horLine(166, 6, colors16bit::GRAY, 149, 109);
	display.horLine(6, 126, colors16bit::GRAY, 149, 109);
	display.horLine(166, 126, colors16bit::GRAY, 149, 109);
}

void subScreenFon ()
{
	display.fillScreen(colors16bit::SILVER);

	display.verLine(106, 0, colors16bit::BLACK, 240, 2);
	display.verLine(212, 0, colors16bit::BLACK, 240, 2);
	display.horLine(0, 120, colors16bit::BLACK, 320, 2);

	display.rectangle(5,5, colors16bit::BLACK,96, 110, 1);
	display.rectangle(111,5, colors16bit::BLACK,96, 110, 1);
	display.rectangle(218,5, colors16bit::BLACK,96, 110, 1);
	display.rectangle(5,125, colors16bit::BLACK,96, 110, 1);
	display.rectangle(111,125, colors16bit::BLACK,96, 110, 1);
	display.rectangle(218,125, colors16bit::BLACK,96, 110, 1);
	display.horLine(6, 6, colors16bit::GRAY, 95, 109);
	display.horLine(112, 6, colors16bit::GRAY, 95, 109);
	display.horLine(219, 6, colors16bit::GRAY, 95, 109);
	display.horLine(6, 126, colors16bit::GRAY, 95, 109);
	display.horLine(112, 126, colors16bit::GRAY, 95, 109);
	display.horLine(219, 126, colors16bit::GRAY, 95, 109);
	back.draw();
	home.draw();
}

void settingValueFon ()
{
	display.fillScreen(colors16bit::SILVER);
	display.verLine(212, 0, colors16bit::BLACK, 240, 2);
	display.horLine(212, 120, colors16bit::BLACK, 108, 2);

	//home and back
	display.rectangle(218,5, colors16bit::BLACK,96, 110, 1);
	display.rectangle(218,125, colors16bit::BLACK,96, 110, 1);
	display.horLine(219, 6, colors16bit::GRAY, 95, 109);
	display.horLine(219, 126, colors16bit::GRAY, 95, 109);
	back.draw();
	home.draw();

	//+, -, <-, ->
	display.rectangle(2,5, colors16bit::BLACK, 50, 55, 1);
	display.rectangle(54,5, colors16bit::BLACK, 50, 55, 1);
	display.rectangle(106,5, colors16bit::BLACK, 50, 55, 1);
	display.rectangle(158,5, colors16bit::BLACK, 50, 55, 1);
	display.horLine(3, 6, colors16bit::GRAY, 49, 54);
	display.horLine(55, 6, colors16bit::GRAY, 49, 54);
	display.horLine(107, 6, colors16bit::GRAY, 49, 54);
	display.horLine(159, 6, colors16bit::GRAY, 49, 54);
	left.draw();
	right.draw();
	plus.draw();
	minus.draw();
}


void drawMainScreen()
{
	mScreen.iterate();
}

void drawRoomScreen()
{
	rooms.iterate();
}

void drawEqupmentScreen()
{
	equipment.iterate();
}







