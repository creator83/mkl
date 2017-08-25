#include "tpm.h"

TPM_Type * Tpm::tpmPtr[2]={TPM0, TPM1};


Tpm::Tpm (nTpm n_, channel ch, division d)
{
	numTpm = static_cast <uint8_t>(n_);
	nCh = static_cast <uint8_t>(ch);
	SIM->SOPT2 |= SIM_SOPT2_TPMSRC(3);
	SIM->SCGC6 |= (SIM_SCGC6_TPM0_MASK << numTpm);
	tpmPtr[numTpm]->SC &= ~TPM_SC_PS_MASK;
	tpmPtr[numTpm]->SC |= TPM_SC_PS(d);
}

void Tpm::setModulo (uint16_t val)
{
	tpmPtr[numTpm]->MOD = val;
}

void Tpm::setCnt (uint16_t val)
{
	tpmPtr[numTpm]->CONTROLS[nCh].CnV = val;
}

TPM_Type * Tpm::getPtrTimer()
{
	return tpmPtr[numTpm];
}

void Tpm::start ()
{
	tpmPtr[numTpm]->SC |= TPM_SC_CMOD(1);
}

void Tpm::stop ()
{
	tpmPtr[numTpm]->SC &= ~TPM_SC_CMOD_MASK;
}

void Tpm::clearFlag ()
{
	tpmPtr[numTpm]->SC |= TPM_SC_TOF_MASK;
}


void Tpm::setModulo (uint16_t val)
{
	tpmPtr[numTpm]->MOD = val;
}

void Tpm::setCnt (uint16_t val)
{
	tpmPtr[numTpm]->CNT = val;
}

void Tpm::interruptEnable ()
{
	tpmPtr[numTpm]->SC |= TPM_SC_TOIE_MASK ;
}

void Tpm::interruptDisable ()
{
	tpmPtr[numTpm]->SC &= ~ TPM_SC_TOIE_MASK ;
}

void Tpm::setKhz (uint16_t val)
{

}

void Tpm::setHz (uint16_t val)
{

}

void Tpm::setMs (uint16_t val)
{

}

void Tpm::setUs (uint16_t val)
{

}
