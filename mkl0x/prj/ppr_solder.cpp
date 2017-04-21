#include "device.h" // Device header
#include "mcg0x.h"
#include "adc0x.h"
#include "segled.h"
#include "tpm0x.h"
#include "pid.h"
#include "buffer.h"
#include "systimer.h"

Tact frq (Tact::mode::fei);

Segled indicator (4);
Pid regulator (2.34, 2.55, 4.69, 160);
Buffer buffer (4);
Systimer mainloop (Systimer::mode::ms, 1);

Pin adcPin (Gpio::Port::A, 2, Gpio::mux::Analog);
Adc thermocouple (Adc::channel::SE1, Adc::resolution::bit12, adcPin);

extern "C"
{
	//void PIT_IRQHandler();
}



int main()
{
	thermocouple.calibrate();
	Tpm triac1 (Tpm::nTpm::TPM_0, Tpm::channel::ch0, Tpm::division::div8);
	triac1.setMode(Tpm::mode::edgePwm, Tpm::togPulseMode::highPulse);
	buffer.parsDec16 (1284);

	while (1)
	{

	}
}
