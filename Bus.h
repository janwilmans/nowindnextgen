//! Bus.h
#ifndef BUS_H
#define BUS_H

#include "basetypes.h"

// bank: in context of memoryMemoryMapper a 16kb region in memory these are max. 256 banks
//       in context of slot-selection, half a page (so a 8kb region of memory)

#include "Scheduler.h"
#include "NullComponent.h"
#include "debug.h"

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

    //new
    MemReadDelegate mReadSection[constSections];
    MemWriteDelegate mWriteSection[constSections];

    bool mMemoryMappedIOSection[constSections];
    byte* mReadSectionMemory[constSections];    
   
    //new 
    inline byte readByte(word address)
    {
        NW_ASSERT(address < 0x10000);
        Uint8 section = address >> constSectionShift;
        if (mMemoryMappedIOSection[section])
        {
            if (address == 0xffff) return mSSSRReadDel();
            return mReadSection[section](address);
        }
        byte value = mReadSectionMemory[section][address & constSectionMask];
        //DBERR("(d) mReadSectionMemory, address: $%04X, value: $%02X\n", address, value);
        return value;
    }

    inline void writeByte(word address, byte value)
    {   
        NW_ASSERT(address < 0x10000);
        NW_ASSERT(value < 0x100);
        if (address == 0xffff) 
        {
            mSSSRWriteDel(value);
        }
        else
        {
            mWriteSection[address >> constSectionShift](address, value);
        }   
    }
   
    // called by the memory BusComponents (the SlotSelector calls MemoryDevice::activate)
    void activateMemReadSection(Uint8 section, MemReadDelegate aDelegate); 
    void activateMemWriteSection(Uint8 section, MemWriteDelegate aDelegate);     

	void deactivateMemReadSection(Uint8 section);
	void deactivateMemWriteSection(Uint8 section);

     // called by the memory BusComponents (the SlotSelector calls MemoryDevice::activate)
    void activateSSSRRead(SSSRReadDelegate aDelegate); 
    void activateSSSRWrite(SSSRWriteDelegate aDelegate);     

    void setReadSectionMemory(Uint8 section, byte* memory);

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

    //new
    SSSRReadDelegate mSSSRReadDel;
    SSSRWriteDelegate mSSSRWriteDel;

    // I/O access
    IOReadDelegate mIORead[256];
    IOWriteDelegate mIOWrite[256];

};

} // nowind namespace 

#endif // BUS_H
