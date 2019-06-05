#include "PWM.h"

using namespace myRIO;
extern NiFpga_Session myrio_session;

/** Create PWM signal
*@param out the pin where the PWM signal will be created (PWM0, PWM1)
*@param frequency the frequency of the PWM signal
*@param duty_cycle the duty cycle of the signal; for 50%, duty_cycle = 50
*@param prescaler the clock prescaler. Must be a power of 2 between 1 and 64 in bits : 0b001 = 1, 0b010 = 2, 0b011 = 4...
*/
PWM::PWM(uint32_t out, double frequency, double duty_cycle, short prescaler) : out(out), dutyCycle(duty_cycle), frequency(frequency), prescaler(prescaler) {
	uint8_t select, pin;
	// Activate the PWM signals in the selected pin 
	if(out == PWMA_0CNFG || out == PWMA_1CNFG || out == PWMA_2CNFG) {
		if(out == PWMA_0CNFG) pin = 2;
		if(out == PWMA_1CNFG) pin = 3;
		if(out == PWMA_2CNFG) pin = 4;
		status = NiFpga_ReadU8(myrio_session, SYSSELECTA, &select);
		select |= (1<<pin);
		NiFpga_MergeStatus(&status,
				NiFpga_WriteU8(myrio_session, SYSSELECTA, select));
	}
	else if(out == PWMB_0CNFG || out == PWMB_1CNFG || out == PWMB_2CNFG) {
		if (out == PWMB_0CNFG) pin = 2;
		if (out == PWMB_1CNFG) pin = 3;
		if (out == PWMB_2CNFG) pin = 4;
		status = NiFpga_ReadU8(myrio_session, SYSSELECTB, &select);
		select |= (1<<pin);
		NiFpga_MergeStatus(&status,
				NiFpga_WriteU8(myrio_session, SYSSELECTB, select)); 
	}
	else if(out == PWMC_0CNFG || out == PWMC_1CNFG) {
		if (out == PWMC_0CNFG) pin = 1;
		if (out == PWMC_1CNFG) pin = 3;
		status = NiFpga_ReadU8(myrio_session, SYSSELECTC, &select);
		select |= (1<<pin);
		NiFpga_MergeStatus(&status,
				NiFpga_WriteU8(myrio_session, SYSSELECTC, select)); 
	}
	else {
		status = -1;
		return;
	}

	// Selection of the needed PWM registers (can be configured as needed)
	if(out == PWMA_0CNFG){
		outcs = PWMA_0CS;
		outmax = PWMA_0MAX;
		outcmp = PWMA_0CMP;
	}
	else if(out == PWMA_1CNFG){
		outcs = PWMA_1CS;
		outmax = PWMA_1MAX;
		outcmp = PWMA_1CMP;
	}
	else if(out == PWMA_2CNFG){
		outcs = PWMA_2CS;
		outmax = PWMA_2MAX;
		outcmp = PWMA_2CMP;
	}
	else if(out == PWMB_0CNFG){
		outcs = PWMB_0CS;
		outmax = PWMB_0MAX;
		outcmp = PWMB_0CMP;
	}
	else if(out == PWMB_1CNFG){
		outcs = PWMB_1CS;
		outmax = PWMB_1MAX;
		outcmp = PWMB_1CMP;
	}
	else if(out == PWMB_2CNFG){
		outcs = PWMB_2CS;
		outmax = PWMB_2MAX;
		outcmp = PWMB_2CMP;
	}
	else if(out == PWMC_0CNFG){
		outcs = PWMC_0CS;
		outmax = PWMC_0MAX;
		outcmp = PWMC_0CMP;
	}
	else if(out == PWMC_1CNFG){
		outcs = PWMC_1CS;
		outmax = PWMC_1MAX;
		outcmp = PWMC_1CMP;
	} else {
		status = -1;
		return;
	}

	NiFpga_MergeStatus(&status, 
			NiFpga_WriteU8(myrio_session, out, 0b100)); //PWM generation mode , not inverted (can be configured as needed)
	NiFpga_MergeStatus(&status, 
			NiFpga_WriteU8(myrio_session, outcs, prescaler)); // Clock divider : 32 (can be configured as needed)


	setPrescaler(prescaler); // will also set the frequency and the duty cycle
}

/** Set the Duty Cycle
*@param dutyCycle the duty cycle to set
*/
void PWM::setDutyCycle(double dutyCycle) {
	uint16_t cmp = round((double)max*dutyCycle/100.0);
	status = NiFpga_WriteU16(myrio_session, outcmp, cmp);
}

/** Set the Frequency
* Will try to keep the current duty cycle.
*@param frequency the frequency to set
*/
void PWM::setFrequency(double frequency) {
	this->frequency = frequency;
	max = (long)40e6/(prescaler*frequency)-1; //Maximum value that the PWM counts
	NiFpga_MergeStatus(&status, 
		NiFpga_WriteU16(myrio_session, outmax, max));
	setDutyCycle(dutyCycle);
}

/** Set the clock prescaler
* Will try to keep the old frequency.
*@param clockDiv the prescaler. Must be 1, 2, 4, 8, 16, 32 or 64
*/
void PWM::setPrescaler(short clockDiv) {
	prescaler = clockDiv;
	setFrequency(frequency);
}

/**Destructor
* Desactivate the channel
*/
PWM::~PWM() {
	status = NiFpga_WriteU8(myrio_session, out, 0x00);
}

/**
 * \example example_pwm.cpp
 */
