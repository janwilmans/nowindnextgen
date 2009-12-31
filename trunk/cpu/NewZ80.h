//! NewZ80.h
#ifndef NEWZ80_H
#define NEWZ80_H

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

class NewZ80 : public CPU {

  public:
    NewZ80(Emulator& aEmulator);
    virtual ~NewZ80();

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
    
    word m_reg_a;
    word m_reg_f;
    word m_reg_b;
    word m_reg_c;

    word m_reg_i;
    word m_reg_r;
    word m_reg_de;                // TODO: uitzoeken of DE vaker als 8 bits registers wordt gebruikt
    word m_reg_hl;
    word m_reg_pc;
    word m_reg_sp;
    word m_reg_ix;
    word m_reg_iy;
    word m_reg_wz;

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

    inline byte opcodeFetch(word);
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
};

} // namespace nowind

#endif // NEWZ80_H
