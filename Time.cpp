#include "Time.h"

using namespace myRIO;

Time::Time(long start) : startTime(start) {}

// Cannot measure more than 1s
Time Time::stopwatch() {
	struct timeval currentTime;
	gettimeofday(&currentTime, NULL);

	return Time(currentTime.tv_usec);
}

void Time::reset() {
	struct timeval currentTime;
	gettimeofday(&currentTime, NULL);

	startTime = currentTime.tv_usec;
}

long Time::elapsed_us() {
	struct timeval currentTime;
	gettimeofday(&currentTime, NULL);

	long finalTime = currentTime.tv_usec;

	if(finalTime<startTime) {
		long inRange = 1e6 - startTime;
		return finalTime + inRange;
	} else
		return finalTime - startTime;
}

long Time::elapsed_ms() {
	return elapsed_us()*1000;
}

void Time::wait_s(long int s) {
	time_t currentTime, finalTime;

    time(&currentTime);
    finalTime = currentTime + s;

    while (currentTime < finalTime)
        time(&currentTime);
}

void Time::wait_ms(long int ms) {
	struct timeval currentTime, finalTime;

	gettimeofday(&currentTime, NULL);

	// gettimeofday return µs%1e6
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

