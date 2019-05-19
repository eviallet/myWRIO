#ifndef PENDULUM_H_
#define PENDULUM_H_

#include "MyRIO.h"
#include "Time.h"

namespace myRIO {

class Pendulum {
public:
	Pendulum(double k1, double k2);

	double compute(double teta, double teta_p);
	void setSetpoint(double setpoint);

	~Pendulum();
private:
	double setpoint;

	double k1;
	double k2;

	const double tetaRef = 90;
};

} /* namespace myRIO */

#endif /* PENDULUM_H_ */
