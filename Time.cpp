#include "Time.h"

using namespace myRIO;

Time::Time(unsigned long sec, unsigned long nsec) : startTimeSec(sec), startTimeNsec(nsec) {}

// Cannot measure more than 4s
Time Time::stopwatch() {
	struct timespec currentTime;
	clock_gettime(CLOCK_MONOTONIC, &currentTime);

	return Time(currentTime.tv_sec, currentTime.tv_nsec);
}

void Time::reset() {
	struct timespec currentTime;
	clock_gettime(CLOCK_MONOTONIC, &currentTime);

	startTimeSec = currentTime.tv_sec;
	startTimeNsec = currentTime.tv_nsec;
}

long Time::elapsed_ns() {
	struct timespec currentTime;
	clock_gettime(CLOCK_MONOTONIC, &currentTime);

	unsigned long d_sec = currentTime.tv_sec - startTimeSec;
	long d_nsec =  startTimeNsec - currentTime.tv_nsec;
	unsigned long finalTime = d_sec * 1e9L - d_nsec;

	return finalTime;
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

