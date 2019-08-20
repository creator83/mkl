#include "MKL26Z4.h"                // Device header



#ifndef DEVICE_H
#define DEVICE_H
class Device;
class DeviceDestroyer
{
private:
    Device* p_instance;
public:    
    ~DeviceDestroyer();
    void initialize( Device* p );
};
class Device {
public:
	//block Port
	enum class periph{portA, portB, portC, portD, portE};
	enum class port {A , B , C , D , E};
	
	//block Uart
	enum class uart{uart1, uart2};

	//block Spi
	enum class spi{spi0, spi1};
	
	static Device& getInstance();
private:
	using function = void (Device::*)();

	static Device * p_instance;
	static DeviceDestroyer destroyer;
	static PORT_Type * portPtr[5];
	static GPIO_Type * gpioPtr[5];
	static UART_Type * uartPtr[2];
	static SPI_Type * spiPtr[2];

	static uint32_t clockPortMask[5];
	static uint32_t * clockPortRegister;
	static function clockPeriphArr[5];
	static function clockPortArr[5];
	static function clockUartArr[2];
	static function clockSpiArr[2];


	Device ();
    Device(Device const&) = delete;
    Device& operator=(Device const&) = delete;
	friend class DeviceDestroyer;
	void clockPortA();
	void clockPortB();
	void clockPortC();
	void clockPortD();
	void clockPortE();
	void clockUart1();
	void clockUart2();
	void clockSpi0();
	void clockSpi1();
public:
	PORT_Type * getPortPtr (port);
	GPIO_Type * getGpioPtr (port);
	UART_Type * getUartPtr (uart);
	SPI_Type * getSpiPtr (spi);
	void clockPeriph (periph);
	void clockPort (port);
	void clockUart (uart);
	void clockSpi (spi);
};
//===GPIO DEFENITION===//
//enum class PORT {A , B , C , D , E};
//extern const uint32_t CLOCK_PORT_MASK[5] = {SIM_SCGC5_PORTA_MASK, SIM_SCGC5_PORTB_MASK, SIM_SCGC5_PORTC_MASK, SIM_SCGC5_PORTD_MASK, SIM_SCGC5_PORTE_MASK};
//extern const PORT_Type * PORT_PTR[5] = {PORTA, PORTB, PORTC, PORTD, PORTE};
//extern const GPIO_Type * GPIO_PTR[5] = {PTA, PTB, PTC, PTD, PTE};
//const IRQn pinInt [5] = {PORTA_IRQn, PORTB_IRQn, PORTC_IRQn, PORTD_IRQn, PORTE_IRQn};
////===SPI DEFENITION===//
//enum class numberSpi {SPI_0=0};
//const uint32_t spiAddress [1]={SPI0_BASE};
//const uint32_t spiClockShift [1]={12};

////===DMA DEFENITION===//
//enum class dmaMux {uart0Rx = 2, uart0Tx, uart1Rx, uart1Tx, spi0Rx = 14, 
//	spi0Tx, i2c=18, ftm0ch0=20, ftm0ch1, ftm0ch2, ftm0ch3, ftm0ch4, ftm0ch5,
//	ftm1ch0 = 28, ftm1ch1, ftm2ch0, ftm2ch1, adc = 40, cmp0 = 42, cmp1 ,dac = 45, 
//	pdb = 48, pta = 49, ptb, ptc, ptd, pte,	dma0 = 60, dma1, dma2, dma3};

//enum class dmaChannel {ch0, ch1, ch2 , ch3};
//const IRQn dmaInt [4] = {DMA0_IRQn, DMA1_IRQn, DMA2_IRQn, DMA3_IRQn};

////===PIT DEFENITION===//
//enum class pitChannel {ch0, ch1, ch2, ch3};
//const IRQn pitInt [4] = {PIT0_IRQn, PIT1_IRQn, PIT2_IRQn, PIT3_IRQn};
#endif