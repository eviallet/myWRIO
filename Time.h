#ifndef TIME_H_
#define TIME_H_

#include <time.h>
#include <sys/time.h>
#include <iostream>

namespace myRIO {

class Time {
public:
	Time(long start = 0);
	static Time stopwatch();
	void reset();
	long elapsed_us();
	long elapsed_ms();
	~Time();

	static void wait_s(long int s);
	static void wait_ms(long int ms);
	static void wait_us(long int us);

private:
	long startTime;
};

}

#endif /* TIME_H_ */
