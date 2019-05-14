#include "MotorPID.h"

using namespace myRIO;

MotorPID::MotorPID(double kp, double ki) :
		lastEnc(0), setpoint(0), errSum(0),
		kp(kp), ki(ki)
{
	for(int i=0; i<NB_AVG; i++)
		lastSpeeds[i] = 0;
}

void MotorPID::setSetpoint(double setpoint) {
	this->setpoint = setpoint;
}

double MotorPID::compute(long enc) {
	double dt = stopwatch.elapsed_ns()*1e-9;

	for(int i=0; i<NB_AVG-1; i++)
		lastSpeeds[i] = lastSpeeds[i+1];

	lastSpeeds[NB_AVG-1] = (enc*1.-lastEnc*1.)*toAngularSpeed/dt;

	if(abs(lastSpeeds[NB_AVG-1])>1e4) lastSpeeds[NB_AVG-1] = 0;

	lastEnc = enc;

	double err = setpoint - getAvgSpeed();

	errSum+=err*dt;
	double I = ki * errSum;

	double output = kp * (err + I);

	if(output < 0) output = 0;
	if(output > 850) output = 850;

	stopwatch.reset();

	return output;
}

double MotorPID::getAvgSpeed() {
	double avgSpeed = 0;
	for(int i=0; i<NB_AVG; i++)
		avgSpeed += lastSpeeds[i];
	avgSpeed/=(double)NB_AVG;
	return avgSpeed;
}

MotorPID::~MotorPID() {
}
