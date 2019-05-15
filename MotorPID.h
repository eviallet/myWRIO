#ifndef MotorPID_H_
#define MotorPID_H_

#include "MyRIO.h"
#include "Time.h"

#define NB_AVG 40

namespace myRIO {

class MotorPID {
public:
	MotorPID(double kp, double ki);

	double compute(long enc);
	void setSetpoint(double setpoint);

	double getAvgSpeed();

	~MotorPID();
private:
	Time stopwatch;

	const double toAngularSpeed = 360./(12.*52.734);

	double lastSpeeds[NB_AVG];

	long lastEnc;
	double setpoint;
	double errSum;

	double kp, ki;
};

}



#endif /* MotorPID_H_ */
