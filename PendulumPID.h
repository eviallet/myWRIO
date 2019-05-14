#ifndef PENDULUMPID_H_
#define PENDULUMPID_H_

#include "MyRIO.h"
#include "Time.h"

namespace myRIO {

class PendulumPID {
public:
	PendulumPID(double kp, double ki, double kd);

	double compute(double angle);
	void setSetpoint(double setpoint);

	~PendulumPID();
private:
	Time stopwatch;

	double setpoint;
	double errSum;
	double lastErr;

	double kp;
	double ki;
	double kd;
};

} /* namespace myRIO */

#endif /* PENDULUMPID_H_ */
