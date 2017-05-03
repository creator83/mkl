#include "device.h"                    // Device header
#include "ili9341.h"
#include "colors16bit.h"
#include "font.h"
#include "mfont.h"
#include "flashspi.h"
#include "tree.h"
#include "list.h"
#include "flexio.h"
#include "pin.h"
#include "pid.h"
#include "data.h"
//#include "coffeebut.h"
#include "shape.h"
#include "cpic.h"
#include "pixel.h"
#include "horline.h"
#include "lcdflash.h"
#include "xpt2046.h"
#include "tbutton.h"
#include "tgrid.h"
#include "adc.h"
#include "ds3231.h"

enum class butAddress {cup=0x0025, light=0x0036, tools=0x0048, clock=0x0074, switchOn=0x0123, switchOff=0x0423,setboil=0x5463, steam=0x6325};

Tact frq;
Spi spiLcd (Spi::SPI_N::SPI_1);
Spi spimem (Spi::SPI_N::SPI_0);
Dma dma1 (Dma::dmaChannel::ch1);
Dma dma2 (Dma::dmaChannel::ch2);
Dma dma0 (Dma::dmaChannel::ch0);

//pins for lcd
Pin cs (Gpio::Port::D, 4, Gpio::mux::Alt2);
Pin sck (Gpio::Port::D, 5, Gpio::mux::Alt2);
Pin mosi (Gpio::Port::D, 6, Gpio::mux::Alt2);
Ili9341 display (spiLcd, Gpio::Port::D, 7, Gpio::Port::E, 0);
Xpt2046 touch (spimem, Gpio::Port::C, 3, Gpio::Port::C, 8);
Pin sdaPin (Gpio::Port::D, 6, Gpio::mux::Alt2);
Pin sclPin (Gpio::Port::D, 6, Gpio::mux::Alt2);
//I2c i2cDriver (I2c::nI2c::I2c0);
//Ds3231 clock (i2cDriver, sdaPin, sclPin);

Pin light (Gpio::Port::C, 3);

//pins for memory
Pin sckF (Gpio::Port::C, 5, Gpio::mux::Alt2);
Pin mosiF (Gpio::Port::C, 6, Gpio::mux::Alt2);
Pin misoF (Gpio::Port::C, 7, Gpio::mux::Alt2);

Flash memory (spimem, Gpio::Port::C, 4);

Lcdflash dis2 (display, memory);

//===Graphic part===//
//main screen
List mScreen;
Tgrid nineArea (touch,3,3);
Tbutton tMainScreen (nineArea);
Tree menu (&mScreen, &tMainScreen);
/*Data hours (20, 100, );
Data minutes (20, 100, );
Data seconds (20, 100, );*/
//buttons



ColorPicture mScreenCup (3,160, static_cast <uint32_t>(butAddress::cup), 105, 80);
ColorPicture mScreenTool (108,160, static_cast <uint32_t>(butAddress::tools), 105, 80);
ColorPicture mScreenLight (213,160, static_cast <uint32_t>(butAddress::light), 105, 80);

//tools screen
List toolScreen;


//boil screen

List boilScreen;
Tbutton tboilScreen (nineArea);
ColorPicture boilScreenOn (3,160, static_cast <uint32_t>(butAddress::switchOn), 105, 80);
ColorPicture boilScreenSet (108,160, static_cast <uint32_t>(butAddress::setboil), 105, 80);
ColorPicture boilScreenSteam (213,160, static_cast <uint32_t>(butAddress::steam), 105, 80);

/*
 * Data currTemp
 * Data setTemp
 */

//clock screen
List clockScreen;


//pid screen
List pidScreen;
/*Data p
Data i
Data d*/

Pin csFxIo (Gpio::Port::E, 16, Gpio::mux::Alt6);
Pin sckFxIo (Gpio::Port::E, 17, Gpio::mux::Alt6);
Pin mosiFxIo (Gpio::Port::E, 18, Gpio::mux::Alt6);
Pin misoFxIo (Gpio::Port::E, 19, Gpio::mux::Alt6);

Pid regulator (2, 3, 4, 95);

uint16_t buffer [8400];


//Shape * mainScreen[] = {&h1, &point1, &buttonLight};

void fon ();
void makeTree ();
void initScreens ();
void setClock (Ds3231 &);

int main ()
{
	//setClock (clock);
	Flexio touchSpi (Flexio::interface::spi, Flexio::nBuffer::buffer0);
	touchSpi.transmite(0xfe);
	light.set();
	display.setDma(dma0);
	memory.setDma( dma2, dma1);
	initScreens ();
	makeTree ();
	/*memory.writeEnable ();
	memory.readStatus ();*/
	//memory.read (dest, 0, 10);
	//memory.eraseChip ();
	/*uint32_t add=0;
		for (uint16_t i=0;i<600;++i, add+=128)
		{
			memory.writePage16 (&picture::monkeys[add], add, 128);
		}
	uint32_t add=0;
	for (uint16_t i, j=0;i<66;++i, add+=128, j+=128)
		{
			memory.writePage16 (&imgButtons::tools[j], add, 128);
		}*/
	//memory.read (monk1, 0, 40);
	//memory.read16 (monk1, 0, 12800);
	//memory.txDum (20);

/*	display.drawPic(3,160, imgButtons::button1, 105, 80);
	display.drawPic(108,160, imgButtons::button1, 105, 80);
	display.drawPic(213,0, imgButtons::button1, 105, 80);
	display.drawPic(213,80, imgButtons::tools, 105, 80);
	display.drawPic(213,160, imgButtons::button1, 105, 80);*/
	/*for (uint32_t i,j=0;i<154000;i+=12800, j+=20)
	{
		memory.txToDma ((uint32_t)buffer, i, 12800);
		display.drawPic(0,j, buffer, 320, 20);
	}*/

	//spiLcd.setFrameSize(Spi::Size::bit8);
	memory.txToDma ((uint32_t)buffer, 0, 16800);
	display.drawPic(0,0, buffer, 105, 80);
	/*uint32_t add=0;
	for (uint16_t i=0;i<600;++i, add+=256)
	{
		memory.writePage (&picture::monkeys[add], add, 256);
	}*/
	//memory.writePage (picture::monkeys, 0, 0xFF);
	//while (memory.flagBusy()) ++i;
	//memory.writeEnable ();

	//memory.read (dest2, 0, 25600);
	/*Font sFontRus (smallTimesNewRomanRus::simbols);
	sFontRus.setHeight(8);
	sFontRus.setWidth(8);
	sFontRus.setShift(32);
	Font rus (rusFont::times16);
	rus.setHeight(14);
	rus.setWidth(16);
	rus.setShift(192);

	display.symbol(50, 100, colors16bit::RED, colors16bit::YELLOW,  'B', sFontRus);
	display.string(50,50, colors16bit::RED, colors16bit::YELLOW, "Hello", sFontRus, 0);
	display.string(50,150, colors16bit::RED, colors16bit::YELLOW, "Русский", rus, -4);
	display.horLine(0,200, &colors16bit::BLUE, 320, 2);
	display.verLine(200,0, &colors16bit::BLUE, 240, 16);
	display.rectangle (20,20, &colors16bit::BLUE, 140, 80, 28);
	display.rectangle (100, 100, &colors16bit::CYAN, 100, 50);
	display.gradientVer(200,0, c, 119, 200);

	display.drawPic(0,0, monk1, 320, 40);*/
	while (1)
	{

	}
}

void makeTree ()
{
	menu.addSon(&boilScreen, &tboilScreen);
}

void initScreens ()
{
	//===main screen===//
	mScreen.addLast(&mScreenCup);
	mScreen.addLast(&mScreenTool);
	mScreen.addLast(&mScreenLight);
	mScreen.setFunction(fon);

	//===boil screen===//
	boilScreen.addLast(&boilScreenOn);
	boilScreen.addLast(&boilScreenSet);
	boilScreen.addLast(&boilScreenSteam);
	boilScreen.setFunction(fon);

	//===clock screen===//

	//===pid screen===//

}

void fon ()
{
	display.fillScreenDma(&colors16bit::BLACK);
}

void setClock (Ds3231 &d)
{
	d.setDate(28);
	d.setDay(2);
	d.setHours(14);
	d.setMinutes(25);
	d.setMonth(2);
	d.setSeconds(0);
	d.setYear(17);
}
