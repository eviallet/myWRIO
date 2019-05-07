#ifndef MOTOR_H_
#define MOTOR_H_

#include "MyRIO.h"
#include "PWM.h"
#include "Encoder.h"
#include "DIO.h"

namespace myRIO {

enum {
	CCW, // LOW
	CW	 // HIGH
};

class Encoder;
class PWM;

class Motor {
public:
	Motor(uint32_t port, double angularCoef);
	void setSpeed(double speed);
	void setAngularSpeed(double speed);
	double getSpeed();
	void setDirection(bool dir);
	void setInterrupt(std::function<void(long, bool)> func, unsigned int threshold);
	~Motor();
private:
	double speed; // 0->100
	double angularCoef;
	bool direction;
	PWM* channel;
	Encoder* enc;
	uint8_t pin;
};

}

#endif /* MOTOR_H_ */
