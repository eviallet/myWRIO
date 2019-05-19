#include "Time.h"

using namespace myRIO;
/** Constructor
* Create a time object referencing the given time
*/
Time::Time(unsigned long sec, unsigned long nsec) : startTimeSec(sec), startTimeNsec(nsec) {}

/** Get the current time
* @return creates a time object with the current time
*/
Time Time::stopwatch() {
	struct timespec currentTime;
	clock_gettime(CLOCK_MONOTONIC, &currentTime);

	return Time(currentTime.tv_sec, currentTime.tv_nsec);
}

/** Reset timer to the current time
*/
void Time::reset() {
	struct timespec currentTime;
	clock_gettime(CLOCK_MONOTONIC, &currentTime);

	startTimeSec = currentTime.tv_sec;
	startTimeNsec = currentTime.tv_nsec;
}

/** Elapsed time nanoseconds
* If you get elapsed_ns() = 1000, 1000ns are elapsed, or 1000e-9 seconds
* WARNING : cannot measure more than 2 seconds (long limit = 2e9; 1 seconds in ns = 1e9)
* @return the elapsed time in nanoseconds
*/
long Time::elapsed_ns() {
	struct timespec currentTime;
	clock_gettime(CLOCK_MONOTONIC, &currentTime);

	unsigned long d_sec = currentTime.tv_sec - startTimeSec;
	long d_nsec =  startTimeNsec - currentTime.tv_nsec;
	unsigned long finalTime = d_sec * 1e9L - d_nsec;

	return finalTime;
}

/** Wait function in seconds
* @param s the seconds to wait
*/
void Time::wait_s(long int s) {
	time_t currentTime, finalTime;

    time(&currentTime);
    finalTime = currentTime + s;

    while (currentTime < finalTime)
        time(&currentTime);
}

/** Wait function in milliseconds
* @param ms the milliseconds to wait
*/
void Time::wait_ms(long int ms) {
	struct timeval currentTime, finalTime;

	gettimeofday(&currentTime, NULL);

	// gettimeofday return Âµs%1e6
	if(currentTime.tv_usec + ms*1000L >= 1e6) { // if we overflowed
		time_t inRange = 1e6 - currentTime.tv_usec; // we counted $inRange µs
		time_t left = 1000L*ms - inRange; // so there is $left left from 0

		while(currentTime.tv_usec > left) // wait to overflow
			gettimeofday(&currentTime, NULL);
		while(currentTime.tv_usec < left) // then count until $left
			gettimeofday(&currentTime, NULL);
	} else {
		finalTime.tv_usec = currentTime.tv_usec + ms*1000L;
		while(currentTime.tv_usec < finalTime.tv_usec)
			gettimeofday(&currentTime, NULL);
	}
}

/** Wait function in microseconds
* @param us the microseconds to wait
*/
void Time::wait_us(long int us) {
	struct timeval currentTime, finalTime;

	gettimeofday(&currentTime, NULL);

	if(currentTime.tv_usec + us > 1e6) { // if we overflowed
		time_t inRange = 1e6 - currentTime.tv_usec; // we counted $inRange µs
		time_t left = us - inRange; // so there is $left left from 0

		while(currentTime.tv_usec > left) // wait to overflow
			gettimeofday(&currentTime, NULL);
		while(currentTime.tv_usec < left) // then count until $left
			gettimeofday(&currentTime, NULL);
	} else {
		finalTime.tv_usec = currentTime.tv_usec + us;
		while (currentTime.tv_usec < finalTime.tv_usec)
			gettimeofday(&currentTime, NULL);
	}
}

Time::~Time() {

}

