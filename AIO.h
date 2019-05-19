#ifndef AIO_H_
#define AIO_H_

#include "CAPI/MyRio.h"
#include <stdio.h> // for MyRio_ReturnIfNotSuccess
#include "MyRIO.h"

namespace myRIO {



	/** Analog output ports */
	enum {
		AO0 = AOA_0VAL,
		AO1 = AOA_1VAL,
		BO0 = AOB_0VAL,
		BO1 = AOB_1VAL,
		CO0 = AOC_0VAL,
		CO1 = AOC_0VAL
	};
	
	/** Analog inputs ports */
	enum {
		AI0 = AIA_0VAL,
		AI1 = AIA_1VAL,
		AI2 = AIA_2VAL,
		AI3 = AIA_3VAL,
		BI0 = AIB_0VAL,
		BI1 = AIB_1VAL,
		BI2 = AIB_2VAL,
		BI3 = AIB_3VAL,
		CI0 = AIC_0VAL,
		CI1 = AIC_0VAL
	};
	
	

	/**
	 * Read or write an analog value through a 12 bits DAC/ADC
	 */
	class AIO {
	public:
		static void writePin(int pin, double value);
		static double readPin (int pin);
	};
} // namespace

#endif
