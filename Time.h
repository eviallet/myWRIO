#ifndef TIME_H_
#define TIME_H_

#include <time.h>
#include <sys/time.h>
#include <iostream>

namespace myRIO {


/**
* Stopwatch or countdown
*/
class Time {
public:
	Time(unsigned long sec = 0, unsigned long nsec = 0);
	static Time stopwatch();
	void reset();
	long elapsed_ns();
	~Time();

	static void wait_s(long int s); 
	static void wait_ms(long int ms);
	static void wait_us(long int us);

private:
	unsigned long startTimeSec; /**< The start time in seconds*/
	unsigned long startTimeNsec; /**< The start time in nanosecond*/
};

}

#endif /* TIME_H_ */
