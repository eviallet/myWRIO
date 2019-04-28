#ifndef PID_H_
#define PID_H_

#include "MyRIO.h"
#include "Time.h"

namespace myRIO {

class PID {
public:
	PID(double kp, double ki);

	int16_t compute(uint32_t input);

	~PID();
private:
	Time stopwatch;

	long setpoint;
	long errSum;

	double kp;
	double ki;
};

}

#endif /* PID_H_ */
