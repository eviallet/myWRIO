#ifndef ENCODER_H_
#define ENCODER_H_

#include "MyRio.h"
#include <thread>

namespace myRIO {

enum {
	ENCA,
	ENCB
};

class Encoder {
public:
	Encoder(uint8_t channel);
	void read(uint32_t &enc);
	void direction(bool &dir);
	void startThread(std::function<void(long enc, bool dir)> func, unsigned int threshold);
	void stopThread();

	~Encoder();
private:
	std::thread* th;
	bool* run;
	uint32_t regCnfg;
	uint32_t regCntr;
	uint32_t regStat;
};

} /* namespace myRIO */

#endif /* ENCODER_H_ */
