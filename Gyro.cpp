// TODO
// complementary filter (anciennces valeurs gyro)
// bande passante


#include "Gyro.h"

using namespace myRIO;

Gyro::Gyro() : regs(new uint8_t[6]), gyroStatusVal(0), xOff(0), yOff(0), zOff(0), th(nullptr), run(new bool) {
	gyro = I2C::open()->slave(0b11010010);

	uint8_t ad = 0;
	gyro->read(L3G4200D_WHO_AM_I, &ad);
	if(ad!=0b11010011) {
		status = -1;
		return;
	}

	if(!gyro->write(L3G4200D_CTRL_REG1,
				   0b11<<6 | // DR<1:0> : data rate
				   0b00<<4 | // BW<1:0> : bandwidth
				   0b1111)   // power mode : normal, X/Y/Z enabled
		||!gyro->write(L3G4200D_CTRL_REG5,0x80)) // 250dps : 8.75mdps/LSB
		status = -1;
}

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


void Gyro::startFreeRunningMode(std::function<void(double&, double&)> func) {
	*run = true;

	if(th!=nullptr) delete th;

	th = new std::thread([this, func](bool* running) {
		Time stopwatch = Time::stopwatch();
		double x, vx;
		double oldx = 0, oldvx = 0;
		double dt;
		while(*run) {
			// wait for new data on the gyro
			while(!gyroStatusVal&(1<<3)) {
				if(!gyro->read(L3G4200D_STATUS_REG, &gyroStatusVal) || status!=0) {
					status = -1;
					printf("Error\n");
					return;
				}
			}

			// read that new data
			if(!gyro->readFrom(L3G4200D_OUT_X_L, regs, 2) || status != 0) {
				status = -1;
				printf("Error\n");
				return;
			}

			vx = (short)((regs[1]<<8) | regs[0]);
			vx*=0.00875;
			vx-=xOff;

			if(abs(oldvx-vx)<1)
				vx = oldvx;

			dt = stopwatch.elapsed_ns() * 1e-9;
			x = oldx + vx * dt * 1e3;

			func(x, dt);

			stopwatch.reset();

			oldx = x;
			oldvx = vx;
		}
	}, std::ref(run));
	th->detach();
}

void Gyro::stopFreeRunningMode() {
	*run = false;
}


Gyro::~Gyro() {
	if(*run)
		*run = false;
	delete [] regs;
}
