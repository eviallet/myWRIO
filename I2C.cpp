#include "I2C.h"

using namespace myRIO;
extern NiFpga_Session myrio_session; // global variable defined in CAPI/MyRio.c
int I2C::nbInstance = 0;

I2C::I2C() : slaveAddr(0) {
	nbInstance++;

	// enable I2C in SYS.SELECT
	uint8_t select;
	status = NiFpga_ReadU8(myrio_session, SYSSELECTA, &select);
	select |= (1<<7);
	NiFpga_MergeStatus(&status,
			NiFpga_WriteU8(myrio_session, SYSSELECTA, select));
	// enable I2C in I2C.CNFG
	NiFpga_MergeStatus(&status,
			NiFpga_WriteU8(myrio_session, I2CACNFG, 1));
	// setup default clock speed
	// 40e6/xe3 : 40e3/x (speed is in KBPS, not in BPS)
	cntr = (uint8_t)(((double)40e3/SPD_400KBPS)+26.0)/2.0;
}

I2C* I2C::open() {
	// TODO I2C B when already 1 instance
	if(nbInstance==0)
		return new I2C();
	else
		return nullptr;
}

I2C* I2C::slave(uint8_t addr) {
	slaveAddr = addr;
	return this;
}

bool I2C::write(uint8_t reg, uint8_t data) {
	return writeFrom(reg, new uint8_t(data), 1);
}

bool I2C::writeFrom(uint8_t reg, uint8_t*data, uint8_t dataSize) {
	return start(I2C_WRITE, reg, data, dataSize);
}


bool I2C::read(uint8_t reg, uint8_t*data) {
	return readFrom(reg, data, 1);
}

bool I2C::readFrom(uint8_t reg, uint8_t*data, uint8_t dataSize) {
	if(dataSize>1)
		reg|=0x80; // enable multi byte read
	return start(I2C_READ, reg, data, dataSize);
}

I2C I2C::speed(int spd) {
	cntr = (uint8_t)(((double)40e3/spd)+26.0)/2.0;
	return *this;
}

bool I2C::start(bool RW, uint8_t reg, uint8_t*data, uint8_t dataSize) {
#ifdef DEBUG
	std::cout << "START " << ((RW==I2C_READ)?"READ":"WRITE") << std::endl;
#endif
	uint8_t state;
	union I2C_CNTL cntl = {0};

	// setup I2C bus clock
	status = NiFpga_WriteU8(myrio_session, I2CACNTR, cntr);

	/*
	 * Generate start
	 * Send address + RW
	 * Receive address ACK
	 * Send data (register addr)
	 * Receive data ACK
	 * Go to TX/RX IDLE state
	 */
	cntl.tx = true;
	cntl.start = true;

	// choose the slave address and set W
	NiFpga_MergeStatus(&status,
			NiFpga_WriteU8(myrio_session, I2CAADDR, slaveAddr));

	// write procedure to myRIO
	NiFpga_MergeStatus(&status,
			NiFpga_WriteU8(myrio_session, I2CACNTL, cntl.data));
#ifdef DEBUG
	std::cout << "CNTL : " << printBits(cntl.data) << " - Slave : " << printBits(slaveAddr) << " - Reg : " << printBits(reg) << std::endl;
#endif
	// choose the device register to operate
	NiFpga_MergeStatus(&status,
			NiFpga_WriteU8(myrio_session, I2CADATO, reg));

	// start the first procedure
	NiFpga_MergeStatus(&status,
			NiFpga_WriteBool(myrio_session, I2CAGO, NiFpga_True));

	// wait for the end of the communication
	state = wait();
	
	// if there was an error, stop
	if(!checkErrors(state)) return false;
	
	// choose the slave address and set R/W
	NiFpga_MergeStatus(&status,
			NiFpga_WriteU8(myrio_session, I2CAADDR, slaveAddr|RW));

	for(int i=0; i<dataSize; i++) {
		/*
		 * if(i==0&&reading) Generate REPEATED START
		 * if(i==0) Send address + read/write
		 * if(i==0) Receive address ACK
		 * Receive/send data
		 * Receive data ACK/send data ACK/(if i==size-1)NACK
		 * if(i==size-1) Generate STOP
		 * Go to if(i!=size-1)TX|RX IDLE/(else)IDLE state
		 */
		cntl = {0};
		cntl.tx = true;
		if(i==0 && RW==I2C_READ) cntl.start = true;
		if(i==dataSize-1)
			cntl.stop = true;
		else if(RW==I2C_READ)
			cntl.ack = true;
#ifdef DEBUG
		std::cout << "CNTL : " << printBits(cntl.data) << " - ";
#endif
		// write the procedure
		NiFpga_MergeStatus(&status,
				NiFpga_WriteU8(myrio_session, I2CACNTL, cntl.data));

		// if we are writing, setup the "DATA OUT" register
		if(RW==I2C_WRITE) {
			NiFpga_MergeStatus(&status,
					NiFpga_WriteU8(myrio_session, I2CADATO, *(data+i)));
#ifdef DEBUG
			std::cout << "Wrote : " << printBits(*(data+i)) << std::endl;
#endif
		}

		// start the procedure
		NiFpga_MergeStatus(&status,
				NiFpga_WriteBool(myrio_session, I2CAGO, NiFpga_True));

		// wait for the end of the communication
		state = wait();
		
		// if there was an error, stop
		if(!checkErrors(state)) return false;

		// if we are reading, read the "DATA IN" register
		if(RW==I2C_READ) {
			NiFpga_MergeStatus(&status,
					NiFpga_ReadU8(myrio_session, I2CADATI, data+i));
#ifdef DEBUG
			std::cout << "Received : " << printBits(*(data+i)) << std::endl;
#endif
		}
	}
	return true;
}

uint8_t I2C::wait() {
	uint8_t state;
	do {
		NiFpga_MergeStatus(&status,
				NiFpga_ReadU8(myrio_session, I2CASTAT, &state));
	} while(state&1); // BSY flag
	return state;
}

bool I2C::checkErrors(uint8_t state) {
	if(state&0x02) {
		union I2C_CNTL cntl = {0};
		cntl.stop = true;
		NiFpga_MergeStatus(&status,
				NiFpga_WriteU8(myrio_session, I2CACNTL, cntl.data));
		return false;
	}
	return true;
}

void I2C::close() {
	delete this;
}

I2C::~I2C() {
	status = NiFpga_WriteU8(myrio_session, I2CACNFG, 0);
}

