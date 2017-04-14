#include "Tpm0x.h"

TPM_MemMapPtr Tpm::tpmPtr[2]={TPM0_BASE_PTR, TPM1_BASE_PTR};

Tpm::setF Tpm::fMode [6] = {};


Tpm::Tpm (nTpm n_, channel ch, division d)
{
	numTpm = static_cast <uint8_t>(n_);
	nCh = static_cast <uint8_t>(ch);

	SIM->SCGC6 |= (SIM_SCGC6_TPM0_MASK << numTpm);
	SIM->SOPT2 |= SIM_SOPT2_TPMSRC(3);
	TPM_SC_REG(tpmPtr [numTpm]) &= ~TPM_SC_PS_MASK;
	TPM_SC_REG(tpmPtr [numTpm]) |= TPM_SC_PS(d);

}

void Tpm::setModulo (uint16_t val)
{
	TPM_MOD_REG(tpmPtr [numTpm]) = val;
}

void Tpm::setCnt (uint16_t val)
{
	TPM_CnV_REG(tpmPtr [numTpm],nCh) = val;
}

void Tpm::start ()
{
	TPM_SC_REG(tpmPtr [numTpm]) |= TPM_SC_CMOD(1);
}

void Tpm::stop ()
{
	TPM_SC_REG(tpmPtr [numTpm]) &= ~TPM_SC_CMOD_MASK;
}

void Tpm::clearFlag ()
{
	TPM_SC_REG(tpmPtr [numTpm]) |= TPM_SC_TOF_MASK;
}

void Tpm::setMode (mode m, togPulseMode n)
{
	(this->*(Tpm::fMode[static_cast <uint8_t>(m)]))(n);
}

void Tpm::initOutputTogle(togPulseMode t_mode)
{
	TPM_SC_REG(tpmPtr [numTpm]) &= ~TPM_SC_CPWMS_MASK;
	TPM_CnSC_REG(tpmPtr [numTpm],nCh) &= ~(TPM_CnSC_ELSA_MASK|TPM_CnSC_ELSB_MASK|TPM_CnSC_MSA_MASK|TPM_CnSC_MSB_MASK);
	TPM_CnSC_REG(tpmPtr [numTpm],nCh) |= TPM_CnSC_MSA_MASK|(static_cast <uint8_t>(t_mode) << TPM_CnSC_ELSA_SHIFT);
}

void Tpm::initOutputPulse(togPulseMode p_mode)
{
	TPM_SC_REG(tpmPtr [numTpm]) &= ~TPM_SC_CPWMS_MASK;
	TPM_CnSC_REG(tpmPtr [numTpm],nCh) &= ~(TPM_CnSC_ELSA_MASK|TPM_CnSC_ELSB_MASK|TPM_CnSC_MSA_MASK|TPM_CnSC_MSB_MASK);
	TPM_CnSC_REG(tpmPtr [numTpm],nCh) |= TPM_CnSC_MSA_MASK|TPM_CnSC_MSB_MASK|(static_cast <uint8_t>(p_mode) << TPM_CnSC_ELSA_SHIFT );
}

void Tpm::initEdgePwm(togPulseMode e_mode)
{
	TPM_SC_REG(tpmPtr [numTpm]) &= ~TPM_SC_CPWMS_MASK;
	TPM_CnSC_REG(tpmPtr [numTpm],nCh) &= ~(TPM_CnSC_ELSA_MASK|TPM_CnSC_ELSB_MASK|TPM_CnSC_MSA_MASK|TPM_CnSC_MSB_MASK);
	TPM_CnSC_REG(tpmPtr [numTpm],nCh) |= TPM_CnSC_MSB_MASK|(static_cast <uint8_t>(e_mode) << TPM_CnSC_ELSA_SHIFT) ;

}

void Tpm::initCenterPwm(togPulseMode e_mode)
{
	TPM_SC_REG(tpmPtr [numTpm]) |= TPM_SC_CPWMS_MASK;
	TPM_CnSC_REG(tpmPtr [numTpm],nCh) &= ~(TPM_CnSC_ELSA_MASK|TPM_CnSC_ELSB_MASK|TPM_CnSC_MSA_MASK|TPM_CnSC_MSB_MASK);
	TPM_CnSC_REG(tpmPtr [numTpm],nCh) |= TPM_CnSC_MSB_MASK|(static_cast <uint8_t>(e_mode) << TPM_CnSC_ELSA_SHIFT) ;

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
