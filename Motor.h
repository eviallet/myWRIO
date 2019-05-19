#ifndef MOTOR_H_
#define MOTOR_H_

#include "MyRIO.h"
#include "PWM.h"
#include "Encoder.h"
#include "DIO.h"

namespace myRIO {
// The direction of the rotation of the motors (Clockwise, Counter Clockwise )
enum {
	CCW, // LOW
	CW	 // HIGH
};

class Encoder;
class PWM;

/**
* Combination of PWM and Encoder to work with motors
*/
class Motor {
public:
	Motor(uint32_t port, bool defaultDirection, double angularCoef);
	void setSpeed(double speed);
	void setAngularSpeed(double speed);
	void setAngularSpeedAndDirection(double speed);
	double getSpeed();
	void setDirection(bool dir);
	bool getDefaultDirection();
	void setInterrupt(std::function<void(long, bool)> func, unsigned int threshold);
	long getEncoderPulses();
	~Motor();
private:
	double speed; /**< The duty cycle of the motor*/
	double angularCoef; /**< Angular coefficient of the motors
	* The relation between the angular speed and the duty cycle. 
	* For exemple for our motor for the 100% duty cycle we had an angular speed of 884,6 °/s.
	* Which means that our coeffient is angular speed * 100% = 8,846.
	*/
	bool defaultDirection; /**< Default direction of the motors
	*  In our case : ClockWise for the left motor and Conter ClockWise for the right motor.
	*/
	bool direction; /**< Direction of the motors*/ 
	PWM* channel; /**< The pin where the PWM will be generated*/
	Encoder* enc; /**< Attached encodeur
	* In our case : Encoder A attached to the PWMA_0 (PWM0) and Encoder B attached to the PWMA_1 (PWM1).
 	*/
	uint8_t pin; /**< The pin where the PWM will be generated*/
};

}

#endif /* MOTOR_H_ */
