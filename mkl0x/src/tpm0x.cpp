#include "Tpm0x.h"

Tpm_MemMapPtr Tpm::Tpm_ptr[2]={Tpm0_BASE_PTR, Tpm1_BASE_PTR};
Tpm::setF Tpm::setMode [6] = {};


Tpm::Tpm(N_Tpm n_, channel ch, division d)
{
	numTpm = n_;
	nCh = ch;

	SIM->SCGC6 |= (SIM_SCGC6_Tpm0_MASK << num_Tpm);
	SIM->SOPT2 |= SIM_SOPT2_TpmSRC(3);
	Tpm_SC_REG(Tpm_ptr [num_Tpm]) &= ~Tpm_SC_PS_MASK;
	Tpm_SC_REG(Tpm_ptr [num_Tpm]) |= Tpm_SC_PS(d);

}

void Tpm::Set_MOD (uint16_t val)
{
	Tpm_MOD_REG(Tpm_ptr [num_Tpm]) = val;
}

void Tpm::Set_CNV (uint16_t val)
{
	Tpm_CnV_REG(Tpm_ptr [num_Tpm],n_ch) = val;
}

void Tpm::start ()
{
	Tpm_SC_REG(Tpm_ptr [num_Tpm]) |= Tpm_SC_CMOD(1);
}

void Tpm::stop ()
{
	Tpm_SC_REG(Tpm_ptr [num_Tpm]) &= ~Tpm_SC_CMOD_MASK;
}

void Tpm::clearFlag ()
{
	Tpm_SC_REG(Tpm_ptr [num_Tpm]) |= Tpm_SC_TOF_MASK;
}

void Tpm::init_output_togle(toggle_mode t_mode)
{
	Tpm_SC_REG(Tpm_ptr [num_Tpm]) &= ~Tpm_SC_CPWMS_MASK;
	Tpm_CnSC_REG(Tpm_ptr [num_Tpm],n_ch) &= ~(Tpm_CnSC_ELSA_MASK|Tpm_CnSC_ELSB_MASK|Tpm_CnSC_MSA_MASK|Tpm_CnSC_MSB_MASK);
	Tpm_CnSC_REG(Tpm_ptr [num_Tpm],n_ch) |= Tpm_CnSC_MSA_MASK|(t_mode << Tpm_CnSC_ELSA_SHIFT);
}

void Tpm::init_output_pulse(pulse_mode p_mode)
{
	Tpm_SC_REG(Tpm_ptr [num_Tpm]) &= ~Tpm_SC_CPWMS_MASK;
	Tpm_CnSC_REG(Tpm_ptr [num_Tpm],n_ch) &= ~(Tpm_CnSC_ELSA_MASK|Tpm_CnSC_ELSB_MASK|Tpm_CnSC_MSA_MASK|Tpm_CnSC_MSB_MASK);
	Tpm_CnSC_REG(Tpm_ptr [num_Tpm],n_ch) |= Tpm_CnSC_MSA_MASK|Tpm_CnSC_MSB_MASK|(p_mode << Tpm_CnSC_ELSA_SHIFT) ;
}

void Tpm::init_edge_pwm(e_pwm_mode e_mode)
{
	Tpm_SC_REG(Tpm_ptr [num_Tpm]) &= ~Tpm_SC_CPWMS_MASK;
	Tpm_CnSC_REG(Tpm_ptr [num_Tpm],n_ch) &= ~(Tpm_CnSC_ELSA_MASK|Tpm_CnSC_ELSB_MASK|Tpm_CnSC_MSA_MASK|Tpm_CnSC_MSB_MASK);
	Tpm_CnSC_REG(Tpm_ptr [num_Tpm],n_ch) |= Tpm_CnSC_MSB_MASK|(e_mode << Tpm_CnSC_ELSA_SHIFT) ;

}

void Tpm::init_center_pwm(e_pwm_mode e_mode)
{
	Tpm_SC_REG(Tpm_ptr [num_Tpm]) |= Tpm_SC_CPWMS_MASK;
	Tpm_CnSC_REG(Tpm_ptr [num_Tpm],n_ch) &= ~(Tpm_CnSC_ELSA_MASK|Tpm_CnSC_ELSB_MASK|Tpm_CnSC_MSA_MASK|Tpm_CnSC_MSB_MASK);
	Tpm_CnSC_REG(Tpm_ptr [num_Tpm],n_ch) |= Tpm_CnSC_MSB_MASK|(e_mode << Tpm_CnSC_ELSA_SHIFT) ;
}


