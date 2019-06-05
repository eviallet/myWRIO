#ifndef PWM_H
#define PWM_H

#include "MyRio.h"
#include "math.h"

namespace myRIO {
	
	/**
	* A0 = pin A D8
	* A1 = pin A D9
	* A2 = pin A D10
	* B0 = pin B D8
	* B1 = pin B D9
	* B2 = pin B D10
	* C0 = pin C D3
	* C1 = pin C D7
	*/
	enum {
		PWMA0 = PWMA_0CNFG, /**< Pin PWMA_0*/
		PWMA1 = PWMA_1CNFG, /**< Pin PWMA_1*/
		PWMA2 = PWMA_2CNFG, /**< Pin PWMA_2*/
		PWMB0 = PWMB_0CNFG, /**< Pin PWMB_0*/
		PWMB1 = PWMB_1CNFG, /**< Pin PWMB_1*/
		PWMB2 = PWMB_2CNFG, /**< Pin PWMB_2*/
		PWMC0 = PWMC_0CNFG, /**< Pin PWMC_0*/
		PWMC1 = PWMC_1CNFG  /**< Pin PWMC_1*/
	};

/**
 * Outputs a PWM signal (3.3V)
 */
class PWM {
public:
	PWM(uint32_t out, double frequency, double duty_cycle, short prescaler = 0b110);
	void setDutyCycle(double dutyCycle);
	void setPrescaler(short prescaler);
	void setFrequency(double frequency);
	~PWM();
private:
	uint32_t out; /**< Pin PWM*/
	uint16_t max; /**< Max value that the PWM counts
	* the counter that generates the PWM signal counts up to the max value to  generate the signal that we want*/
	double dutyCycle; /**< PWM Duty Cycle*/
	double frequency; /**< PWM Frequency */
	uint16_t prescaler; /**< PWM Clock prescaler */
	uint32_t outcs; /**< PWM Clock Select register*/
	uint32_t outmax; /**< PWM Max register*/
	uint32_t outcmp; /**< PWM Compare register*/
};
} /* namespace myRIO */

#endif /* PWM_H */
