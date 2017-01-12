#include "i2c.h"

I2C_MemMapPtr I2c::i2cAdr [2] = {I2C0_BASE_PTR, I2C1_BASE_PTR};

I2c::I2c (nI2c n)
{
	numberI2c = static_cast <uint8_t> (n);
	i2cAdr [numberI2c]->F = 0x1F;
	i2cAdr [numberI2c]->C1 =  I2C_C1_MST_MASK| I2C_C1_IICEN_MASK;
}

void I2c::start ()
{
	i2cAdr [numberI2c]->C1 |= I2C_C1_TX_MASK;
	i2cAdr [numberI2c]->C1 =  I2C_C1_MST_MASK;
	while (!flagBusy());
}

void I2c::restart ()
{
	i2cAdr [numberI2c]->C1 |= I2C_C1_RSTA_MASK;
	while (!flagBusy());
}

void I2c::stop ()
{
	i2cAdr [numberI2c]->C1 &= ~I2C_C1_MST_MASK;
	while (!flagBusy());
}

void I2c::putData (uint8_t val)
{
	i2cAdr [numberI2c]->D = val;
}

uint8_t I2c::getData ()
{
	return i2cAdr [numberI2c]->D;
}

void I2c::write (uint8_t val)
{
	while (!flagTcf());
	i2cAdr [numberI2c]->C1 |= I2C_C1_TX_MASK;
	putData(val);
	while (!flagIicif());
	i2cAdr [numberI2c]->S |= I2C_S_IICIF_MASK;
}

uint8_t I2c::read (bool ack)
{
	while (!flagTcf());
	i2cAdr [numberI2c]->C1 &= ~(I2C_C1_TX_MASK| I2C_C1_TXAK_MASK);
	i2cAdr [numberI2c]->C1 |= ack << I2C_C1_TXAK_SHIFT;
	uint8_t data = getData();
	while (!flagIicif());
	i2cAdr [numberI2c]->S |= I2C_S_IICIF_MASK;
	return data;
}



bool I2c::flagBusy ()
{
	return i2cAdr [numberI2c]->S & I2C_S_BUSY_MASK ;
}

bool I2c::flagTcf ()
{
	return i2cAdr [numberI2c]->S & I2C_S_TCF_MASK;
}

bool I2c::flagIicif ()
{
	return i2cAdr [numberI2c]->S & I2C_S_TCF_MASK;
}

