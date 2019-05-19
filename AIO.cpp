#include "AIO.h"

using namespace myRIO;
extern NiFpga_Session myrio_session; // global variable defined in CAPI/MyRio.c

/**
* Write an analog pin to the desired value.
* Pins Ax and Bx can handle 0 to 5V values.
* Pins Cx can handle +/- 10V.
* @param pin One of AO0, AO1, ... CO2
* @param value The voltage to write on the pin, in volts
*/
void AIO::writePin(int pin, double value) {
	if(pin == AO0 || pin == AO1 || pin == BO0 || pin == BO1) {
		uint16_t val = value*1e9/1220703;
		status = NiFpga_WriteU16(myrio_session, pin, val);
	} else if(pin == CO0 || pin == CO1) {
		int16_t val = value*1e9/4882813;
		status = NiFpga_WriteI16(myrio_session, pin, val);
	}

	// apply the new value
	NiFpga_MergeStatus(&status,
			NiFpga_WriteBool(myrio_session, AOSYSGO, NiFpga_True));
}

/**
* Read an analog pin.
* Pins Ax and Bx can handle 0 to 5V values.
* Pins Cx can handle +/- 10V.
* @param pin One of AO0, AO1, ... CO2
* @return The value read on the pin, in volts
*/
double AIO::readPin(int pin) {
	if(pin == AI0 || pin == AI1 || pin == AI2 || pin == AI3 ||
			pin == BI0 || pin == BI1 || pin == BI2 || pin == BI3) {
		uint16_t val;
		status = NiFpga_ReadU16(myrio_session, pin, &val);
		return 1220703e-9*val;
	} else if(pin == CI0 || pin == CI1) {
		int16_t val;
		status = NiFpga_ReadI16(myrio_session, pin, &val);
		return 4882813e-9*val;		
	} else {
		status = -1;
		return -1;
	}
}

/** \example example_aio.cpp */
