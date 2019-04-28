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
	PWM(uint32_t out, long frequency, uint8_t duty_cycle);
	void setDutyCycle(uint16_t dutyCycle);
	~PWM();
private:
	uint32_t out;
	uint16_t max;
	uint8_t dutyCycle;
	uint32_t outcs;
	uint32_t outmax;
	uint32_t outcmp;
};
} /* namespace myRIO */

#endif /* PWM_H */
