#ifndef MotorPID_H_
#define MotorPID_H_

#include "MyRIO.h"
#include "Time.h"

#define NB_AVG 10

namespace myRIO {

class MotorPID {
public:
	MotorPID(double kp, double ki);

	double compute(long enc);
	void setSetpoint(double setpoint);

	~MotorPID();
private:
	Time stopwatch;

	const double toAngle = 360./(12.*52.734);

	long lastEnc;

	double setpoint;
	double errSum;

	double kp;
	double ki;
};

}

#endif /* MotorPID_H_ */
