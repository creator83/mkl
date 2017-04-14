#include "device.h"


#ifndef TPM_H
#define TPM_H


class Tpm
{
  //variables
public:
	enum class channel {ch0, ch1, ch2, ch3, ch4, ch5};
	enum class nTpm {TPM_0, TPM_1};
	enum class modeClock {TPM_disable, TPM_inc, TPM_ext};
	enum class mode {input, outToggle, outPulse, edgePwm, centerPwm, softwareCompare};
	enum class division {div1, div2, div4, div8, div16, div32, div64, div128};
	enum class togPulseMode {toggle=1, clear=2, set=3, outputHigh=1, outputLow=2, lowPulse=1, highPulse=2};

	//enum mode {khz, hz, ms, us};
private:
	using setF = void (Tpm::*) (togPulseMode);
	uint8_t nCh;
	static TPM_MemMapPtr tpm_ptr[2];
	uint8_t numTpm;
	static setF setMode [6];
public:
	tpm(nTpm n_, channel ch, division d);
	void start ();
	void stop ();
	void clearFlag ();
	void setMode (mode);
	void setModulo (uint16_t val);
	void setCnt (uint16_t val);
private:
	void initOutputTogle(toggle_mode t_mode);
	void init_output_pulse(pulse_mode p_mode);
	void init_edge_pwm(e_pwm_mode e_mode);
	void init_center_pwm(e_pwm_mode e_mode);
	void khz_set (uint16_t val);
	void hz_set (uint16_t val);
	void ms_set (uint16_t val);
	void us_set (uint16_t val);
};

#endif /* PIT_H */
