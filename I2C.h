#ifndef I2C_H_
#define I2C_H_

#include "MyRio.h"
#include "CAPI/MyRio.h"

namespace myRIO {

union I2C_CNTL {
	uint8_t data;
	struct {
		bool tx : 1;
		bool start : 1;
		bool stop : 1;
		bool ack : 1;
	};
};

enum {
	SPD_100KBPS = 100,
	SPD_400KBPS = 400
};

enum {
	I2C_WRITE,
	I2C_READ
};

class I2C {
public:
	static I2C *open();
	void close();
	I2C* slave(uint8_t addr);
	bool write(uint8_t reg, uint8_t data);
	bool writeFrom(uint8_t reg, uint8_t* data, uint8_t dataSize);
	bool read(uint8_t reg, uint8_t*data);
	bool readFrom(uint8_t reg, uint8_t* data, uint8_t dataSize);
	I2C speed(int SPD_XXKBPS);
	bool start(bool RW, uint8_t reg, uint8_t*data, uint8_t dataSize);

	~I2C();
private:
	static int nbInstance;
	I2C();
	uint8_t wait();
	bool checkErrors(uint8_t state);


	uint8_t slaveAddr;
	uint8_t cntr;
};

} /* namespace myRIO */

#endif /* I2C_H_ */
