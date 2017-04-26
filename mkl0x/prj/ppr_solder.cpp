#include "device.h" // Device header
#include "mcg0x.h"
#include "adc0x.h"
#include "segled.h"
#include "tpm0x.h"
#include "pid.h"
#include "buffer.h"
#include "systimer.h"
#include "senc.h"

Tact frq (Tact::mode::fei);
//pid value

const double p  = 2.0;
const double i  = 0.3;
const double d  = 0.5;
uint16_t currTemp;
uint16_t pidVal;

struct data
{
  uint16_t value;
  uint16_t lowLimit;
  uint16_t highLimit;
} setTemp{260}, beepVal;

struct
{
	uint16_t pid;
}interval{0};

Segled indicator (4);
Pid regulator (p, i, d, setTemp.value);
Buffer buffer (3);
Systimer mainloop (Systimer::mode::ms, 1);
Senc encoder (Gpio::Port::A, 2, Gpio::Port::A, 3);

Pin adcPin (Gpio::Port::A, 2, Gpio::mux::Analog);
Adc thermocouple (Adc::channel::SE1, Adc::resolution::bit12, adcPin);

extern "C"
{
	void SysTick_Handler ();
	void ADC0_IRQHandler ();
}

void ADC0_IRQHandler ()
{
	//currTemp = thermocouple
}

void SysTick_Handler ()
{
	static struct
	{
		uint16_t pid;
	}counter{0};
	counter.pid++;

	//indicate current temperature
	buffer.parsDec16(currTemp);
	indicator.value(buffer.getContent(), buffer.getCount());

	encoder.scan();
	if (counter.pid>interval.pid)
	{
		regulator.compute(currTemp);
	}
}

void initData ();

int main()
{
	initData();
	thermocouple.calibrate();
	Tpm triac1 (Tpm::nTpm::TPM_0, Tpm::channel::ch0, Tpm::division::div8);
	triac1.setMode(Tpm::mode::edgePwm, Tpm::togPulseMode::highPulse);
	buffer.parsDec16 (1284);


	while (1)
	{

	}
}

void initData ()
{
	setTemp.highLimit = 300;
	setTemp.lowLimit = 240;
	beepVal.highLimit = 250;
	beepVal.lowLimit = 0;
	beepVal.value = 60;
}

