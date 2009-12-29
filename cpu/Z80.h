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

    bool mMemoryMappedIOSection[constSections];
    byte* readSectionMemory[constSections];

    inline byte readByte(word address)
    {
        NW_ASSERT(address < 0x10000);
        Uint8 section = address >> constSectionShift;
        if (mMemoryMappedIOSection[section])
        {
            if (address == 0xffff) return readSSSR();
            return readSection[section](address);
        }
        byte value = readSectionMemory[section][address & constSectionMask];
        DBERR("(d) readSectionMemory, address: $%04X, value: $%02X\n", address, value);
        return value;
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
            writeSection[address >> constSectionShift](address, value);
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

    void setupBdosEnv(const char* filename);
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
    void dumpStateInfo();

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
};

} // namespace nowind

#endif
