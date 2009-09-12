//! Z80.h
#ifndef Z80_H
#define Z80_H

#include <iostream>

#include <string>
#include <sstream>
#include <map>
#include <stdlib.h>

#include "msxtypes.h"
#include "FastDelegate.h"
#include "Emulator.h"

//#define inline  __forceinline
//#define inline_after __attribute__((always_inline))
#define inline_after ;//
// class functies nooit inline ?
// mogelijke speed-up: readmem etc. uit de class halen? 

typedef fastdelegate::FastDelegate2<word, byte> writeDelegate;   // returns void 


/* all of these macros are using in opcodes*.inc so they are easely be adapted
   to used on different platforms and in different configurations */

#define TS(states) mEmuTime += states

#define reg_af ((reg_a << 8) | reg_f)
#define reg_bc ((reg_b << 8) | reg_c)

#define reg_d (reg_de >> 8)
#define reg_e (reg_de & 255)

#define reg_h (reg_hl >> 8)
#define reg_l (reg_hl & 255)

#define SFLAG 0x80
#define ZFLAG 0x40
#define YFLAG 0x20
#define HFLAG 0x10
#define XFLAG 0x08
#define PFLAG 0x04
#define NFLAG 0x02
#define CFLAG 0x01

/* memory read/write macros */
#define READMEM readMem
#define READMEM16 readMem16
#define WRITEMEM writeMem
#define WRITEMEM16 writeMem16

#include "cpu/CPU.h"

class Z80 : public CPU {

public:
	Z80(AddressBus& addressBus, IOBus& ioBus);
	virtual ~Z80();
private:
    Emulator* mEmulator;

    // for testing only
    byte memblock[64*1024];

    float opcodeCounter[256];
    float opcodeCounterCB[256];
    float opcodeCounterDD[256];
    float opcodeCounterED[256];
    float opcodeCounterFD[256];
    
    /* new memory layout */
    const byte   *readBlock[8];
    writeDelegate   writeFunc[8];

	/* interne z80 registers */
	word			reg_a;
	word			reg_f;
	word			reg_b;
	word			reg_c;
	
	word		    reg_i;
	word    		reg_r;
	word			reg_de;		// TODO: uitzoeken of DE vaker als 8 bits registers wordt gebruikt
	word			reg_hl;
	word    		reg_pc;
	word    		reg_sp;
	word    		reg_ix;
    word    		reg_iy;
	word			reg_wz;

	word    		shadow_af;
	word   		    shadow_b;
	word			shadow_c;
	word    		shadow_de;
	word    		shadow_hl;

	bool            IFF1;
	bool            IFF2;
	word    		interruptMode;
	word			refreshCounter;

	word    		flagSZ[256];
	word    		flagSZsub[256];
	word    		flagSZP[256];
	word    		flagInc[256];
	word    		flagDec[256];

	inline byte	opcodeFetch(word) inline_after;
	void            debugInstuctionCounter();

	void            writeIo(word, word);
	byte            readIo(word);

	inline byte	readMem(word) inline_after;
	inline word	readMem16(word) inline_after;
	inline void writeMem(word, byte) inline_after;
	inline void writeMem16(word, word) inline_after;	
	
public:
	byte readMemPublic(word address);
	word readMem16Public(word address);
	void writeMemPublic(word address, byte value);
	void writeMem16Public(word address, word value);

	unsigned long   cpuFrequency;
	word            start_test_nr;
	void            initialize();

	void            reset();
	void            nmiCPU();
	bool			getIFF1();
	void            intCPU(byte);
	void            setPC(word);

	emuTimeType		ExecuteInstructionsUntil(emuTimeType startTime, emuTimeType endTime);

	void            saveState();
	void            loadState();

	emuTimeType     mEmuTime;

	void            hijackBdos();
	void            dumpPages();
	void            dumpSlotSelection();
	void            dumpCpuInfo();		
	word            getSP();
};

#endif
