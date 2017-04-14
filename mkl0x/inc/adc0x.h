#include "device.h"                 // Device header
#include "pin.h"


#ifndef ADC0X_H
#define ADC0X_H

class Adc
{
public:
	enum channel {SE0, SE1, SE2, SE3, SE4, SE5, SE6, SE7, SE8, SE9, SE10, SE11, SE12, SE13};
	enum Port {A, B};
	enum resolution {bit8, bit12, bit10, bit16};
private:
	static const uint8_t pin_def [14][2];
	Pin pin;
	uint8_t n_channel;
	uint8_t res;
	union {

		unsigned error :1;
	}flags;
public:
	Adc(channel ch_, resolution r_);
	bool calibrate ();
	uint16_t convert ();
};

#endif /* LIB_ADC_H_ */
