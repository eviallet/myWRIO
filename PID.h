#ifndef PID_H_
#define PID_H_

#include "MyRIO.h"
#include "Time.h"

namespace myRIO {

class PID {
public:
	PID(double kp, double ki);

	double compute(long enc);
	void setSetpoint(double setpoint);

	~PID();
private:
	Time stopwatch;

	const double toAngularSpeed = 360./(12.*52.734);

	long lastEnc;
	double setpoint;
	double errSum;

	double kp;
	double ki;
};

}

#endif /* PID_H_ */
