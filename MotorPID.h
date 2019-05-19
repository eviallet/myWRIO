#ifndef MotorPID_H_
#define MotorPID_H_

#include "MyRIO.h"
#include "Time.h"

#define NB_AVG 40

namespace myRIO {

/**
 * This class can be used to regulate a motor speed.
 */
class MotorPID {
public:
	MotorPID(double kp, double ki);

	double compute(long enc);
	void setSetpoint(double setpoint);
	double getSetpoint();

	double getAvgSpeed();

	~MotorPID();
private:
	Time stopwatch; /**< Timer to get the elapsed tiem since the last refresh */

	const double toAngularSpeed = 360./(12.*52.734);
	/**< The encoder gives 12 clock edges for a complete turn (360°). The gear ratio is 1/52.734 ; hence, an edge is 0.568893° */

	double lastSpeeds[NB_AVG]; /**< Moving average filter */

	long lastEnc; /**< The value of the encoder, last time mesured*/
	double setpoint; /**< The setpoint of the motors*/
	double errSum; /**< The sum of the errors
	* Errors from integration.
	*/

	double kp; /**< Proportional coefficient*/
	double ki; /**< integral coefficient*/
};

}



#endif /* MotorPID_H_ */
