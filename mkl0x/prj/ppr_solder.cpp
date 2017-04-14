#include "device.h"                   // Device header
#include "mcg0x.h"
#include "adc0x.h"
#include "segled.h"
#include "tpm0x.h"
#include "pid.h"

Tact frq (Tact::mode::fei);

Segled indicator (4);
Pid regulator (2.34, 2.55, 4.69, 160);

extern "C"
{
	//void PIT_IRQHandler();
}


int main()
{
	Tpm triac1 (Tpm::nTpm::TPM_0, Tpm::channel::ch0, Tpm::division::div8);
	triac1.setMode(Tpm::mode::edgePwm, Tpm::togPulseMode::highPulse);

	while (1)
	{

	}
}
