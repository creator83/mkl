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
#include "font.h"
#include "list.h"
#include "mpic.h"
#include "spi.h"
#include "xpt2046.h"
#include "systimer.h"
#include "tbutton.h"
#include "pit.h"
#include "tgrid.h"
#include "tree.h"
#include "data.h"
#include "sstring.h"

//Pressure data

Ssd1289::sFont mNumber;
Ssd1289::sFont bNumber;
Data dryPressureValue (23, 50, colors16bit::BLACK, colors16bit::GRAY, 8, 2, &bNumber);
Data lowPressureValue (129, 50, colors16bit::BLACK, colors16bit::GRAY, 22, 2, &bNumber);
Data hiPressureValue (129, 170, colors16bit::BLACK, colors16bit::GRAY, 34, 2, &bNumber);
Data currentPressureValue (23, 170, colors16bit::BLACK, colors16bit::GRAY, 0, 2, &bNumber);

Tact frq;
Spi spi1 (Spi::SPI_N::SPI_1);
Xpt2046 touch (spi1);
Ssd1289 display;
Tgrid fourArea (touch, 2,2);
Tgrid sixArea (touch,3,2);
Tgrid settingsArea (touch,6,4);


Tbutton tMainScreen (fourArea);
Tbutton tEqipmentScreens (sixArea);
Tbutton tRoomsScreens (sixArea);
Tbutton tBathScreens (sixArea);
Tbutton tDiningScreens (sixArea);
Tbutton tLivingScreens (sixArea);

Tbutton tPump (sixArea);
Tbutton tDryPressure (settingsArea);
Tbutton tLowPressure (settingsArea);
Tbutton tHiPressure (settingsArea);

//main screen objects
List mScreen;

//equipment screen objects
List equipment;

//external
List external;

//clock
List clock;

//sub equioment
List fanScreen;
List pumpScreen;
List floorScreen;
List levelScreen;

List hiPressure;
List lowPressure;
List dryPressure;

//romms screen
List rooms;

//
List diningScreen;
List livingScreen;
List bathScreen;

List settingValue;

Tree menu (&mScreen, &tMainScreen);
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

//pump screen
MonoPicture cPressure (34, 230, colors16bit::GRAY, colors16bit::BLACK, smallImages::pressure, 5, 40);
MonoPicture upPressure (120, 230, colors16bit::GRAY, colors16bit::BLACK, smallImages::pressure, 5, 40);
MonoPicture downPressure (120, 110, colors16bit::GRAY, colors16bit::BLACK, smallImages::pressure, 5, 40);
MonoPicture dPressure (14, 110, colors16bit::GRAY, colors16bit::BLACK, smallImages::pressure, 5, 40);
MonoPicture down (160, 110, colors16bit::GRAY, colors16bit::BLACK, smallImages::down, 5, 40);
MonoPicture up (160, 230, colors16bit::GRAY, colors16bit::BLACK, smallImages::up, 5, 40);
MonoPicture drop (54, 110, colors16bit::GRAY, colors16bit::BLACK, smallImages::water, 5, 40);

//bath screen
MonoPicture floorBath (22, 80, colors16bit::GRAY, colors16bit::BLACK, smallImages::floor, 8, 64);
MonoPicture fanBath (128, 80, colors16bit::GRAY, colors16bit::BLACK, smallImages::fan, 8, 64);




struct flags
{
  unsigned touch :1;
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
		menu.treeAction();
		menu.useCurrent();
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
void subEquipmentFon ();
void roomsFon ();

void drawMainScreen();
void drawRoomScreen();
void drawEqupmentScreen();


void drawLowPressureScreen();
void drawHiPressureScreen();


const uint16_t colors [] = {colors16bit::BLACK, colors16bit::RED, colors16bit::BLUE,  colors16bit::GREEN, colors16bit::CYAN, colors16bit::MAGENTA,  colors16bit::YELLOW, colors16bit::WHITE,
 colors16bit::GRAY, colors16bit::SKY, colors16bit::ORANGE,  colors16bit::PINK, colors16bit::BROWN,colors16bit::VIOLET, colors16bit::SILVER,
 colors16bit::GOLD,  colors16bit::BEGH, colors16bit::NAVY, colors16bit::DARK_GREEN, colors16bit::DARK_CYAN, colors16bit::MAROON,  colors16bit::PURPLE,
colors16bit::LIGHT_GREY,  colors16bit::DARK_GREY};

List * Screens [] = {&mScreen, &equipment, &rooms};

void backF ();
void homeF ();
void forward ();
void forward1 ();
void makeTree ();
void initTouchButton ();
void initScreens ();


int main()
{
	Shape::driver = &display;
	spi1.setMode(Spi::Mode::software);
	Pin sck (Gpio::Port::E, 2, Gpio::mux::Alt2);
	Pin mosi (Gpio::Port::E, 1, Gpio::mux::Alt2);
	Pin miso (Gpio::Port::E, 3, Gpio::mux::Alt2);

	mNumber.font = midleNumbers::number;
	mNumber.height = 36;
	mNumber.width = 2;

	bNumber.font = bigNumbers::numbers;
	bNumber.height = 35;
	bNumber.width = 3;

	initScreens ();
	initTouchButton ();
	makeTree ();

	//NVIC_EnableIRQ(PIT_IRQn);
	//mainloop.start();
	//Systimer mainLoop (Systimer::mode::ms, 1000);
	while (1)
	{
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

void subEquipmentFon ()
{
	display.fillScreen(colors16bit::SILVER);
	display.verLine(106, 0, colors16bit::BLACK, 240, 2);
	display.verLine(212, 0, colors16bit::BLACK, 240, 2);
	display.horLine(0, 120, colors16bit::BLACK, 320, 2);

	//home and back
	display.rectangle(218,5, colors16bit::BLACK,96, 110, 1);
	display.rectangle(218,125, colors16bit::BLACK,96, 110, 1);

	display.rectangle(5,5, colors16bit::BLACK,96, 110, 1);
	display.rectangle(111,5, colors16bit::BLACK,96, 110, 1);

	display.horLine(219, 6, colors16bit::GRAY, 95, 109);
	display.horLine(219, 126, colors16bit::GRAY, 95, 109);
	display.horLine(6, 6, colors16bit::GRAY, 95, 109);
	display.horLine(112, 6, colors16bit::GRAY, 95, 109);
	display.horLine(6, 126, colors16bit::GRAY, 95, 109);
	display.horLine(112, 126, colors16bit::GRAY, 95, 109);

	back.draw();
	home.draw();

}

void roomsFon ()
{
	display.fillScreen(colors16bit::SILVER);
	display.verLine(106, 0, colors16bit::BLACK, 120, 2);
	display.verLine(212, 0, colors16bit::BLACK, 240, 2);
	display.horLine(0, 120, colors16bit::BLACK, 320, 2);

	//home and back
	display.rectangle(218,5, colors16bit::BLACK,96, 110, 1);
	display.rectangle(218,125, colors16bit::BLACK,96, 110, 1);

	display.rectangle(5,5, colors16bit::BLACK,96, 110, 1);
	display.rectangle(111,5, colors16bit::BLACK,96, 110, 1);


	display.horLine(6, 6, colors16bit::GRAY, 95, 109);
	display.horLine(112, 6, colors16bit::GRAY, 95, 109);
	display.horLine(6, 126, colors16bit::GRAY, 201, 109);
	display.horLine(219, 126, colors16bit::GRAY, 95, 109);
	display.horLine(219, 6, colors16bit::GRAY, 95, 109);

	back.draw();
	home.draw();
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


void backF ()
{
	getBack (menu);
}

void homeF ()
{
	getRoot (menu);
}
void forward ()
{
	getForward (menu);

}
void forward1 ()
{
	getForward (menu, 2);
}


void makeTree ()
{
	menu.addSon(&rooms, &tRoomsScreens);
	menu.addBrother(&equipment, &tEqipmentScreens);
	menu.addBrother(&external, &tEqipmentScreens);
	menu.addBrother(&clock, &tEqipmentScreens);
	menu.getRoot();
	menu.getForward(0);
	menu.addSon(&diningScreen, &tDiningScreens);
	menu.addBrother(&livingScreen, &tLivingScreens);
	menu.addBrother(&bathScreen, &tBathScreens);

	//eqipment sub screens
	menu.getRoot();
	menu.getForward(1);
	menu.addSon(&fanScreen, &tEqipmentScreens);
	menu.addBrother(&pumpScreen, &tPump);
	menu.addBrother(&floorScreen, &tEqipmentScreens);
	menu.addBrother(&levelScreen, &tEqipmentScreens);

	//pump screens
	menu.getBack();
	menu.getForward(1);
	menu.addSon(&dryPressure, &tDryPressure);
	menu.addBrother(&lowPressure, &tLowPressure);
	menu.addBrother(&hiPressure, &tHiPressure);
	//menu.getBack();
	menu.getRoot();
	menu.useCurrent();
}

void initTouchButton ()
{
		tMainScreen.addButton(0, forward);
		tMainScreen.addButton(1, forward);
		tMainScreen.addButton(2, forward);
		tMainScreen.addButton(3, forward);

		tEqipmentScreens.addButton(0,forward);
		tEqipmentScreens.addButton(1,forward);
		tEqipmentScreens.addButton(2,forward);
		tEqipmentScreens.addButton(3,forward);
		tEqipmentScreens.addButton(4,homeF);
		tEqipmentScreens.addButton(5,backF);

		tRoomsScreens.addButton(0,forward);
		tRoomsScreens.addButton(1,forward);
		tRoomsScreens.addButton(3,forward1);
		tRoomsScreens.addButton(4,homeF);
		tRoomsScreens.addButton(5,backF);

		tBathScreens.addButton(4,homeF);
		tBathScreens.addButton(5,backF);

		tDiningScreens.addButton(4,homeF);
		tDiningScreens.addButton(5,backF);

		tLivingScreens.addButton(4,homeF);
		tLivingScreens.addButton(5,backF);

		tPump.addButton(0,forward);
		tPump.addButton(2,drawLowPressureScreen);
		tPump.addButton(3,drawHiPressureScreen);
		tPump.addButton(4,homeF);
		tPump.addButton(5,backF);

		tDryPressure.addButton(16,homeF);
		tDryPressure.addButton(17,homeF);
		tDryPressure.addButton(20,homeF);
		tDryPressure.addButton(21,homeF);
		tDryPressure.addButton(23,backF);
		tDryPressure.addButton(22,backF);
		tDryPressure.addButton(18,backF);
		tDryPressure.addButton(19,backF);

		tLowPressure.addButton(16,homeF);
		tLowPressure.addButton(17,homeF);
		tLowPressure.addButton(20,homeF);
		tLowPressure.addButton(21,homeF);
		tLowPressure.addButton(23,backF);
		tLowPressure.addButton(22,backF);
		tLowPressure.addButton(18,backF);
		tLowPressure.addButton(19,backF);

		tHiPressure.addButton(16,homeF);
		tHiPressure.addButton(17,homeF);
		tHiPressure.addButton(20,homeF);
		tHiPressure.addButton(21,homeF);
		tHiPressure.addButton(23,backF);
		tHiPressure.addButton(22,backF);
		tHiPressure.addButton(18,backF);
		tHiPressure.addButton(19,backF);
}

void initScreens ()
{
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

		external.setFunction(subEquipmentFon);

		//clock
		clock.setFunction(subEquipmentFon);

		//sub equipment screen
		fanScreen.setFunction(subEquipmentFon);
		pumpScreen.setFunction(subEquipmentFon);
		pumpScreen.addLast(&cPressure);
		pumpScreen.addLast(&upPressure);
		pumpScreen.addLast(&downPressure);
		pumpScreen.addLast(&dPressure);
		pumpScreen.addLast(&up);
		pumpScreen.addLast(&down);
		pumpScreen.addLast(&drop);
		pumpScreen.addLast(&dryPressureValue);
		pumpScreen.addLast(&lowPressureValue);
		pumpScreen.addLast(&hiPressureValue);
		pumpScreen.addLast(&currentPressureValue);
		floorScreen.setFunction(subEquipmentFon);
		levelScreen.setFunction(subEquipmentFon);

		//pump setting screen
		hiPressure.setFunction(settingValueFon);
		lowPressure.setFunction(settingValueFon);
		dryPressure.setFunction(settingValueFon);

		//sub rooms screen
		diningScreen.setFunction(roomsFon);
		livingScreen.setFunction(roomsFon);
		bathScreen.setFunction(roomsFon);
		bathScreen.addLast(&floorBath);
		bathScreen.addLast(&fanBath);
}

void drawLowPressureScreen()
{
	getForward (menu, 1);
}

void drawHiPressureScreen()
{
	getForward (menu, 2);
}


