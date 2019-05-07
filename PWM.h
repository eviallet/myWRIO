#ifndef PWM_H
#define PWM_H

#include "MyRio.h"
#include "math.h"

namespace myRIO {
	enum {
		PWM0 = PWMA_0CNFG,
		PWM1 = PWMA_1CNFG,
		PWM2 = PWMA_2CNFG
	};

class PWM {
public:
	PWM(uint32_t out, long frequency, double duty_cycle);
	void setDutyCycle(double dutyCycle);
	~PWM();
private:
	uint32_t out;
	uint16_t max;
	double dutyCycle;
	uint32_t outcs;
	uint32_t outmax;
	uint32_t outcmp;
};
} /* namespace myRIO */

#endif /* PWM_H */
