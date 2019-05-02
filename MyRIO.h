#ifndef MYRIO_H
#define MYRIO_H

#include <iostream>
#include <math.h>
#include "CAPI/MyRio.h"

#include "Acc.h"
#include "DIO.h"
#include "Encoder.h"
#include "Gyro.h"
#include "I2C.h"
#include "PWM.h"
#include "Time.h"
#include "Wifi.h"
#include "Motor.h"
#include "PID.h"
#include "Log.h"



namespace myRIO {

	bool myRIO_init();
	bool myRIO_close();
	bool myRIO_error();

	static NiFpga_Status status;
}

static std::string printBits(uint8_t byte) {
	for(int i=7; i>=0; i--)
		std::cout << ((byte & (1 << i)) != 0)?'1':'0';
	return "";
}

#endif
