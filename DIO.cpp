#include "DIO.h"

using namespace myRIO;
extern NiFpga_Session myrio_session; // global variable defined in CAPI/MyRio.c

uint8_t DIO::DOLED30_Status = 0;

/** Write to a led
* @param led the led that you want to write to
* @param state the state of the led; 1 = HIGH , 0 = LOW
*/
void DIO::writeLed(int led, bool state) {
	if(state) DOLED30_Status |= (1<<led);
	else	  DOLED30_Status &=~(1<<led);

	status = NiFpga_WriteU8(myrio_session, DOLED30, DOLED30_Status);
}

/** 
* Parses the necessary registers to read or write a pin.
* @param pin the concerned pin, one of A0, A1... C7
* @param dirReg will contain the direction register for the pin
* @param pinsReg will contain the port in/out register for the pin
* @param io one of IN/OUT
*/
void DIO::parsePin(int pin, uint32_t *dirReg, uint32_t *pinsReg, bool io) {
	if(pin<A_LAST/2) {
		*dirReg = DIOA_70DIR;
		*pinsReg= (io==OUT)?DIOA_70OUT:DIOA_70IN;
	} else if(pin<A_LAST) {
		*dirReg = DIOA_158DIR;
		*pinsReg= (io==OUT)?DIOA_158OUT:DIOA_158IN;
	} else if(pin<B_LAST/2) {
		*dirReg = DIOB_70DIR;
		*pinsReg= (io==OUT)?DIOB_70OUT:DIOB_70IN;
	} else if(pin<B_LAST) {
		*dirReg = DIOB_158DIR;
		*pinsReg= (io==OUT)?DIOB_158OUT:DIOB_158IN;
	} else {
		*dirReg = DIOC_70DIR;
		*pinsReg= (io==OUT)?DIOC_70OUT:DIOC_70IN;
	}
}

/** Write to a pin
* @param pin the pin of the myRio that you want to write to
* @param state the state of the pin; 1 = HIGH , 0 = LOW
*/
void DIO::writePin(int pin, bool state) {
	uint32_t dirReg, pinsReg;
	parsePin(pin, &dirReg, &pinsReg, OUT);
	uint8_t dirVal, pinVal;
	uint8_t relPin = pin%8; // relative pin position in register

	// keep the current values of the pins and the directions
	// since we are working on 8 pins at a time
	status = NiFpga_ReadU8(myrio_session, pinsReg, &pinVal);
	NiFpga_MergeStatus(&status,
			NiFpga_ReadU8(myrio_session, dirReg, &dirVal));

    MyRio_ReturnIfNotSuccess(status,
            "Could not read the DIO!");

	dirVal |= (1<<relPin); // make sure that the pin is an output

	if(state) pinVal |= (1<<relPin);
	else	  pinVal &=~(1<<relPin);

	NiFpga_MergeStatus(&status,
			NiFpga_WriteU8(myrio_session, pinsReg, pinVal));
	NiFpga_MergeStatus(&status,
			NiFpga_WriteU8(myrio_session, dirReg, dirVal));

    MyRio_ReturnIfNotSuccess(status,
            "Could not write to the DIO!");

}

/** Read the state of a pin
* @param pin the pin of the myRio that you want to read from
* @return the state of the pin
*/
int DIO::readPin(int pin) {
	uint32_t dirReg, pinsReg;
	parsePin(pin, &dirReg, &pinsReg, IN);
	uint8_t dirVal, pinVal;
	uint8_t relPin = pin%8; // relative pin position in register

	status = NiFpga_ReadU8(myrio_session, dirReg, &dirVal);

    MyRio_ReturnValueIfNotSuccess(status, -1,
            "Could not read the DIO!");

	dirVal &=~ (1<<relPin); // make sur that the pin is an input

	NiFpga_MergeStatus(&status,
			NiFpga_WriteU8(myrio_session, dirReg, dirVal));
	NiFpga_MergeStatus(&status,
			NiFpga_ReadU8(myrio_session, pinsReg, &pinVal));

    MyRio_ReturnValueIfNotSuccess(status, -1,
            "Could not read the DIO!");

    return (pinVal&(1<<relPin))>>relPin;
}
