#include "device.h"               // Device header

PORT_Type * Device::portPtr[5]={PORTA, PORTB, PORTC, PORTD, PORTE};
GPIO_Type * Device::gpioPtr[5]={PTA, PTB, PTC, PTD, PTE};
UART_Type * Device::uartPtr[2] = {UART1, UART2};
SPI_Type * Device::spiPtr[2] = {SPI0, SPI1};

uint32_t Device::clockPortMask[5]={SIM_SCGC5_PORTA_MASK, SIM_SCGC5_PORTB_MASK, SIM_SCGC5_PORTC_MASK, SIM_SCGC5_PORTD_MASK, SIM_SCGC5_PORTE_MASK};
uint32_t * Device::clockPortRegister = (uint32_t *)&SIM->SCGC5;

Device::function  Device::clockPeriphArr [5] = {};
Device::function  Device::clockPortArr [5] = {&Device::clockPortA, &Device::clockPortB, &Device::clockPortC, &Device::clockPortD, &Device::clockPortE};
Device::function  Device::clockUartArr [2] = {&Device::clockUart1, &Device::clockUart2};
Device::function  Device::clockSpiArr [2] = {&Device::clockSpi0, &Device::clockSpi1};

Device * Device::p_instance = nullptr;

DeviceDestroyer Device::destroyer;

DeviceDestroyer::~DeviceDestroyer(){
    delete p_instance;
}
void DeviceDestroyer::initialize( Device* p ){
    p_instance = p;
}


Device::Device(){
}
Device& Device::getInstance()
{
	if (p_instance == nullptr)
	{
		p_instance = new Device ();
        destroyer.initialize(p_instance);
	}
	return *p_instance;
}

PORT_Type * Device::getPortPtr (port prt){
	return Device::portPtr[static_cast<uint8_t>(prt)];
}

GPIO_Type * Device::getGpioPtr (port prt){
	return Device::gpioPtr[static_cast<uint8_t>(prt)];
}
UART_Type * Device::getUartPtr (uart u){
	return Device::uartPtr[static_cast<uint8_t>(u)];
}
SPI_Type * Device::getSpiPtr (spi u){
	return Device::spiPtr[static_cast<uint8_t>(u)];
}
void Device::clockPeriph (periph p){
	(this->*(Device::clockPeriphArr[static_cast<uint8_t>(p)]))();
}
void Device::clockPort (port p){
	(this->*(Device::clockPortArr[static_cast<uint8_t>(p)]))();
}
void Device::clockUart (uart p){
	(this->*(Device::clockUartArr[static_cast<uint8_t>(p)]))();
}
void Device::clockSpi (spi p){
	(this->*(Device::clockSpiArr[static_cast<uint8_t>(p)]))();
}
void Device::clockPortA(){
	SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK;
}
void Device::clockPortB(){
}
void Device::clockPortC(){
}
void Device::clockPortD(){
}
void Device::clockPortE(){
}
void Device::clockUart1(){
	SIM->SCGC4 |= SIM_SCGC4_UART0_MASK;
}
void Device::clockUart2(){
	SIM->SCGC4 |= SIM_SCGC4_UART1_MASK;
}
void Device::clockSpi0(){
	SIM->SCGC4 |= SIM_SCGC4_SPI0_MASK;
}
void Device::clockSpi1(){
	SIM->SCGC4 |= SIM_SCGC4_SPI1_MASK;
}
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
