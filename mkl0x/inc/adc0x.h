#include "device.h"                 // Device header
#include "pin0x.h"


#ifndef ADC0X_H
#define ADC0X_H

class Adc
{
public:
	enum class channel {SE0, SE1, SE2, SE3, SE4, SE5, SE6, SE7, SE8, SE9, SE10, SE11, SE12, SE13};
	enum class resolution {bit8, bit12, bit10, bit16};
	enum class mode {softwareTrg, hardwareTrg};
	enum class samples {smpls4, smpls8, smpls16, smpls32};
	enum class hwTriger {external, cmp0=1, tpm0=8, tpm1=9, lptmr0=14};
private:
	Pin * pinDriver;
	uint8_t n_channel;
	uint8_t res;
public:
	//constructor software
	Adc(channel ch_, resolution r_, Pin &);

	Adc(mode ,channel ch_, resolution r_, Pin &);
	void interruptEnable ();
	void interruptDisable ();
	void setHwAVG (samples);

	uint16_t getResult ();
	bool calibrate ();
	uint16_t convert ();
};

#endif /* LIB_ADC_H_ */
