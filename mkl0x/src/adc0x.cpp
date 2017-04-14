#include "adc0x.h"

const uint8_t Adc::pin_def [14][2] = {
{A, 12},
{B, 5},
{A, 9},
{A, 8},
{B, 2},
{B, 1},
{B, 0},
{A, 7},
{B, 11},
{B, 9},
{B, 8},
{A, 0},
{B, 13}
};

Adc::Adc(channel ch_, resolution r_)
:pin ((pin_def [n_channel][0]), (pin_def [n_channel][1]), Gpio::mux::Analog)
{
	n_channel = ch_;
	//tact ADC0
	SIM->SCGC6 |= SIM_SCGC6_ADC0_MASK;
	//Set busclock
	ADC0->CFG1 &= ~ADC_CFG1_ADICLK_MASK;
	//Set divider - 8
	ADC0->CFG1 |= ADC_CFG1_ADIV(3);
	ADC0->SC3 |= ADC_SC3_AVGE_MASK|ADC_SC3_AVGS(3);
	ADC0->SC2 &= ~ ADC_SC2_ADTRG_MASK;
	ADC0->SC3 |= ADC_SC3_CAL_MASK ;
	while (!(ADC0->SC1[0]&ADC_SC1_COCO_MASK));

	ADC0->CFG1|= ADC_CFG1_ADLSMP_MASK|ADC_CFG1_MODE(r_);
}

void Adc::calibrate ()
{
	unsigned short cal_var;


    SIM_SCGC6 |= SIM_SCGC6_ADC0_MASK;  // enable ADC0 clock

	ADC_SC2_REG(adcmap) &=  ~ADC_SC2_ADTRG_MASK ; // Enable Software Conversion Trigger for Calibration Process
	ADC_SC3_REG(adcmap) &= ( ~ADC_SC3_ADCO_MASK & ~ADC_SC3_AVGS_MASK ); // set single conversion, clear avgs bitfield for next writing

	//For best calibration results
	ADC_SC3_REG(adcmap) |= ( ADC_SC3_AVGE_MASK | ADC_SC3_AVGS(3) );  // Turn averaging ON and set at max value ( 32 )

	ADC_SC3_REG(adcmap) |= ADC_SC3_CAL_MASK ;      // Start CAL

	//ADC1_SC1A
	while ( (ADC_SC1_REG(adcmap,0) & ADC_SC1_COCO_MASK ) != ADC_SC1_COCO_MASK ); // Wait calibration end

	 //COCO=1 calibration complete
	if ((ADC_SC3_REG(adcmap)& ADC_SC3_CALF_MASK) == ADC_SC3_CALF_MASK )
	{
	   return true;    // Check for Calibration fail error and return
	}
	  // Calculate plus-side calibration
	  cal_var = 0x00;

	  cal_var =  ADC_CLP0_REG(adcmap);
	  cal_var += ADC_CLP1_REG(adcmap);
	  cal_var += ADC_CLP2_REG(adcmap);
	  cal_var += ADC_CLP3_REG(adcmap);
	  cal_var += ADC_CLP4_REG(adcmap);
	  cal_var += ADC_CLPS_REG(adcmap);

	  cal_var = cal_var/2;
	  cal_var |= 0x8000; // Set MSB

	  ADC_PG_REG(adcmap) = ADC_PG_PG(cal_var);

	/*
	  // Calculate minus-side calibration
	  cal_var = 0x00;

	  cal_var =  ADC_CLM0_REG(adcmap);
	  cal_var += ADC_CLM1_REG(adcmap);
	  cal_var += ADC_CLM2_REG(adcmap);
	  cal_var += ADC_CLM3_REG(adcmap);
	  cal_var += ADC_CLM4_REG(adcmap);
	  cal_var += ADC_CLMS_REG(adcmap);

	  cal_var = cal_var/2;

	  cal_var |= 0x8000; // Set MSB

	  ADC_MG_REG(adcmap) = ADC_MG_MG(cal_var);
	*/

	  ADC_SC3_REG(adcmap) &= ~ADC_SC3_CAL_MASK ; /* Clear CAL bit */

	  return(0);
}

uint16_t Adc::convert ()
{
	//Select 4 channel and start conversation
	ADC0->SC1[0] = ADC_SC1_ADCH(n_channel);
	while (!(ADC0->SC1[0]&ADC_SC1_COCO_MASK));
	return ADC0->R[0];
}



