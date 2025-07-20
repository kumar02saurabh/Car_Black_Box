#include <xc.h>
#include "adc.h"

void init_adc(void)
{
    /* Turn ON the ADC module */
	ADON = 1;
    
    /* CHANEEL 4 SELECTED*/
    CHS3=0;
    CHS2=1;
    CHS1=0;
    CHS0=0;
    

	
    
    /* Voltage reference bit as VSS */
	VCFG1 = 0;
	/* Voltage reference bit as VDD */
	VCFG0 = 0;
    
    
    /*AN4 analog port*/
    PCFG3=1;
    PCFG2=0;
    PCFG1=1;
    PCFG0=0;
    

    /* Selecting right justified ADRES Registers order */
	ADFM = 1;
    
    
	/*
	 * Selecting the conversion clock of Fosc / 32 -> 1.6usecs -> 1Tad
	 * Our device frequency is 20 MHz
	 */
	ADCS0 = 0;
	ADCS1 = 1;
	ADCS2 = 0;

	/* Stop the conversion to start with */
	GODONE = 0;

	

	

	/* Just clearing the ADRESH & ADRESL registers, for time pass */
	ADRESH = 0;
	ADRESL = 0;

    /* 
	 * Acqusition time selection bits 
	 * Set for 4 Tad
	 */
	ACQT2 = 0;
	ACQT1 = 1;
	ACQT0 = 0;
	
}

unsigned short read_adc()
{
	unsigned short reg_val;

	/*select the channel*/

	/* Start the conversion */
	GO = 1;
	while (GO);
	reg_val = ((ADRESH & 0x03) << 8)  | ADRESL; 

	return reg_val;
}