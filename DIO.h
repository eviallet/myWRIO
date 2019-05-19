#ifndef DIO_H_
#define DIO_H_

#include "CAPI/MyRio.h"
#include <stdio.h> // for MyRio_ReturnIfNotSuccess
#include "MyRIO.h"

#define LOW 0
#define HIGH 1

namespace myRIO {

	/** IN/OUT pin direction */
	enum {
		IN,
		OUT
	};

	/** LEDs on the chassis */
	enum {
		LED0,
		LED1,
		LED2,
		LED3
	};

	/** A port DIO */
	enum {
		A0,
		A1,
		A2,
		A3,
		A4,
		A5,
		A6,
		A7,
		A8,
		A9,
		A10,
		A11,
		A12,
		A13,
		A14,
		A15,
		A_LAST
	};

	/** B port DIO */
	enum {
		B0 = A_LAST,
		B1,
		B2,
		B3,
		B4,
		B5,
		B6,
		B7,
		B8,
		B9,
		B10,
		B11,
		B12,
		B13,
		B14,
		B15,
		B_LAST
	};

	/** C port DIO */
	enum {
		C0 = B_LAST,
		C1,
		C2,
		C3,
		C4,
		C5,
		C6,
		C7,
		C_LAST
	};

	/**
	* Digital input/output
	*/
	class DIO {
	public:
		static void writeLed(int led, bool state);
		static void writePin(int pin, bool state);
		static int readPin (int pin);
	private:
		static void parsePin(int pin, uint32_t *dirReg, uint32_t *pinsReg, bool io);
		static uint8_t DOLED30_Status;
		/**< Say LED2 is HIGH, so DOLED30 is 0100. If you set LED3 to HIGH, so 1000, without saving the other LEDs,
		 * you will only be able to use them one at a time.  */
	};
} // namespace

#endif
