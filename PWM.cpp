#include "PWM.h"

using namespace myRIO;
extern NiFpga_Session myrio_session;

PWM::PWM(uint32_t out, long frequency, uint8_t duty_cycle) : out(out), dutyCycle(duty_cycle) {
	uint8_t select;
	status = NiFpga_ReadU8(myrio_session, SYSSELECTA, &select);
	select |= (1<<2) | (1<<3);
	NiFpga_MergeStatus(&status,
			NiFpga_WriteU8(myrio_session, SYSSELECTA, select)); // clear main
	
	max = (long)40e6/(32*frequency)-1;
	
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
	} else {
		status = -1;
		return;
	}

	NiFpga_MergeStatus(&status, 
			NiFpga_WriteU8(myrio_session, out, 0b100)); //PWM generation + not inverted
	NiFpga_MergeStatus(&status, 
			NiFpga_WriteU8(myrio_session, outcs, 0b110)); // clock divider : 32
	NiFpga_MergeStatus(&status, 
			NiFpga_WriteU16(myrio_session, outmax, max));
}

void PWM::setDutyCycle(uint16_t dutyCycle) {
	uint16_t cmp = round(max*dutyCycle/100);
	status = NiFpga_WriteU16(myrio_session, outcmp, cmp);
}

PWM::~PWM() {
	status = NiFpga_WriteU8(myrio_session, out, 0x00);
}
