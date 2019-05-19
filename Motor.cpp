#include "Motor.h"

using namespace myRIO;

/** Motors configuration
* @param port the PWM port (PWM[A:C]_[0:2])
* @param defaultDirection the default direction of the motor
* @param angularCoef the angular coefficient of the motor
* In our case : code to configurate only two motors.
* The code can be easly changed to control more motors
*/
Motor::Motor(uint32_t port, bool defaultDirection, double angularCoef) :
		speed(0), angularCoef(angularCoef), defaultDirection(defaultDirection), direction(defaultDirection),
		pin(port==PWMA0?A2:A3) // The code to change to implement more PWM
{
	channel = new PWM(port, 10e3, speed);
	MyRio_ReturnIfNotSuccess(status,
			"PWM initialisation error");
	enc = new Encoder(port!=PWMA0); // if port==PWMA0 : (port!=PWMA0)=0=ENCA
	MyRio_ReturnIfNotSuccess(status,
			"Encoder initialisation error");

	setDirection(defaultDirection);
	setSpeed(0);
}
/** Get the Encoder values
* @return return the value of the encoder
*/
long Motor::getEncoderPulses() {
	uint32_t e;
	enc->read(e);
	return (long)e;
}

/** Set the speed in duty cycle
* @param speed the speed (duty cycle) of the motor
* In this function the changment of the speed is done from a value in percentage [0:100].
*/
void Motor::setSpeed(double speed) {
	this->speed = speed;
	channel->setDutyCycle(speed);
}

/** Set the speed in angular speed
* @param speed the speed (angular speed) of the motor
* In this function the changment of the speed is done from a value °/s [0:850].
*/
void Motor::setAngularSpeed(double speed) {
	if(speed<0)
		speed = - speed;
	this->speed = speed/angularCoef;
	channel->setDutyCycle(this->speed);
}

/** Set the speed in angular speed and the direction
* @param speed the speed (angular speed) and the direction of the motor 
* In this function the changment of the speed is done from a value °/s [0:850].
* The direction is done with the the funtion setDirection
*/
void Motor::setAngularSpeedAndDirection(double speed) {
	if(speed>=0)
		setDirection(defaultDirection);
	else {
		setDirection(!defaultDirection);
		speed = -speed;
	}
	this->speed = speed/angularCoef;
	channel->setDutyCycle(this->speed);
}

/** Get the speed of the motor
* @return the speed of the motor
*/
double Motor::getSpeed() {
	return speed;
}

/** Get the default direction of the motor
* @return the direction; CW -> 1 , CCW -> 0
*/
bool Motor::getDefaultDirection() {
	return defaultDirection;
}
/** Set the direction of the motor
* @param dir the direction; CW -> 1 , CCW -> 0
*/
void Motor::setDirection(bool dir) {
	this->direction = dir;
	DIO::writePin(pin, dir);
}

/** Set a function to be called when a threshold is reached.
 * The encoder will pass the current encoder value and the direction of the motor (CW -> 1 , CCW -> 0)
 * \example example_motor_pid_encoder.cpp
*/
void Motor::setInterrupt(std::function<void(long, bool)> func, unsigned int threshold) {
	enc->startThread(func, threshold);
}
/** Destructor
* Stop the motor by seting the speed to 0.
* Stop the thread of the encoders.
*/
Motor::~Motor() {
	channel->setDutyCycle(0);
	enc->stopThread();
}

