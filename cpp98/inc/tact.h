#include "MKL26Z4.h"             // Device header

#ifndef TACT_H
#define TACT_H


class Tact
{
  //variables
public:
  enum divider {div1, div2, div4, div8, div16, div32, div64, div128};

 private:
  static uint16_t cpu_clock;
  static uint16_t bus_clock;
  static uint16_t mcgir_clock;
  static uint16_t mcgpll_clock;
  static uint16_t mcgfll_clock;
  uint8_t src;
  //functions
public:
  Tact ();
  static uint16_t & getFrqCpu (){return cpu_clock;};
  static uint16_t & getFrqBus (){return bus_clock;};
  static uint16_t & getFrqMcgir (){return mcgir_clock;};
  static uint16_t & getFrqMcgpll (){return mcgir_clock;};
  static uint16_t & getFrqMcgfll (){return mcgir_clock;};

private:

  void initFei ();
  void initFee ();
  void initFbi ();
  void initFbe ();
  void initPee ();

  //===ext_OSC===//

};

 
#endif

