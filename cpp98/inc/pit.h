#include "MKL26Z4.h"
#include "tact.h"



#ifndef PIT_H
#define PIT_H

class Pit;
typedef void (Pit::*ptr)(uint16_t);


class Pit
{
  //variables
public:
	enum class channel: uint8_t {ch0, ch1};
	enum class mode: uint8_t {khz, hz, ms, us};
private:
	const uint8_t irq_n = 22;
	uint8_t n_ch;
	static ptr set_func [4];
public:
	Pit(channel ch, uint16_t n, mode m = mode::ms);
	void clear_flag ();
	bool flag_TIF ();
	void start ();
	void stop ();
	void interrupt_enable ();
	void interrupt_disable ();
private:
	void khz_set (uint16_t val);
	void hz_set (uint16_t val);
	void ms_set (uint16_t val);
	void us_set (uint16_t val);


};

#endif /* PIT_H */
