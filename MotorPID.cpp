#include "MotorPID.h"

using namespace myRIO;

/** Set the PI coefficients of the motors
* @param kp proportional coefficient
* @param ki integral coefficicent
* This functions sets the coefficients of the PI motor corrector.
* And initialises a vector with 0's.
*/
MotorPID::MotorPID(double kp, double ki) :
		lastEnc(0), setpoint(0), errSum(0),
		kp(kp), ki(ki)
{
	for(int i=0; i<NB_AVG; i++)
		lastSpeeds[i] = 0;
}

/** Set the setpoint
* @param setpoint the setpoint that we want to give to the motor 
*/
void MotorPID::setSetpoint(double setpoint) {
	this->setpoint = setpoint;
}

/** Get the setpoint
* @return returns the setpoint
*/
double MotorPID::getSetpoint() {
	return setpoint;
}

/**
 * Refresh the PID output
 * @param enc The current encoder value
 * @return The output value, in degrees per second
 */
double MotorPID::compute(long enc) {
	double dt = stopwatch.elapsed_ns()*1e-9;

	// shift each speed to the left, discarding the oldest one
	for(int i=0; i<NB_AVG-1; i++)
		lastSpeeds[i] = lastSpeeds[i+1];

	// compute the current speed
	lastSpeeds[NB_AVG-1] = (enc*1.-lastEnc*1.)*toAngularSpeed/dt;
	lastEnc = enc;

	// If there was a delay between the call of the constructor and the refresh, the speed will go crazy because of dt
	if(abs(lastSpeeds[NB_AVG-1])>1e4) lastSpeeds[NB_AVG-1] = lastSpeeds[NB_AVG-2];

	double err = setpoint - getAvgSpeed();

	errSum+=err*dt;
	double I = ki * errSum;

	double output = kp * (err + I);

	// saturate the output so we don't kill the motors
	if(output < -850) output = -850;
	if(output > 850) output = 850;

	// reset the dt timer
	stopwatch.reset();

	return output;
}

/**
 * Calculate the average of the last NB_AVG speeds
 * @return the average
 */
double MotorPID::getAvgSpeed() {
	double avgSpeed = 0;
	for(int i=0; i<NB_AVG; i++)
		avgSpeed += lastSpeeds[i];
	avgSpeed/=(double)NB_AVG;
	return avgSpeed;
}

/** Destructor (Does nothing)
*/
MotorPID::~MotorPID() {
}
