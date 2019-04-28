#include "Acc.h"

using namespace myRIO;
extern NiFpga_Session myrio_session; // global variable defined in CAPI/MyRio.c

Acc::Acc() :
	scale(1.0/ACCSCALEWGHT_CST) {
}

void Acc::readAcc(int type, double &v) {
	uint16_t temp;
	status = NiFpga_ReadU16(myrio_session,
			type,
			&temp);
  	  MyRio_ReturnIfNotSuccess(status,
	     "Could not read the accelerometer!");

	// x is returned as an unsigned short, but is a signed short
	v = ((int16_t)temp)*scale;

}

void Acc::x(double &v) {
	readAcc(ACCXVAL, v);
}

void Acc::y(double &v) {
	readAcc(ACCYVAL, v);
}

void Acc::z(double &v) {
	readAcc(ACCZVAL, v);
}

void Acc::all(double &vx, double &vy, double &vz) {
	x(vx);
	y(vy);
	z(vz);
}


void Acc::setScale(double s) {
	scale = 1.0/s;
}



Acc::~Acc() {
}

