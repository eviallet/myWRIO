#include "PendulumPID.h"

using namespace myRIO;

PendulumPID::PendulumPID(double kp, double ki, double kd) :
		setpoint(0), errSum(0), lastErr(0),
		kp(kp), ki(ki), kd(kd) {

}

void PendulumPID::setSetpoint(double setpoint) {
	this->setpoint = setpoint;
}

double PendulumPID::compute(double angle) {
	double dt = stopwatch.elapsed_ns()*1e-9;

	double err = setpoint - angle;

	errSum+=err*dt;
	double I = ki * errSum;

	double D = kd * (err - lastErr) / dt;
	lastErr = err;

	double output = kp * (err + I + D);

	if(output < -850) output = -850;
	if(output > 850) output = 850;

	stopwatch.reset();

	return output;
}

PendulumPID::~PendulumPID() {

}
