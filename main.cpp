#include <iostream>
#include <cstdio>
#include <cstdlib>
#include "MyRIO.h"
#include <math.h>

//#define DEF_I2C
//#define DEF_WIFI
#define DEF_MOTOR

using namespace std;
using namespace myRIO;

double map(double x, double in_min, double in_max, double out_min, double out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

int main() {
	if(!myRIO_init()) {cout << "Error initializing myRIO"; return -1;}

#ifdef DEF_I2C
	Log log("log");

	// ACCELEROMETER
	Acc acc;
	double xAcc, lastxAcc = 180, zAcc;
	const double ACC_G = 1.042968750;
	// accelerometer filter : t = 0.1527e-3; [a, b] = createDiscreteFilter(t, t/3)
	double accFilterA = 0.6321, accFilterB = 0.3679;
	double accFiltered = 180, oldAccFiltered = 180, oldAcc = 180;

	// GYROSCOPE
	Gyro gyro;
	double xRotOff, angle;
	// preventing gyroscope from drifting
	unsigned char updateCount = 0;
	const unsigned char updateLimit = 7, difLimit = 7;
	// initializing and calibrating the gyro
	if(myRIO_error()) {cout << "Gyro - Error while initializing" << endl; return -1;}
	gyro.calibrate();
	if(myRIO_error()) {cout << "Gyro - Error while calibrating" << endl; return -1;}

	// COMPLEMENTARY FILTER
	const double GYRO_WEIGHT = 0.5;

	// starting the angle thread
	gyro.startFreeRunningMode([&](double &xRot, double &dt){
		system("clear");

		// reading accelerometer values
		acc.z(zAcc);
		acc.y(xAcc); // gyro.x orthogonal à acc.x -> acc.y est l'axe qui nous intéresse.
		xAcc = asin(xAcc/ACC_G)*180./3.1415926535898; // converting to degrees
		if(zAcc>=0) xAcc=180-xAcc; // tombe en avant : positif

		if(xAcc!=xAcc) xAcc = lastxAcc; // if acc is NaN, keep the old value

		// mapping accelerometer values to make them linear (no values between 78 - 103)
		if(xAcc<90) xAcc = map(xAcc, 0, 78.894271, 0, 90);
		else xAcc = map(xAcc, 103.148749, 180, 90, 180);


		// gyro angle at 180° will be  from calibration : offset that
		xRotOff = 180 + xRot;

		// filter accelerometer data
		accFiltered = accFilterA*oldAcc + accFilterB*oldAccFiltered;
		oldAccFiltered = accFiltered;
		oldAcc = xAcc;

		// complementary filter
		angle = GYRO_WEIGHT*xRotOff + (1.-GYRO_WEIGHT)*accFiltered;

		// if gyro values are drifting too much
		if(abs((int)xRotOff - (int)xAcc) > difLimit) {
			updateCount ++;
			// for several consecutive times
			if(updateCount >= updateLimit) {
				// then get the accelerometer value (to remove integral error)
				xRot = xAcc - 180;
				updateCount = 0;
			}
		} else
			updateCount = 0;


		printf("xRot = %f, xAcc = %f, angle = %f, dt = %f\n", xRotOff, xAcc, angle, dt*1e9);
		log.println(dt*1e9, xRotOff, xAcc, angle);
	});
	while(1);
#endif
#ifdef DEF_WIFI
	Wifi w([&](short setpoint) {});
	while(!w.isConnected());
#endif
#ifdef DEF_MOTOR
	Time t = Time::stopwatch();
	PID motorRightPid(1.5, 35);

	Motor motorLeft(PWM1, 8.846);
	motorLeft.setDirection(CW);
	motorLeft.setSpeed(20);

	Motor motorRight(PWM0, 9.32);
	motorRight.setDirection(CW);

	double setpoint = 360;
	motorRightPid.setSetpoint(setpoint);

	double a = 0.2592, b = 0.7408;
	double yf, oldYf = 0, oldU = 0;

	Log log("log");
	motorRight.setInterrupt([&](long enc, bool dir) {
		yf = a*oldU + b*oldYf;
		oldYf = yf;
		oldU = enc;

		double correctedCmd = motorRightPid.compute(yf);
		motorRight.setAngularSpeed(correctedCmd);
		log.println(t.elapsed_ns(), setpoint, correctedCmd, enc, yf);
		t.reset();
	}, 1);

	motorRight.setAngularSpeed(setpoint);
	Time::wait_s(2);
	log.close();
#endif


	return 0;
}

