#include "Pendulum.h"

using namespace myRIO;

Pendulum::Pendulum(double k1, double k2) :
		setpoint(0), k1(k1), k2(k2) {}

void Pendulum::setSetpoint(double setpoint) {
	this->setpoint = setpoint;
}

double Pendulum::compute(double teta, double teta_p) {
	double output = k1*(teta-tetaRef)+k2*teta_p;

	if(output < -850) output = -850;
	if(output > 850) output = 850;

	return -output;
}

Pendulum::~Pendulum() {}
