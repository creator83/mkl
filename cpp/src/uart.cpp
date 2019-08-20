#include "uart.h"


Uart::Uart (Device::uart u, baud b, Pin & tx, Pin & rx)
:rx_(rx), tx_(tx){
	Device &device = Device::getInstance();
	uartPtr = device.getUartPtr(u);
	device.clockUart(u);
	uint16_t sbr;
  
  //===Settings UART===//
	//Disable UART
	uartPtr->C2 &= ~(UART_C2_RE_MASK|UART_C2_TE_MASK);
	uartPtr->C1 = 0;
	
	//calculate baud
	//sbr = (tact::get_frq_bus()*1000000)/(16*b);
	
	uartPtr->BDH = UART_BDH_SBR(sbr >> 8);

	uartPtr->BDL = UART_BDL_SBR(sbr);
	
	uartPtr->C2 |= (UART_C2_RE_MASK|UART_C2_TE_MASK);
}

void Uart::transmit (uint8_t data)
{
	/* Wait until space is available in the FIFO */
  while(!(UART2->S1 & UART_S1_TC_MASK));
   
  /* Send the character */
  uartPtr->D = data;
}

void Uart::transmit (char * str)
{
  while (*str){
    while(!(uartPtr->S1 & UART_S1_TC_MASK));
    uartPtr->D = *str;
    str++;
  }
}
