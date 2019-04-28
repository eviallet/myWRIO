#include "PID.h"

using namespace myRIO;

PID::PID(double kp, double ki) :
		setpoint(0), errSum(0),
		kp(kp), ki(ki)
{
}

int16_t PID::compute(uint32_t input) {
	double dt = stopwatch.elapsed_us()*1e-6;

	double err = setpoint - input;

	errSum+=err*dt;
	double I = ki * errSum;

	int16_t output = kp * (err + I);

	if(output < 0) output = 0;
	if(output > 99) output = 99;

	stopwatch.reset();

	return output;
}

PID::~PID() {

}
