#include "MKL26Z4.h"                 // Device header


/*
 SPI0
 C: CS-4, SCK-5, MOSI-6,MISO-7
 D: CS-0, SCK-1, MOSI-2,MISO-3
 E: CS-16, SCK-17, MOSI-18,MISO-19


 SPI1
 D:CS-4, SCK-5, MOSI-6,MISO-7
*/

#ifndef SPI_H
#define SPI_H

class Spi;

typedef uint16_t(Spi::*PotMemFn)() ;
typedef uint16_t(Spi::*ptr_ex)(uint16_t) ;

typedef void(Spi::*PotMemF)(uint16_t) ;

class Spi
{
//variables
public:
  enum class Division {div2 , div4 , div8 , div16 , div32 , div64 , div128 , div256, div512};
  enum class SPI_N {SPI_0, SPI_1};
  enum class Role {slave , master};
  enum class Cpol {neg, pos};
  enum class Cpha {first, second};
  enum class Size {bit8, bit16};
  enum class Mode {software, hardware};
  enum class Dma {transmit=5, receive=2};


protected:
  uint8_t size_;
  uint8_t cpol_;
  uint8_t cpha_;
  uint8_t role_;
  uint8_t div_;
  uint8_t n_spi;
  uint8_t mode_;

private:

  static PotMemFn ptr_receive[2];
  static PotMemF ptr_transmite[2];
  static ptr_ex ptr_exchange[2];
  static SPI_MemMapPtr spiAdr [2];

//functions
public:
  Spi(SPI_N, Division d_, Cpol cpol_=Cpol::neg, Cpha cpha_=Cpha::first, Size s=Size::bit8, Mode m= Mode::hardware, Role r=Role::master);

  Spi(SPI_N, Role r=Role::master );
  void start ();
  void stop ();
  void setCpol (Cpol);
  void setCpha (Cpha);
  void setDivision (Division);
  void setFrameSize (Size);
  void setMode (Mode);
  void enableDma (Dma);
  void disableDma (Dma);
  void putDataDh (uint8_t data);
  void putDataDl (uint8_t data);
  uint8_t getDataDh ();
  uint8_t getDataDl ();
  bool flagSptef ();
  bool flagSprf ();

private:
};



#endif

