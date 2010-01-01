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

    // active read/write bytes methods for each section
    ReadSectionDelegate mReadSection[constSections];
    WriteSectionDelegate mWriteSection[constSections];

    // flag for each section that indicates whether or not 
    // to use the corresponding mReadSection or the faster mReadSectionMemory
    bool mMemoryMappedIOSection[constSections];
    
    // activate direct-read memorySections
    byte* mReadSectionMemory[constSections];    
   
    inline byte readByte(word address)
    {
        NW_ASSERT(address < 0x10000);
        Uint8 section = address >> constSectionShift;
        if (mMemoryMappedIOSection[section])
        {
            if (address == 0xffff && mExpandedSlotActive[section]) return mSSSRRead();
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
            mSSSRWrite(value);
        }
        else
        {
            mWriteSection[address >> constSectionShift](address, value);
        }   
    }

    // called by BusComponent::preActivate()
    void activateSection(Uint8 section, Uint8 mainslot, Uint8 subslot, bool expanded);
   
    // called by the memory BusComponents (the SlotSelector calls MemoryDevice::activate)
    void activateReadSection(Uint8 section, ReadSectionDelegate aDelegate); 
    void activateWriteSection(Uint8 section, WriteSectionDelegate aDelegate);     

    // called by the memory BusComponents
	void deactivateReadSection(Uint8 section);
	void deactivateWriteSection(Uint8 section);

    // called only by the SlotSelector
    void activateSSSRRead(SSSRReadDelegate aDelegate); 
    void activateSSSRWrite(SSSRWriteDelegate aDelegate);    

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
    friend class BusComponent;
    
    // private method, but BusComponent can access it
    void activateReadSectionMemory(Uint8 section, byte* memory);

	// null device
	NullComponent* mNullComponent;

    SSSRReadDelegate mSSSRRead;
    SSSRWriteDelegate mSSSRWrite;

    // I/O access
    IOReadDelegate mIORead[256];
    IOWriteDelegate mIOWrite[256];

    bool mExpandedSlotActive[constSections];
};

} // nowind namespace 

#endif // BUS_H
