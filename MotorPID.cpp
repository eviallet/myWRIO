#include "MotorPID.h"

using namespace myRIO;

MotorPID::MotorPID(double kp, double ki) :
		lastEnc(0), setpoint(0), errSum(0),
		kp(kp), ki(ki)
{
}

void MotorPID::setSetpoint(double setpoint) {
	this->setpoint = setpoint;
}

double MotorPID::compute(long enc) {
	double dt = stopwatch.elapsed_ns()*1e-9;

	double angle = enc * toAngle;
	//double angularSpeed = (enc*1. - lastEnc)*toAngle/dt;
	lastEnc = enc;

	double err = setpoint - angle;

	errSum+=err*dt;
	double I = ki * errSum;

	double output = kp * (err + I);

	if(output < -850) output = -850;
	if(output > 850) output = 850;

	stopwatch.reset();

	return output;
}


MotorPID::~MotorPID() {
}
