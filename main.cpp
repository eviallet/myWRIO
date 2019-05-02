#include <iostream>
#include <cstdio>
#include <cstdlib>
#include "MyRIO.h"

//#define DEF_I2C
//#define DEF_WIFI
#define DEF_MOTOR

using namespace std;
using namespace myRIO;

int main() {
	if(!myRIO_init()) {
		cout << "Error initializing myRIO"; return -1;
	}

#ifdef DEF_I2C
	Gyro gyro;
	if(myRIO_error()) {
		cout << "Gyro - Error while initializing" << endl; return -1;
	}
	gyro.calibrate();
	if(myRIO_error()) {
		cout << "Gyro - Error while calibrating" << endl; return -1;
	}
	gyro.startFreeRunningMode([](double vx){
		system("clear");
		printf("x = %f\n", vx);
	});
#endif
#ifdef DEF_WIFI
	Wifi w([&](short setpoint) {});
	while(!w.isConnected());
#endif
#ifdef DEF_MOTOR
	Time t = Time::stopwatch();

	Motor motorRight(PWM0);
	motorRight.setSpeed(0);

	Motor motorLeft(PWM1);
	motorLeft.setDirection(CW);
	int setpoint = 25;

	Log log("log");
	motorLeft.setInterrupt([&](long enc, bool dir) {
		//cout << t.elapsed_us() << "  " << enc << endl;
		log.println(t.elapsed_us(), setpoint, enc);
		t.reset();
	}, 1);
	motorLeft.setSpeed(0);
	Time::wait_s(1);
	t.reset();
	motorLeft.setSpeed(setpoint);
	Time::wait_s(1);
	setpoint = 50;
	motorLeft.setSpeed(setpoint);
	Time::wait_s(1);
	motorLeft.setSpeed(0);
	Time::wait_s(1);
	log.close();
#endif


	return 0;
}

