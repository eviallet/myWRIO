#ifndef MYRIO_H
#define MYRIO_H

#include <iostream>
#include <math.h>
#include "CAPI/MyRio.h"

#include "Acc.h"
#include "DIO.h"
#include "AIO.h"
#include "Encoder.h"
#include "Gyro.h"
#include "I2C.h"
#include "PWM.h"
#include "Time.h"
#include "Wifi.h"
#include "Motor.h"
#include "MotorPID.h"
#include "Pendulum.h"
#include "Log.h"



namespace myRIO {

	bool myRIO_init();
	bool myRIO_close();
	bool myRIO_error();

	static NiFpga_Status status; /**< This global variable allow the user to keep track of errors if not 0. */


/**
 * This function can be used to print each bit of an unsigned char
 * @param byte The byte to print
 * @return An empty string, for convenience : cout << "Bytes : " << printBits(var) << endl;
 */
static std::string printBits(uint8_t byte) {
	for(int i=7; i>=0; i--)
		std::cout << ((byte & (1 << i)) != 0)?'1':'0';
	return "";
}

/**
* \example example_acc.cpp
* \example example_aio.cpp
* \example example_blink.cpp
* \example example_log.cpp
* \example example_motor_pid_encoder.cpp
* \example example_pwm.cpp
* \example project_angle.cpp
* \example project_car.cpp
* \example project_pendulum.cpp
*/

}

#endif
