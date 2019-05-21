#include "Encoder.h"

using namespace myRIO;
extern NiFpga_Session myrio_session;

// 1:53 gear ratio
/** Create an Encoder
* Enable channel A or B encoder.
* There is also an Encoder on channel C which can be added.
* @param channel one of ENCA, ENCB
*/
Encoder::Encoder(uint8_t channel) : th(nullptr), run(new bool) {
	uint32_t sysSelect;

	switch(channel) {
	case ENCA:
		sysSelect = SYSSELECTA;
		regCnfg = ENCACNFG;
		regCntr = ENCACNTR;
		regStat = ENCASTAT;
		break;
	case ENCB:
		sysSelect = SYSSELECTB;
		regCnfg = ENCBCNFG;
		regCntr = ENCBCNTR;
		regStat = ENCBSTAT;
		break;
	}
	// enable encoder in SYS.SELECT.X
	uint8_t select;
	status = NiFpga_ReadU8(myrio_session, sysSelect, &select);
	select |= (1<<5);
	NiFpga_MergeStatus(&status,
			NiFpga_WriteU8(myrio_session, sysSelect, select));

	// enable encoder
	NiFpga_MergeStatus(&status,
			NiFpga_WriteU8(myrio_session, regCnfg, 1));
}
/** Read encoder
* Reads the value of the encoder.
* @param enc the encoder address
*/
void Encoder::read(uint32_t &enc) {
	status = NiFpga_ReadU32(myrio_session, regCntr, &enc);
}

/** 
* Return the direction of the motor (CCW -> 0, CW -> 1)
*/
void Encoder::direction(bool &dir) {
	uint8_t tmp;
	status = NiFpga_ReadU8(myrio_session, regStat, &tmp);
	dir = tmp&0x1;
}

/**
 * Start a thread that will monitor the encoder register.
 * It will alert the user if a given threshold is reached, executing the given function.
 * The threshold is a variation since the last threshold reach (or 0 if none have been reached)
 * 1 increment ~ 0.58°, @see MotorPID::toAngularSpeed
 * @param func a function to execute when threshold is reached
 * @param threshold the number of increments before executing the function
*/
void Encoder::startThread(std::function<void(long enc, bool dir)> func, unsigned int threshold) {
	*run = true;

	if(th!=nullptr) delete th;

	th = new std::thread([=](bool* running){
		uint32_t enc, oldEnc = 0;
		bool dir;
		while(*running) {
			read(enc);
			direction(dir);
			if(enc-oldEnc>=threshold) {
				func((long)enc, dir);
				oldEnc = enc;
			}
		}
	}, std::ref(run));
}
/** Stop the encoder thread
*/
void Encoder::stopThread() {
	*run = false;
}

/** Destructeur
* Stop the encoder and disable it. 
*/
Encoder::~Encoder() {
	// disable encoder
	if(*run)
		stopThread();
	status = NiFpga_WriteU8(myrio_session, ENCACNFG, 0);
}
