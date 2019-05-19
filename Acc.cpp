#include "Acc.h"

using namespace myRIO;
extern NiFpga_Session myrio_session; // global variable defined in CAPI/MyRio.c

/**
 * Constructs an Accelerometer.
 * Set the scale to the default (1/256)
 */
Acc::Acc() :
	scale(1.0/ACCSCALEWGHT_CST) {
}

/**
 * Get the acceleration for an axis.
 * @param type One of ACCXVAL, ACCYVAL or ACCZVAL
 * @param v The return value, passed by reference
 */
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

/**
 * Read the accelerometer value on the x axis
 * @param v The result is given by reference for speed
 */
void Acc::x(double &v) {
	readAcc(ACCXVAL, v);
}

/**
 * Read the accelerometer value on the y axis
 * @param v The result is given by reference for speed
 */
void Acc::y(double &v) {
	readAcc(ACCYVAL, v);
}

/**
 * Read the accelerometer value on the z axis
 * @param v The result is given by reference for speed
 */
void Acc::z(double &v) {
	readAcc(ACCZVAL, v);
}

/**
 * Read the accelerometer value on each axis
 * @param vx Acceleration on x axis
 * @param vy Acceleration on y axis
 * @param vz Acceleration on z axis
 */
void Acc::all(double &vx, double &vy, double &vz) {
	x(vx);
	y(vy);
	z(vz);
}


Acc::~Acc() {
}

