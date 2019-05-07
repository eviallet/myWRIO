#ifndef GYRO_H_
#define GYRO_H_

#include "MyRIO.h"
#include "I2C.h"
#include "Time.h"
#include <thread>

/*
 * VERT = A34 = SDA
 * JAUNE = A32 = SCL
 * ROUGE = A33 = VCC
 * NOIR = A30 = GND
 */

namespace myRIO {

constexpr uint8_t L3G4200D_WHO_AM_I   = 0x0F;
constexpr uint8_t L3G4200D_CTRL_REG1  = 0x20;
constexpr uint8_t L3G4200D_CTRL_REG2  = 0x21;
constexpr uint8_t L3G4200D_CTRL_REG3  = 0x22;
constexpr uint8_t L3G4200D_CTRL_REG4  = 0x23;
constexpr uint8_t L3G4200D_CTRL_REG5  = 0x24;
constexpr uint8_t L3G4200D_STATUS_REG = 0x27;
constexpr uint8_t L3G4200D_OUT_X_L    = 0x28;
constexpr uint8_t L3G4200D_OUT_X_H    = 0x29;
constexpr uint8_t L3G4200D_OUT_Y_L    = 0x2A;
constexpr uint8_t L3G4200D_OUT_Y_H    = 0x2B;
constexpr uint8_t L3G4200D_OUT_Z_L    = 0x2C;
constexpr uint8_t L3G4200D_OUT_Z_H    = 0x2D;

class I2C;

class Gyro {
public:
	Gyro();
	~Gyro();
	void calibrate(short samples = 100);
	void readGyro(double&vx, double&vy, double&vz);
	void startFreeRunningMode(std::function<void(double&, double&)> func);
	void stopFreeRunningMode();

private:
	uint8_t *regs;
	uint8_t gyroStatusVal;
	I2C* gyro;
	double xOff, yOff, zOff;
	std::thread *th;
	bool* run;
};

} /* namespace myRIO */

#endif /* GYRO_H_ */
