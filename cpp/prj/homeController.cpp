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
#include "rgb.h"
#include "font.h"
#include "list.h"
#include "mpic.h"
#include "cpic.h"
#include "spi.h"
#include "xpt2046.h"
#include "systimer.h"
#include "tbutton.h"
#include "pit.h"
#include "tgrid.h"
#include "tree.h"
#include "data.h"
#include "sstring.h"
#include "ds3231.h"
#include "i2c.h"
#include "buffer.h"
//Pressure data

Tact frq;
Spi spi1 (Spi::SPI_N::SPI_1);
Xpt2046 touch (spi1);
Ssd1289 display;
Tgrid fourArea (touch, 2,2);
Tgrid sixArea (touch,3,2);
Tgrid settingsArea (touch,6,4);
Pin sda (Gpio::Port::E, 18, Gpio::mux::Alt4);
Pin scl (Gpio::Port::E, 19, Gpio::mux::Alt4);

Buffer buffer;

uint16_t dryPressureData = 5;
uint16_t hiPressureData = 32;
uint16_t lowPressureData = 18;
uint16_t currentPressureData = 0;

uint16_t lowLevelData = 150;
uint16_t hiLevelData = 190;
uint16_t currentLevelData = 0;

uint16_t onHummidityData = 650;
uint16_t offHummidityData = 350;
uint16_t currentHummidityData = 350;

uint16_t offTemperatureData = 285;
uint16_t currentTemperatureData = 0;

uint16_t currentTemperatureLiving = 0;
uint16_t currentHummidityLiving = 0;

uint16_t currentTemperatureBath = 0;
uint16_t currentHummidityBath = 0;
Ssd1289::sFont mNumber;
Ssd1289::sFont bNumber;
Ssd1289::sFont hNumber;
Ssd1289::sFont rFont;

const char * days [7][12] = {
	{"Понедельник"},
	{"Вторник"},
	{"Среда"},
	{"Четверг"},
	{"Пятница"},
	{"Суббота"},
	{"Восскресенье"}
};

const char * months [12][3] = {
	{"Янв"},
	{"Фев"},
	{"Мар"},
	{"Апр"},
	{"Май"},
	{"Июн"},
	{"Июл"},
	{"Авг"},
	{"Сен"},
	{"Окт"},
	{"Ноя"},
	{"Дек"}
};

//pump data
Sstring dryPressureValue (23, 50, colors16bit::BLACK, colors16bit::GRAY, 3, &bNumber, 1);
Sstring lowPressureValue (129, 50, colors16bit::BLACK, colors16bit::GRAY, 3, &bNumber, 1);
Sstring hiPressureValue (129, 170, colors16bit::BLACK, colors16bit::GRAY, 3, &bNumber, 1);
Sstring currentPressureValue (23, 170, colors16bit::BLACK, colors16bit::GRAY, 3, &bNumber, 1);

//level data
Sstring hiLevelValue (129, 170, colors16bit::BLACK, colors16bit::GRAY, 3, &bNumber, 1);
Sstring lowLevelValue (129, 50, colors16bit::BLACK, colors16bit::GRAY, 3, &bNumber, 1);
Sstring currentLevelValue (23, 170, colors16bit::BLACK, colors16bit::GRAY, 3, &bNumber, 1);

//fan data
Sstring onHummidityValue (129, 50, colors16bit::BLACK, colors16bit::GRAY, 3, &bNumber, 1);
Sstring offHummidityValue (129, 170, colors16bit::BLACK, colors16bit::GRAY, 3, &bNumber, 1);
Sstring currentHummidityValue (23, 170, colors16bit::BLACK, colors16bit::GRAY, 3, &bNumber, 1);

//floor data
Sstring offTemperatureValue (129, 170, colors16bit::BLACK, colors16bit::GRAY, 3, &bNumber, 1);
Sstring currentTemperatureValue (23, 170, colors16bit::BLACK, colors16bit::GRAY, 3, &bNumber, 1);
/*
//Alarm
Data alarm1TurnOnValue (23, 50, colors16bit::BLACK, colors16bit::GRAY, 6, 2, &bNumber);
Data alarm1TurnOffValue (129, 50, colors16bit::BLACK, colors16bit::GRAY, 9, 2, &bNumber);
Data alarm2TurnOnValue (23, 50, colors16bit::BLACK, colors16bit::GRAY, 18, 2, &bNumber);
Data alarm2TurnOffValue (23, 50, colors16bit::BLACK, colors16bit::GRAY, 21, 2, &bNumber);
*/

//rooms data
Sstring livingRoomTemperature  (16, 190, colors16bit::BLACK, colors16bit::GRAY, 3, &bNumber, 1);
Sstring livingRoomHummidity (148, 190, colors16bit::BLACK, colors16bit::GRAY, 3, &bNumber, 1);
Sstring bathRoomTemperature  (16, 190, colors16bit::BLACK, colors16bit::GRAY, 3, &bNumber, 1);
Sstring bathRoomHummidity (148, 190, colors16bit::BLACK, colors16bit::GRAY, 3, &bNumber, 1);


//String high pressure
Sstring hiPressureString (200, 50,  colors16bit::BLACK, colors16bit::GRAY, "Давление отключения",&rFont, 0);
Sstring lowPressureString (200, 50,  colors16bit::BLACK, colors16bit::GRAY, "Давление включения",&rFont, 0);
Sstring dryPressureString (200, 50,  colors16bit::BLACK, colors16bit::GRAY, "Давление сухого хода",&rFont, 0);

//String rooms
Sstring livingRoomString (200, 50,  colors16bit::BLACK, colors16bit::GRAY, "Жилая комната",&rFont, 0);
Sstring bathRoomString (200, 50,  colors16bit::BLACK, colors16bit::GRAY, "Ванная комната",&rFont, 0);

//String calendar

Sstring hoursString (10, 197,  colors16bit::BLACK, colors16bit::GRAY, "00",&hNumber, 0);
Sstring minutesString (82, 197,  colors16bit::BLACK, colors16bit::GRAY, "00",&hNumber, 0);
Sstring secondsString (154, 197,  colors16bit::BLACK, colors16bit::GRAY, "00",&hNumber, 0);
Sstring daysString (10, 230,  colors16bit::BLACK, colors16bit::GRAY, 12 ,&rFont, 0);
Sstring monthsString (50, 140,  colors16bit::BLACK, colors16bit::GRAY, 3,&rFont, 0);

//fan button
Horline fanOffButton (9, 76, colors16bit::RED, 90, 35);
Sstring fanOffString (10, 110,  colors16bit::BLACK, colors16bit::RED, 4,&rFont, 0);
Horline fanOnButton (9, 76, colors16bit::GREEN, 90, 35);
Sstring fanOnString (10, 110,  colors16bit::BLACK, colors16bit::GREEN, 4,&rFont, 0);
Horline fanAutoButton (9, 76, colors16bit::YELLOW, 90, 35);
Sstring fanAutoString (10, 110,  colors16bit::BLACK, colors16bit::YELLOW, 4,&rFont, 0);



Tbutton tMainScreen (fourArea);
Tbutton tEqipmentScreens (sixArea);
Tbutton tRoomsScreens (sixArea);
Tbutton tCalendarScreens (sixArea);
Tbutton tExternalScreens (sixArea);

Tbutton tBathScreens (sixArea);
Tbutton tDiningScreens (sixArea);
Tbutton tLivingScreens (sixArea);
Tbutton tRgbScreens (sixArea);

Tbutton tPump (sixArea);
Tbutton tDryPressure (settingsArea);
Tbutton tLowPressure (settingsArea);
Tbutton tHiPressure (settingsArea);

Tbutton tLevel (sixArea);
Tbutton tOnLevel (settingsArea);
Tbutton tOffLevel (settingsArea);
Tbutton tAlarm1 (settingsArea);
Tbutton tAlarm2 (settingsArea);

Tbutton tFloor (sixArea);
Tbutton tOnTemperature (settingsArea);

Tbutton tFan (sixArea);
Tbutton tOnHummidity (settingsArea);
Tbutton tOffHummidity (settingsArea);

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

//sub screen pump screen
List hiPressure;
List lowPressure;
List dryPressure;

//sub screen level screen
List hiLevel;
List lowLevel;
List alarm1;
List alarm2;

//sub screen floor screen
List OnTemperature;

//sub screen fan screen
List onHummidity;
List offHummidity;

//romms screen
List rooms;

//
List diningScreen;
List livingScreen;
List bathScreen;

List rgbScreen;

List settingValue;

List fanControlButton;

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
MonoPicture floorBath (26, 70, colors16bit::GRAY, colors16bit::BLACK, smallImages::floor1, 7, 56);
MonoPicture fanBath (132, 70, colors16bit::GRAY, colors16bit::BLACK, smallImages::fan1, 7, 56);

//living screen
MonoPicture lightLiving (26, 70, colors16bit::GRAY, colors16bit::BLACK, smallImages::light, 7, 56);
MonoPicture settingLiving (132, 70, colors16bit::GRAY, colors16bit::BLACK, smallImages::settings, 7, 56);

//time point
MonoPicture dPoint1 (26, 70, colors16bit::GRAY, colors16bit::BLACK, smallImages::dPoint, 1, 45);
MonoPicture dPoint2 (26, 70, colors16bit::GRAY, colors16bit::BLACK, smallImages::dPoint, 1, 45);

//ColorPicture rgbCircle (0, 230, picture::rgb, 210, 219);


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
void rgbFon ();

void drawMainScreen();
void drawRoomScreen();
void drawEqupmentScreen();


void drawLowPressureScreen();
void drawHiPressureScreen();

/*void setClock ();
void readClock ();*/

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
void initData ();
void parsPressureData (Sstring &, uint16_t &);
void parsLevelData (Sstring &, uint16_t &);
void parsTemperatureData (Sstring &, uint16_t &);


int main()
{
	/*I2c i2c0 (I2c::nI2c::I2c0);
	Ds3231 calendar (i2c0);
	setClock ();*/
	Shape::driver = &display;
	spi1.setMode(Spi::Mode::software);
	Pin sck (Gpio::Port::E, 2, Gpio::mux::Alt2);
	Pin mosi (Gpio::Port::E, 1, Gpio::mux::Alt2);
	Pin miso (Gpio::Port::E, 3, Gpio::mux::Alt2);

	mNumber.font = numbers::times36;
	mNumber.height = 24;
	mNumber.width = 16;
	mNumber.shift = 0;

	bNumber.font = numbers::times40;
	bNumber.height = 27;
	bNumber.width = 17;
	bNumber.shift = 0;

	rFont.font = rusFont::times16;
	rFont.height = 14;
	rFont.width = 16;
	rFont.shift = 192;

	hNumber.font = numbers::times48;
	hNumber.height = 32;
	hNumber.width = 21;
	hNumber.shift = 0;

	initScreens ();
	initTouchButton ();
	initData ();

	makeTree ();
	display.string(10, 50,  colors16bit::GRAY, colors16bit::BLACK,"Привет", rFont,0);

	//NVIC_EnableIRQ(PIT_IRQn);
	//mainloop.start();
	//Systimer mainLoop (Systimer::mode::ms, 1000);
	while (1)
	{
		//readClock ();

		delay_ms(100);
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

void rgbFon ()
{
	display.fillScreen(colors16bit::WHITE);
	display.verLine(212, 0, colors16bit::BLACK, 240, 2);
	display.horLine(212, 120, colors16bit::BLACK, 108, 2);

	//home and back
	display.rectangle(218,5, colors16bit::BLACK,96, 110, 1);
	display.rectangle(218,125, colors16bit::BLACK,96, 110, 1);
	display.horLine(219, 6, colors16bit::GRAY, 95, 109);
	display.horLine(219, 126, colors16bit::GRAY, 95, 109);
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

	menu.getBack();
	menu.getForward(1);
	menu.addSon(&rgbScreen, &tRgbScreens);

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

	//level screens
	menu.getBack();
	menu.getForward(3);
	menu.addSon (&hiLevel, &tOffLevel);
	menu.addBrother (&lowLevel, &tOnLevel);
	menu.addBrother (&alarm1, &tAlarm1);
	menu.addBrother (&alarm2, &tAlarm2);

	//floor screens
	menu.getBack();
	menu.getForward(2);
	menu.addSon (&OnTemperature, &tOnTemperature);

	//fan screens
	menu.getBack();
	menu.getForward(0);
	menu.addSon (&onHummidity, &tOnHummidity);
	menu.addBrother (&offHummidity, &tOffHummidity);

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

		tLivingScreens.addButton(2,forward);
		tLivingScreens.addButton(4,homeF);
		tLivingScreens.addButton(5,backF);

		tRgbScreens.addButton(4,homeF);
		tRgbScreens.addButton(5,backF);


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

		tLevel.addButton(4,homeF);
		tLevel.addButton(5,backF);

		tOnLevel.addButton(16,homeF);
		tOnLevel.addButton(17,homeF);
		tOnLevel.addButton(20,homeF);
		tOnLevel.addButton(21,homeF);
		tOnLevel.addButton(23,backF);
		tOnLevel.addButton(22,backF);
		tOnLevel.addButton(18,backF);
		tOnLevel.addButton(19,backF);


		tOffLevel.addButton(16,homeF);
		tOffLevel.addButton(17,homeF);
		tOffLevel.addButton(20,homeF);
		tOffLevel.addButton(21,homeF);
		tOffLevel.addButton(23,backF);
		tOffLevel.addButton(22,backF);
		tOffLevel.addButton(18,backF);
		tOffLevel.addButton(19,backF);

		tAlarm1.addButton(16,homeF);
		tAlarm1.addButton(17,homeF);
		tAlarm1.addButton(20,homeF);
		tAlarm1.addButton(21,homeF);
		tAlarm1.addButton(23,backF);
		tAlarm1.addButton(22,backF);
		tAlarm1.addButton(18,backF);
		tAlarm1.addButton(19,backF);

		tAlarm2.addButton(16,homeF);
		tAlarm2.addButton(17,homeF);
		tAlarm2.addButton(20,homeF);
		tAlarm2.addButton(21,homeF);
		tAlarm2.addButton(23,backF);
		tAlarm2.addButton(22,backF);
		tAlarm2.addButton(18,backF);
		tAlarm2.addButton(19,backF);
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
		clock.addLast(&hoursString);
		clock.addLast(&minutesString);
		clock.addLast(&secondsString);
		clock.addLast(&daysString);
		clock.addLast(&monthsString);


		//sub equipment screen
		fanScreen.setFunction(subEquipmentFon);
		fanScreen.addLast(&onHummidityValue);
		fanScreen.addLast(&offHummidityValue);
		fanScreen.addLast(&currentHummidityValue);

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
		floorScreen.addLast(&offTemperatureValue);
		floorScreen.addLast(&currentTemperatureValue);

		levelScreen.setFunction(subEquipmentFon);
		levelScreen.addLast(&hiLevelValue);
		levelScreen.addLast(&lowLevelValue);
		levelScreen.addLast(&currentLevelValue);

		//pump setting screen
		hiPressure.setFunction(settingValueFon);
		hiPressure.addLast(&hiPressureString);

		lowPressure.setFunction(settingValueFon);
		lowPressure.addLast(&lowPressureString);

		dryPressure.setFunction(settingValueFon);
		dryPressure.addLast(&dryPressureString);

		//sub rooms screen
		diningScreen.setFunction(roomsFon);
		livingScreen.setFunction(roomsFon);
		livingScreen.addLast(&lightLiving);
		livingScreen.addLast(&settingLiving);
		livingScreen.addLast(&livingRoomTemperature);
		livingScreen.addLast(&livingRoomHummidity);
		bathScreen.setFunction(roomsFon);
		bathScreen.addLast(&floorBath);
		bathScreen.addLast(&fanBath);
		bathScreen.addLast(&bathRoomTemperature);
		bathScreen.addLast(&bathRoomHummidity);

		rgbScreen.setFunction(rgbFon);
		//rgbScreen.addLast(&rgbCircle);
}

void drawLowPressureScreen()
{
	getForward (menu, 1);
}

void drawHiPressureScreen()
{
	getForward (menu, 2);
}
/*
void setClock ()
{
	calendar.stop();
	calendar.setSeconds(0);
	calendar.setMinutes(10);
	calendar.setHours(19);
	calendar.setDay(2);
	calendar.setDate(24);
	calendar.setMonth(1);
	calendar.setYear(17);
	calendar.setData();
	calendar.start();
}

void readClock ()
{
	uint8_t temp, result;
	calendar.readCalendar();
	temp = calendar.getHours();
	temp &= valueMask::Dhours >> 4;
	hoursString.setElement(0, temp);
	temp = calendar.getHours()&valueMask::hours;
	hoursString.setElement(1, temp);

	temp = calendar.getMinutes();
	temp &= valueMask::Dminutes >> 4;
	minutesString.setElement(0, temp);
	temp = calendar.getMinutes()&valueMask::minutes;
	minutesString.setElement(1, temp);

	temp = calendar.getSeconds();
	temp &= valueMask::Dseconds >> 4;
	secondsString.setElement(0, temp);
	temp = calendar.getSeconds()&valueMask::seconds;
	secondsString.setElement(1, temp);
	temp = calendar.getDay()&valueMask::day;
	daysString.copy(days[temp][0]);
}*/

void initData ()
{
	//===Pressure===//
	//decimal point
	dryPressureValue.setElement(1, 11);
	lowPressureValue.setElement(1, 11);
	hiPressureValue.setElement(1, 11);
	currentPressureValue.setElement(1, 11);
	parsPressureData (dryPressureValue, dryPressureData);
	parsPressureData (lowPressureValue, lowPressureData);
	parsPressureData (hiPressureValue, hiPressureData);
	parsPressureData (currentPressureValue, currentPressureData);

	//===Level===//
	parsLevelData (hiLevelValue, hiLevelData);
	parsLevelData (lowLevelValue, lowLevelData);
	parsLevelData (currentLevelValue, currentLevelData);

	//===Hummidity===//
	onHummidityValue.setElement(2, 11);
	offHummidityValue.setElement(2, 11);
	currentHummidityValue.setElement(2, 11);
	parsTemperatureData (onHummidityValue, onHummidityData);
	parsTemperatureData (offHummidityValue, offHummidityData);
	parsTemperatureData (currentHummidityValue, currentHummidityData);

	//===Floor temperature===//
	offTemperatureValue.setElement(2, 11);
	currentTemperatureValue.setElement(2, 11);
	parsTemperatureData (offTemperatureValue, offTemperatureData);
	parsTemperatureData (currentTemperatureValue, currentTemperatureData);

	//===Room data===//
	livingRoomTemperature.setElement(2, 11);
	livingRoomHummidity.setElement(2, 11);
	bathRoomTemperature.setElement(2, 11);
	bathRoomHummidity.setElement(2, 11);
	parsTemperatureData (livingRoomTemperature, currentTemperatureLiving);
	parsTemperatureData (livingRoomHummidity, currentHummidityLiving);
	parsTemperatureData (bathRoomTemperature,  currentTemperatureBath);
	parsTemperatureData (bathRoomHummidity, currentHummidityBath);
}

void parsPressureData (Sstring & cont, uint16_t & val)
{
	buffer.setFont(Array_dec);
	buffer.parsDec16(val, 2);
	cont.setElement(0, *buffer.getElement(3));
	cont.setElement(2, *buffer.getElement(4));
}

void parsLevelData (Sstring &cont, uint16_t &val)
{
	buffer.setFont(Array_dec);
	buffer.parsDec16(val, 3);
	cont.setElement(0, *buffer.getElement(2));
	cont.setElement(1, *buffer.getElement(3));
	cont.setElement(2, *buffer.getElement(4));
}

void parsTemperatureData (Sstring &cont, uint16_t &val)
{
	buffer.setFont(Array_dec);
	buffer.parsDec16(val, 3);
	cont.setElement(0, *buffer.getElement(2));
	cont.setElement(2, *buffer.getElement(3));
	cont.setElement(3, *buffer.getElement(4));
}
