#include "spi.h"

PotMemFn Spi::ptr_receive[2] = {};

PotMemF Spi::ptr_transmite[2] = {};
ptr_ex Spi::ptr_exchange[2] =  {};



Spi::Spi(Device::spi spi_, Division d, Cpol cpol, Cpha cpha, Size s, Mode m, Role r)
{
	Device &device = Device::getInstance();
	spiPtr = device.getSpiPtr(spi_);
	device.clockSpi(spi_);
	size_ = (uint8_t)s;
	cpol_ = (uint8_t)cpol;
	cpha_ = (uint8_t)cpha;
	role_ = (uint8_t)r;
	div_ = (uint8_t)d;
	mode_ = (uint8_t)m;

  //Turn on tacting SPI1
  device.clockSpi(spi_);

  //===Settings SPI1===//
  spiPtr->S = 0x00;
  spiPtr->C2 &= ~SPI_C2_MODFEN_MASK;
  spiPtr->BR = SPI_BR_SPR(div_);
  spiPtr->C2 |= mode_ << SPI_C2_MODFEN_SHIFT|(size_<<SPI_C2_SPIMODE_SHIFT);
  spiPtr->C1 &= ~(SPI_C1_MSTR_MASK |SPI_C1_CPHA_MASK|SPI_C1_CPOL_MASK|SPI_C1_SSOE_MASK);
  spiPtr->C1 |= (role_ << SPI_C1_MSTR_SHIFT|cpol_ << SPI_C1_CPOL_SHIFT |cpha_ << SPI_C1_CPHA_SHIFT|mode_ <<  SPI_C1_SSOE_SHIFT);
}

Spi::Spi (Device::spi spi_, Role r )
{
	Device &device = Device::getInstance();
	spiPtr = device.getSpiPtr(spi_);
	role_ = (uint8_t)r;
	//Turn on tacting SPI1
	device.clockSpi(spi_);

	//===Settings SPI1===//
	spiPtr->C1 &= ~(SPI_C1_MSTR_MASK |SPI_C1_CPHA_MASK|SPI_C1_CPOL_MASK);
	spiPtr->C1 |= role_ << SPI_C1_MSTR_SHIFT;
}

void Spi::start ()
{
	spiPtr->C1 |= SPI_C1_SPE_MASK;
}

void Spi::stop ()
{
	spiPtr->C1 &= ~ SPI_C1_SPE_MASK;
}

void Spi::setCpol (Cpol c)
{
	spiPtr->C1 &= ~SPI_C1_CPOL_MASK;
	spiPtr->C1 |= (uint8_t)c << SPI_C1_CPOL_SHIFT;
}

void Spi::setCpha (Cpha c)
{
	spiPtr->C1 &= ~SPI_C1_CPHA_MASK;
	spiPtr->C1 |= (uint8_t)c << SPI_C1_CPHA_SHIFT;
}

void Spi::setDivision (Division d)
{
	spiPtr->BR = SPI_BR_SPR(uint8_t (d));
}

void Spi::setFrameSize (Size s)
{
	spiPtr->C2 &= ~SPI_C2_SPIMODE_MASK;
	spiPtr->C2 |= (uint8_t)s << SPI_C2_SPIMODE_SHIFT;
}

void Spi::setMode (Mode m)
{
	spiPtr->C2 &= ~SPI_C2_MODFEN_MASK;
	spiPtr->C2 |= (uint8_t)m << SPI_C2_MODFEN_SHIFT;
	spiPtr->C1 &= ~SPI_C1_SSOE_MASK;
	spiPtr->C1 |= (uint8_t)m << SPI_C1_SSOE_SHIFT;
}
/*
void Spi::setDma (Dma &d)
{
	driverDma = &d;
	DMA0->DMA[d.getChannel()].DCR |= DMA_DCR_CS_MASK;
	DMA0->DMA[d.getChannel()].DCR |= DMA_DCR_EADREQ_MASK| DMA_DCR_ERQ_MASK;
}

void Spi::enableDma (dma d)
{
	spiPtr->C2 |= (1 << (uint8_t)d);
}

void Spi::disableDma (dma d)
{
	spiPtr->C2 &= ~(1 << (uint8_t)d);
}
*/
void Spi::putDataDh (uint8_t data)
{
	spiPtr->DH = data;
}

void Spi::putDataDl (uint8_t data)
{
	spiPtr->DL = data;
}
uint8_t Spi::getDataDh ()
{
	return spiPtr->DH;
}
uint8_t Spi::getDataDl ()
{
	return spiPtr->DL;
}

bool Spi::flagSptef ()
{
	return spiPtr->S & SPI_S_SPTEF_MASK;
}

bool Spi::flagSprf ()
{
	return spiPtr->S & SPI_S_SPRF_MASK;
}

SPI_Type * Spi::getSpiPtr ()
{
	return spiPtr;
}
