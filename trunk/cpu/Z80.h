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

class Z80 : public CPU {

  public:
    Z80(Emulator& aEmulator);
    virtual ~Z80();

protected:
    inline word readWord(word address)
    {
        byte lowByte = mBus.readByte(address);           // read the low byte first, todo: verify that this is correct!
        return lowByte | (mBus.readByte(address+1) << 8);
    }

    inline void writeWord(word address, word value)
    {
        mBus.writeByte(address, value & 0xff);           // write the low byte first, todo: verify that this is correct!
        mBus.writeByte((address + 1) & 0xffff, value >> 8);
    }

    inline byte readIO(word port)
    { 
        return mBus.readIO(port); 
    }
    
    inline void writeIO(word port, byte value)
    { 
        mBus.writeIO(port, value); 
    }
    
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

};

} // namespace nowind

#endif
