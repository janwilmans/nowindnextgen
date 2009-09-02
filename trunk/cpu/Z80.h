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

typedef fastdelegate::FastDelegate2<nw_word, nw_byte> writeDelegate;   // returns void 


/* all of these macros are using in opcodes*.inc so they are easely be adapted
   to used on different platforms and in different configurations */

#define TS(states) emuTime += states

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

#include "cpu/ICPU.h"

class Z80 : public ICPU {

public:
	Z80();
	virtual ~Z80();
private:
    Emulator* mEmulator;

    float opcodeCounter[256];
    float opcodeCounterCB[256];
    float opcodeCounterDD[256];
    float opcodeCounterED[256];
    float opcodeCounterFD[256];
    
    /* new memory layout */
    const nw_byte   *readBlock[8];
    writeDelegate   writeFunc[8];

	/* interne z80 registers */
	nw_word			reg_a;
	nw_word			reg_f;
	nw_word			reg_b;
	nw_word			reg_c;
	
	nw_word		    reg_i;
	nw_word    		reg_r;
	nw_word			reg_de;		// TODO: uitzoeken of DE vaker als 8 bits registers wordt gebruikt
	nw_word			reg_hl;
	nw_word    		reg_pc;
	nw_word    		reg_sp;
	nw_word    		reg_ix;
    nw_word    		reg_iy;
	nw_word			reg_wz;

	nw_word    		shadow_af;
	nw_word   		shadow_b;
	nw_word			shadow_c;
	nw_word    		shadow_de;
	nw_word    		shadow_hl;

	bool            IFF1;
	bool            IFF2;
	nw_word    		interruptMode;
	nw_word			refreshCounter;

	nw_word    		flagSZ[256];
	nw_word    		flagSZsub[256];
	nw_word    		flagSZP[256];
	nw_word    		flagInc[256];
	nw_word    		flagDec[256];

	inline nw_byte	opcodeFetch(nw_word) inline_after;
	void            debugInstuctionCounter();

	void            writeIo(nw_word, nw_word);
	nw_byte         readIo(nw_word);

	inline nw_byte	readMem(nw_word) inline_after;
	inline nw_word	readMem16(nw_word) inline_after;
	inline void     writeMem(nw_word,nw_word) inline_after;
	inline void     writeMem16(nw_word,nw_word) inline_after;	
	
public:
	nw_byte			readMemPublic(nw_word address);
	nw_word			readMem16Public(nw_word address);
	void 			writeMemPublic(nw_word address, nw_byte value);
	void 			writeMem16Public(nw_word address, nw_word value);

    //emuTimeType     nextInterrupt;
	unsigned long   cpuFrequency;

	nw_word         start_test_nr;
	void            initialize();

	void            reset();
	void            hardReset();
	void            nmiCPU();
	bool			getIFF1();
	void            intCPU(nw_byte);
	void            start(nw_word);

	emuTimeType		ExecuteInstructionsUntil(emuTimeType endTime);

	void            saveState();
	void            loadState();

	emuTimeType     emuTime;

	void            hijackBdos();
	unsigned long   bdosCount;
	void            dumpPages();
	void            dumpSlotSelection();
	void            dumpCpuInfo();		
    void            abortEmulator();
	nw_word         getSP();
};

#endif
