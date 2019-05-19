#include "Gyro.h"

using namespace myRIO;
using namespace GyroRegisters;

/**
* Constructs a helper for sensor L3G4200D
*/
Gyro::Gyro() : regs(new uint8_t[6]), gyroStatusVal(0), xOff(0), yOff(0), zOff(0), th(nullptr), run(new bool) {
	gyro = I2C::open()->slave(0b11010010);

	uint8_t ad = 0;
	gyro->read(L3G4200D_WHO_AM_I, &ad);
	if(ad!=0b11010011) {
		status = -1;
		return;
	}

	if(!gyro->write(L3G4200D_CTRL_REG1,
				   0b00<<6 | // DR<1:0> : data rate
				   0b00<<4 | // BW<1:0> : bandwidth
				   0b1111)   // power mode : normal, X/Y/Z enabled
		||!gyro->write(L3G4200D_CTRL_REG5,0x80)) // 250dps : 8.75mdps/LSB
		status = -1;
}

/**
* Compute the offset from error of the last X samples
* The gyro must be on a plane surface.
* @param samples the number of samples to average the offset from
*/
void Gyro::calibrate(short samples) {
	double x[samples], y[samples], z[samples];

	Time t = Time::stopwatch();

	for(int i=0; i<samples; i++) {
		while(!gyroStatusVal&(1<<3)) {
			if(!gyro->read(L3G4200D_STATUS_REG, &gyroStatusVal) || status!=0) {
				status = -1;
				return;
			}
		}
		readGyro(x[i], y[i], z[i]);
	}

	unsigned long elapsed = t.elapsed_ns();

	std::cout << std::endl << "Elapsed : " << elapsed << " us, which is " << elapsed/samples << " us per sample, or"
			" a fq of " << 1.0e9/(1.*elapsed/samples) << " Hz" << std::endl << std::endl;


	for(int i=0; i<samples; i++) {
		xOff+=x[i];
		yOff+=y[i];
		zOff+=z[i];
	}

	xOff/=samples;
	yOff/=samples;
	zOff/=samples;
}

/**
* Read all X, Y and Z values from the gyro registers
* @param vx x value
* @param vy y value
* @param vz z value
*/
void Gyro::readGyro(double&vx, double&vy, double&vz) {
	gyroStatusVal = 0;

	if(!gyro->readFrom(L3G4200D_OUT_X_L, regs, 6) || status != 0) {
		status = -1;
		return;
	}

	vx = (short)((regs[1]<<8) | regs[0]);
	vx*=0.00875;
	vx-=xOff;

	vy = (short)((regs[3]<<8) | regs[2]);
	vy*=0.00875;
	vy-=yOff;

	vz = (short)((regs[5]<<8) | regs[4]);
	vz*=0.00875;
	vz-=zOff;
}

/**
* Start a thread to run infinitely that will check the gyro values
* @param func callback function, that will receive the elapsed time and the x value
*/
void Gyro::startFreeRunningMode(std::function<void(double&, double&)> func) {
	*run = true;

	if(th!=nullptr) delete th;

	th = new std::thread([this, func](bool* running) {
		Time stopwatch = Time::stopwatch();
		double vx, oldvx = 0, dt;
		while(*run) {
			// wait for new data on the gyro
			while(!gyroStatusVal&(1<<3)) {
				if(!gyro->read(L3G4200D_STATUS_REG, &gyroStatusVal) || status!=0) {
					status = -1;
					printf("Gyro - Error\n");
					return;
				}
			}

			// read that new data
			if(!gyro->readFrom(L3G4200D_OUT_X_L, regs, 2) || status != 0) {
				status = -1;
				printf("Gyro - Error\n");
				return;
			}

			// registers are L_H, we need it H_L
			vx = (short)((regs[1]<<8) | regs[0]);
			// 250dps sensitivity
			vx*=0.00875;
			// remove the offset
			vx-=xOff;

			// do not trust too much that value
			vx = vx*0.3 + oldvx*0.7;
			oldvx = vx;
			
			dt = stopwatch.elapsed_ns() * 1e-9;

			func(vx, dt);

			stopwatch.reset();
		}
	}, std::ref(run));
	th->detach();
}

/**
* Stop the running thread
*/
void Gyro::stopFreeRunningMode() {
	*run = false;
}

/**
* Close the thread if it was running
*/
Gyro::~Gyro() {
	if(*run)
		*run = false;
	delete [] regs;
}
