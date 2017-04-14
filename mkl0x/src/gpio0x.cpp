#include "gpio0x.h"

GPIO_MemMapPtr Gpio::GpioBase [2] = {GPIOA_BASE_PTR , GPIOB_BASE_PTR};
PORT_MemMapPtr Gpio::PortBase [2] = {PORTA_BASE_PTR, PORTB_BASE_PTR};


Gpio::Gpio ()
{

}

Gpio::Gpio (Port p)
{
  prt = static_cast <uint8_t> (p);
  //takt port
  SIM->SCGC5 |= (0x200 << prt);
}





