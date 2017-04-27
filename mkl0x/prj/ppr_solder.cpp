#include "device.h" // Device header
#include "mcg0x.h"
#include "adc0x.h"
#include "segled.h"
#include "tpm0x.h"
#include "pid.h"
#include "buffer.h"
#include "systimer.h"
#include "senc.h"
#include "lptmr.h"
#include "button.h"

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
	uint16_t blink;
}interval{500};

struct
{
	unsigned triacs : 3;
	unsigned blink  : 1;

}flag{0};
Segled indicator (4);
Pid regulator (p, i, d, setTemp.value);
Buffer buffer (3);
Systimer mainloop (Systimer::mode::ms, 1);


Pin adcPin (Gpio::Port::A, 2, Gpio::mux::Analog);
Adc thermocouple (Adc::mode::hardwareTrg, Adc::channel::SE1, Adc::resolution::bit12, adcPin);
Lptmr adcTrigger (Lptmr::division::div8);

//Pwm channels
Pin triac1Pin (Gpio::Port::B, 6, Gpio::mux::Alt2);
Pin triac2Pin (Gpio::Port::B, 7, Gpio::mux::Alt2);
Pin beeperPin (Gpio::Port::B, 10, Gpio::mux::Alt2);
Tpm triac1 (Tpm::nTpm::TPM_1, Tpm::channel::ch0, Tpm::division::div8);
Tpm triac2 (Tpm::nTpm::TPM_1, Tpm::channel::ch1, Tpm::division::div8);
Tpm beeper (Tpm::nTpm::TPM_0, Tpm::channel::ch1, Tpm::division::div8);

Tpm * triacs [2] = {&triac1, &triac2};


//Buttons & Encoder
Senc encoder (Gpio::Port::A, 3, Gpio::Port::A, 4);
Button buttonEnc (Gpio::Port::A, 5);
Button button (Gpio::Port::A, 6);




extern "C"
{
	void SysTick_Handler ();
	void ADC0_IRQHandler ();
}

void ADC0_IRQHandler ()
{
	uint16_t result = thermocouple.getResult();

	currTemp = result/200;
	uint16_t pidResult = regulator.compute(currTemp);
	for (uint8_t i=0;i<2;++i)
	{
		if (flag.triacs&(1 << i))
		{
			triacs[i]->setModulo(pidResult);
		}
	}
}

void SysTick_Handler ()
{
	static struct
	{
		uint16_t blink;
	}counter{0};

	if (!flag.blink)
	{
	//indicate current temperature
	buffer.parsDec16(currTemp);
	indicator.value(buffer.getContent(), buffer.getCount());

	encoder.scan();
	button.scanButton();
	buttonEnc.scanButton();
	buttonEnc.scanAction();
	}
	else
	{
		counter.blink++;
		if (counter.blink>interval.blink)
		{}
	}
}

void buttonEncShort ();
void buttonEncLong ();


void initData ();

int main()
{
	initData();
	thermocouple.calibrate();

	triac1.setMode(Tpm::mode::edgePwm, Tpm::togPulseMode::highPulse);
	triac1.start();
	buffer.parsDec16 (1284);

	thermocouple.setHwTrg(Adc::hwTriger::lptmr0);
	thermocouple.setHwAVG(Adc::samples::smpls32);
	thermocouple.interruptEnable();
	thermocouple.setADC();


	//10ms
	adcTrigger.setComp(30000);
	adcTrigger.start();

	//settings buttons
	button.setShortLimit(10);
	buttonEnc.setShortLimit(10);
	buttonEnc.setshortPressAction(buttonEncShort);
	buttonEnc.setlongPressAction(buttonEncLong);


	button.setLongLimit(1000);
	buttonEnc.setLongLimit(1000);


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

void buttonEncShort ()
{
	flag.triacs++;
	if (flag.triacs>3) {
		flag.triacs = 0;
	}/*
	switch (flag.triacs)
	{
	case 0:
	{
		triac1.setModulo(0);
		triac2.setModulo(0);
	}
	break;
	case 1:
	{
		triac2.setModulo(0);
	}
	break;
	case 2:
	{
		triac1.setModulo(0);
	}
	}*/
	for (uint8_t i=0;i<2;++i)
	{
		if (!(flag.triacs&(1 << i)))
		{
			triacs[i]->setModulo(0);
		}
	}
}

void buttonEncLong ()
{
	flag.blink ^= 1;
}
