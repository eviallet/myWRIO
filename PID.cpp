#include "PID.h"

using namespace myRIO;

PID::PID(double kp, double ki) :
		lastEnc(0), setpoint(0), errSum(0),
		kp(kp), ki(ki)
{
}

void PID::setSetpoint(double setpoint) {
	this->setpoint = setpoint;
}

double PID::compute(long enc) {
	double dt = stopwatch.elapsed_ns()*1e-9;

	double angularSpeed = (enc*1.-lastEnc*1.)*toAngularSpeed/dt;
	lastEnc = enc;

	double err = setpoint - angularSpeed;

	errSum+=err*dt;
	double I = ki * errSum;

	double output = kp * (err + I);

	if(output < 0) output = 0;
	if(output > 850) output = 850;

	stopwatch.reset();

	return output;
}

PID::~PID() {

}
