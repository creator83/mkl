#include "adc0x.h"

const uint8_t adc::pin_def [14][2] = {
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

adc::adc(channel ch_, resolution r_)
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

uint16_t adc::convert ()
{
	//Select 4 channel and start conversation
	ADC0->SC1[0] = ADC_SC1_ADCH(n_channel);
	while (!(ADC0->SC1[0]&ADC_SC1_COCO_MASK));
	return ADC0->R[0];
}



