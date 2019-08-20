#include "device.h"                 // Device header
#include "pin.h"
#include "tact.h"


#ifndef UART_H
#define UART_H

class Uart
{
//variables
public:
  enum baud {baud9600 = 9600 , baud57600 = 57600 , baud115200 = 115200};
private:
	Pin & rx_;
	Pin & tx_; 
	UART_Type * uartPtr;
//functions
public:
	Uart (Device::uart, baud b, Pin & tx, Pin & rx);
  void transmit (uint8_t data);
  void transmit (char * str);  
  uint8_t receive ();
};

#endif
