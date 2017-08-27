#include "mcg0x.h"

uint32_t Tact::cpuClock;
uint32_t Tact::busClock;
uint32_t Tact::mcgirClock;
uint32_t Tact::mcgpllClock;
uint32_t Tact::mcgfllClock;

Tact::setF Tact::setMode [3] = {&Tact::initFei, &Tact::initFee};

Tact::Tact ()
{
	initFei();
}

Tact::Tact (mode m)
{
	(this->*(Tact::setMode[static_cast <uint8_t>(m)]))();
}


void Tact::initFee ()
{
	/* SIM->SCGC5: PORTA=1 */
	SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK;   /* Enable clock gate for ports to enable pin routing */
    /* SIM->CLKDIV1: OUTDIV1=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,OUTDIV4=1,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0 */
	SIM->CLKDIV1 = (SIM_CLKDIV1_OUTDIV1(0x00) | SIM_CLKDIV1_OUTDIV4(0x01)); /* Update system prescalers */
	/* PORTA->PCR[3]: ISF=0,MUX=0 */
	PORTA->PCR[3] &= (uint32_t)~(uint32_t)((PORT_PCR_ISF_MASK | PORT_PCR_MUX(0x07)));
	/* PORTA->PCR[4]: ISF=0,MUX=0 */
	PORTA->PCR[4] &= (uint32_t)~(uint32_t)((PORT_PCR_ISF_MASK | PORT_PCR_MUX(0x07)));
	/* Switch to FEE Mode */
	/* MCG->C2: LOCRE0=0,??=0,RANGE0=0,HGO0=0,EREFS0=1,LP=0,IRCS=0 */
	MCG->C2 = (MCG_C2_RANGE0(0x00) | MCG_C2_EREFS0_MASK);
	/* OSC0->CR: ERCLKEN=1,??=0,EREFSTEN=0,??=0,SC2P=0,SC4P=0,SC8P=0,SC16P=0 */
	OSC0->CR = OSC_CR_ERCLKEN_MASK;
	/* MCG->C1: CLKS=0,FRDIV=0,IREFS=0,IRCLKEN=1,IREFSTEN=0 */
	MCG->C1 = (MCG_C1_CLKS(0x00) | MCG_C1_FRDIV(0x00) | MCG_C1_IRCLKEN_MASK);
	/* MCG->C4: DMX32=1,DRST_DRS=1 */
	MCG->C4 = (uint8_t)((MCG->C4 & (uint8_t)~(uint8_t)(
		            MCG_C4_DRST_DRS(0x02)
		           )) | (uint8_t)(
		            MCG_C4_DMX32_MASK |
		            MCG_C4_DRST_DRS(0x01)
		           ));
	while((MCG->S & MCG_S_IREFST_MASK) != 0x00U) { /* Check that the source of the FLL reference clock is the external reference clock. */
	}
	while((MCG->S & 0x0CU) != 0x00U) {    /* Wait until output of the FLL is selected */
	}
	cpuClock = 47972;
	busClock = 23980;
	mcgirClock = 32;
}

void Tact::initFei ()
{
	//=== Update system prescalers ===//
	  SIM->CLKDIV1 = 
		(SIM_CLKDIV1_OUTDIV1(0x00) //core clock division
		| SIM_CLKDIV1_OUTDIV4(0x01)) //bus clock/flash clock
		; 
		
	  //=== Switch to FEI Mode ===//
	  MCG->C1 = 
		MCG_C1_CLKS(0x00) // FLL is selected
		|MCG_C1_FRDIV(0x00) //divider 1
		|MCG_C1_IREFS_MASK //the slow internal reference clock selected
		|MCG_C1_IRCLKEN_MASK //MCGIRCLK active
		;

	  MCG->C2 = 0;/*
		MCG_C2_RANGE0(0x00) //Low frequency range selected for the crystal oscillator
		|MCG_C2_EREFS0_MASK // Oscillator requested
		;*/
	  /* MCG_C4: DMX32=0,DRST_DRS=1 */
	  MCG->C4 = (uint8_t)((MCG->C4 & (uint8_t)~(uint8_t)(
	            MCG_C4_DMX32_MASK 
							|MCG_C4_DRST_DRS(0x02)))
							| (uint8_t)(MCG_C4_DRST_DRS(0x01)))
							;
		OSC0->CR = 0; //External reference clock is inactive
	  while((MCG->S & MCG_S_IREFST_MASK) == 0x00U) { /* Check that the source of the FLL reference clock is the internal reference clock. */
	  }
	  while((MCG->S & MCG_S_CLKST_MASK) != 0x00U) {    /* Wait until output of the FLL is selected */
	  }
	  cpuClock = 41943;
	  busClock = 20970;
		if ((MCG->S & MCG_S_IRCST_MASK)==0)
			mcgirClock = 32;
		else
			mcgirClock = 4000;
}
