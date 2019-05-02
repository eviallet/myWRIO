#include "Encoder.h"

using namespace myRIO;
extern NiFpga_Session myrio_session;

// 1:53 gear ratio

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

void Encoder::read(uint32_t &enc) {
	status = NiFpga_ReadU32(myrio_session, regCntr, &enc);
}

void Encoder::direction(bool &dir) {
	uint8_t tmp;
	status = NiFpga_ReadU8(myrio_session, regStat, &tmp);
	dir = tmp&0x1;
}

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

void Encoder::stopThread() {
	*run = false;
}

Encoder::~Encoder() {
	// disable encoder
	if(*run)
		stopThread();
	status = NiFpga_WriteU8(myrio_session, ENCACNFG, 0);
}
