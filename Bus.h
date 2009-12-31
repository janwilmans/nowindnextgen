//! Bus.h
#ifndef BUS_H
#define BUS_H

#include "basetypes.h"

// bank: in context of memoryMemoryMapper a 16kb region in memory these are max. 256 banks
//       in context of slot-selection, half a page (so a 8kb region of memory)

#include "Scheduler.h"
#include "NullComponent.h"

namespace nowind {

class Scheduler;
class BusComponent;

class Bus 
{
public:
    Bus(Emulator& aEmulator);

    // add an IO-mapped device
    void addIODevice(BusComponent* aIODevice);
    
    // IO related BusComponents use this to register
    void registerReadIO(Uint16 port, IOReadDelegate aDelegate);
    void registerWriteIO(Uint16 port, IOWriteDelegate aDelegate);

    // called directly by the CPU or other bus controller
    byte readIO(word port);
    void writeIO(word port, byte value);

    bool mMemoryMappedIOSection[constSections];
    
    /*
    //new 
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
    */
    
    // the cpu registers its memory delegates with these methods
    void registerMemRead(Uint8 section, MemReadDelegate* aDelegate);
    void registerMemWrite(Uint8 section, MemWriteDelegate* aDelegate);

    // called by the memory BusComponents (the SlotSelector calls MemoryDevice::activate)
    void activateMemReadSection(Uint8 section, MemReadDelegate aDelegate); 
    void activateMemWriteSection(Uint8 section, MemWriteDelegate aDelegate);     

	void deactivateMemReadSection(Uint8 section);
	void deactivateMemWriteSection(Uint8 section);

    // the cpu registers its SSSR delegates with these methods
    void registerSSSRRead(SSSRReadDelegate* aDelegate);
    void registerSSSRWrite(SSSRWriteDelegate* aDelegate);

    // called by the memory BusComponents (the SlotSelector calls MemoryDevice::activate)
    void activateSSSRRead(SSSRReadDelegate aDelegate); 
    void activateSSSRWrite(SSSRWriteDelegate aDelegate);     

    //quick and dirty hack
    void registerReadSectionMemory(Uint8 section, byte** readSectionMemory);
    void setReadSectionMemory(Uint8 section, byte* memory);
    byte** mReadSectionMemory[8];

    // the destructor should release any allocated resources (memory/filehandles etc.) during runtime 
    virtual ~Bus();
    
    // Component
    virtual void prepare();
    virtual void initialize();
    virtual void prepare_shutdown();    
    
protected:
    Emulator& mEmulator;
    Scheduler& mScheduler;
private:
	// null device
	NullComponent* mNullComponent;

    MemReadDelegate* mMemReadDel;
    MemWriteDelegate* mMemWriteDel;

    // memory access
    MemReadDelegate* mMemRead[constSections];
    MemWriteDelegate* mMemWrite[constSections];

    // subslot selector access
    SSSRReadDelegate* mSSSRRead;
    SSSRWriteDelegate* mSSSRWrite;

    // I/O access
    IOReadDelegate mIORead[256];
    IOWriteDelegate mIOWrite[256];

};

} // nowind namespace 

#endif // BUS_H
