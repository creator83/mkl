#include "MKL26Z4.h"                 // Device header
#include "tact.h"

#ifndef I2C_H
#define I2C_H

const uint8_t readBit = 1;
const uint8_t writeBit = 0;

class I2c
{
//variables
public:

enum class nI2c {I2c0,I2c1};

protected:


private:
	static I2C_MemMapPtr i2cAdr [2];
	uint8_t numberI2c;
//functions
public:
  I2c (nI2c);
  void start ();
  void restart ();
  void stop ();
  void putData (uint8_t);
  uint8_t getData ();
  void write (uint8_t);
  uint8_t read (bool);
  bool flagBusy ();
  bool flagTcf ();
  bool flagIicif ();
  bool flagRxak ();
  void setAddress (uint8_t);
  void wByte (uint8_t address, uint8_t reg, uint8_t data);

private:
};



#endif

