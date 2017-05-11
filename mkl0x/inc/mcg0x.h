#include "device.h"             // Device header

#ifndef MCG0X_H
#define MCG0X_H


class Tact
{
  //variables
public:
  enum class divider {div1, div2, div4, div8, div16, div32, div64, div128};
  enum class mode {fei, fee};
 private:
  using setF = void (Tact::*) ();
  static uint32_t cpuClock;
  static uint32_t busClock;
  static uint32_t mcgirClock;
  static uint32_t mcgpllClock;
  static uint32_t mcgfllClock;
  uint8_t src;
  static setF setMode [3];
  //functions
public:
  Tact ();
  Tact (mode m);

  static uint32_t & getFrqCpu (){return cpuClock;};
  static uint32_t & getFrqBus (){return busClock;};
  static uint32_t & getFrqMcgir (){return mcgirClock;};
  static uint32_t & getFrqMcgpll (){return mcgirClock;};
  static uint32_t & getFrqMcgfll (){return mcgirClock;};

private:

  void initFei ();
  void initFee ();
  void initFbi ();
  void initFbe ();
  void initPee ();

  //===ext_OSC===//

};

 
#endif

