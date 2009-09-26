//! Z80.h
#ifndef Z80_H
#define Z80_H

#include <iostream>

#include <string>
#include <sstream>
#include <map>
#include <stdlib.h>

#include "basetypes.h"
#include "FastDelegate.h"
#include "Emulator.h"
#include "debug.h"

#include "Bus.h"
#include "cpu/CPU.h"

namespace nowind {

typedef fastdelegate::FastDelegate2 < word, byte > writeDelegate;       // returns void 

/* all of these macros are using in opcodes*.inc so they are easely be adapted
   to used on different platforms and in different configurations */

#define TS(states) localEmuTime += states

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

#define READMEM readByte
#define READMEM16 readWord
#define WRITEMEM writeByte
#define WRITEMEM16 writeWord

class Z80 : public CPU {

  public:
    Z80(Bus& aBus);
    virtual ~ Z80();

protected:

    MemReadDelegate readSection[constSections];
    MemWriteDelegate writeSection[constSections];

    SSSRReadDelegate readSSSR;
    SSSRWriteDelegate writeSSSR;

    IOReadDelegate readIOPort;
    IOWriteDelegate writeIOPort;

    inline byte readByte(word address)
    {
        NW_ASSERT(address < 0x10000);
        if (address == 0xffff) return readSSSR();
        return readSection[address >> constSectionShift](address & constSectionMask);
    }

    inline void writeByte(word address, byte value)
    {   
        NW_ASSERT(address < 0x10000);
        NW_ASSERT(value < 0x100);
        if (address == 0xffff) 
        {
            writeSSSR(value);
        }
        else
        {
            writeSection[address >> constSectionShift](address & constSectionMask, value);
        }   
    }

    inline word readWord(word address)
    {
        byte lowByte = readByte(address);           // read the low byte first, todo: verify that this is correct!
        return lowByte | (readByte(address+1) << 8);
    }

    inline void writeWord(word address, word value)
    {
        writeByte(address, value & 0xff);           // write the low byte first, todo: verify that this is correct!
        writeByte((address + 1) & 0xffff, value >> 8);
    }

    inline byte readIO(word port);
    inline void writeIO(word port, byte value);
public:

    // Component interface
    virtual void prepare();
    virtual void initialize();
    virtual void prepare_shutdown();

private:

    float opcodeCounter[256];
    float opcodeCounterCB[256];
    float opcodeCounterDD[256];
    float opcodeCounterED[256];
    float opcodeCounterFD[256];

    /* interne z80 registers */
    word reg_a;
    word reg_f;
    word reg_b;
    word reg_c;

    word reg_i;
    word reg_r;
    word reg_de;                // TODO: uitzoeken of DE vaker als 8 bits registers wordt gebruikt
    word reg_hl;
    word reg_pc;
    word reg_sp;
    word reg_ix;
    word reg_iy;
    word reg_wz;

    word shadow_af;
    word shadow_b;
    word shadow_c;
    word shadow_de;
    word shadow_hl;

    bool IFF1;
    bool IFF2;
    word interruptMode;
    word refreshCounter;

    word flagSZ[256];
    word flagSZsub[256];
    word flagSZP[256];
    word flagInc[256];
    word flagDec[256];

    byte opcodeFetch(word);
    void debugInstuctionCounter();

  public:

    Uint32 cpuFrequency;

    void reset();
    void nmiCPU();
    bool getIFF1();
    void intCPU(byte);
    void setPC(word);

    emuTimeType ExecuteInstructions(emuTimeType startTime, emuTimeType endTime);

    void saveState();
    void loadState();

    emuTimeType mEndTime;

    void hijackBdos();
    //void dumpPages();
    void dumpSlotSelection();
    void dumpCpuInfo();
    word getSP();
};

} // namespace nowind

#endif
