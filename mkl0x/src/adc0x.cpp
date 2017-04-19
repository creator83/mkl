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

bool Adc::calibrate ()
{
	unsigned short cal_var;


    SIM_SCGC6 |= SIM_SCGC6_ADC0_MASK;  // enable ADC0 clock

	ADC0->SC2 &=  ~ADC_SC2_ADTRG_MASK ; // Enable Software Conversion Trigger for Calibration Process
	ADC0->SC3 &= ( ~ADC_SC3_ADCO_MASK & ~ADC_SC3_AVGS_MASK ); // set single conversion, clear avgs bitfield for next writing

	//For best calibration results
	ADC0->SC3 |= ( ADC_SC3_AVGE_MASK | ADC_SC3_AVGS(3) );  // Turn averaging ON and set at max value ( 32 )

	ADC0->SC3 |= ADC_SC3_CAL_MASK ;      // Start CAL

	//ADC1_SC1A
	while ( (ADC0->SC1[0] & ADC_SC1_COCO_MASK ) != ADC_SC1_COCO_MASK ); // Wait calibration end

	 //COCO=1 calibration complete
	if ((ADC0->SC3& ADC_SC3_CALF_MASK) == ADC_SC3_CALF_MASK )
	{
	   return true;    // Check for Calibration fail error and return
	}
	  // Calculate plus-side calibration
	  cal_var = 0x00;

	  cal_var =  ADC0->CLP0;
	  cal_var += ADC0->CLP1;
	  cal_var += ADC0->CLP2;
	  cal_var += ADC0->CLP3;
	  cal_var += ADC0->CLP4;
	  cal_var += ADC0->CLPS;

	  cal_var = cal_var/2;
	  cal_var |= 0x8000; // Set MSB


	  ADC0->PG = ADC_PG_PG(cal_var);

	  ADC0->SC3 &= ~ADC_SC3_CAL_MASK ; /* Clear CAL bit */

	  return false;
}

uint16_t Adc::convert ()
{
	//Select 4 channel and start conversation
	ADC0->SC1[0] = ADC_SC1_ADCH(n_channel);
	while (!(ADC0->SC1[0]&ADC_SC1_COCO_MASK));
	return ADC0->R[0];
}



