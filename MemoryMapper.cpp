#include "MemoryMapper.h"
#include "BusComponent.h"
#include "SlotSelector.h"
#include "Bus.h"
#include "debug.h"

using namespace nowind;
using namespace fastdelegate;

MemoryMapper::MemoryMapper(Emulator& aEmulator, Uint16 kilobytes) : BusComponent(aEmulator)
{
    NW_ASSERT(((kilobytes/(16))*(16)) == kilobytes);  // kilobytes is a multiple of 16
    mBanks = kilobytes/16;
    Uint32 size = mBanks*16*1024;
    mMemory = new byte[size];       // check basetypes.h to see the ACTUAL type of 'byte'
    for (Uint32 i=0; i<size;i++) mMemory[i] = 0xff;

    mSelectedBank[0] = 3;
    mSelectedBank[1] = 2;
    mSelectedBank[2] = 1;
    mSelectedBank[3] = 0;
}

void MemoryMapper::prepare()
{

}

void MemoryMapper::initialize()
{
    
}

void MemoryMapper::prepare_shutdown()
{

}

MemoryMapper::~MemoryMapper()
{

}

void MemoryMapper::attachIO()
{
    mBus.registerReadIO(0xfc, MakeDelegate(this, &MemoryMapper::readIO));
    mBus.registerReadIO(0xfd, MakeDelegate(this, &MemoryMapper::readIO));
    mBus.registerReadIO(0xfe, MakeDelegate(this, &MemoryMapper::readIO));
    mBus.registerReadIO(0xff, MakeDelegate(this, &MemoryMapper::readIO));
    
    mBus.registerWriteIO(0xfc, MakeDelegate(this, &MemoryMapper::writeIO));
    mBus.registerWriteIO(0xfd, MakeDelegate(this, &MemoryMapper::writeIO));
    mBus.registerWriteIO(0xfe, MakeDelegate(this, &MemoryMapper::writeIO));
    mBus.registerWriteIO(0xff, MakeDelegate(this, &MemoryMapper::writeIO));
}

void MemoryMapper::detachIO()
{

}

byte MemoryMapper::readIO(word port)
{
    byte result = 0xff;
    switch(port)
    {
    case 0xfc:
        result = mSelectedBank[0];
        break;
    case 0xfd:
        result = mSelectedBank[1];
        break;
    case 0xfe:
        result = mSelectedBank[2];
        break;
    case 0xff:
        result = mSelectedBank[3];
        break;
    default:
        NW_ASSERT(false);
        break;
    }      
    return result;  
}

void MemoryMapper::writeIO(word port, byte value)
{
    DBERR("writeIO, bank: 0x%02X, value: 0x%02X\n", port, value);
    switch(port)
    {
    case 0xfc:
        mSelectedBank[0] = value;
        mSlotSelector->activatePage(0);
        break;
    case 0xfd:
        mSelectedBank[1] = value;
        mSlotSelector->activatePage(1);
        break;
    case 0xfe:
        mSelectedBank[2] = value;
        mSlotSelector->activatePage(2);
        break;
    case 0xff:
        mSelectedBank[3] = value;
        mSlotSelector->activatePage(3);
        break;
    default:
        NW_ASSERT(false);
        break;
    } 
}

void MemoryMapper::activate(Uint8 section)
{   
    //DBERR("MemoryMapper::activate section: %d\n", section);
    mBus.activateReadSection(section, MakeDelegate(this, &MemoryMapper::readByte));
    mBus.activateWriteSection(section, MakeDelegate(this, &MemoryMapper::writeByte));
    
    Uint8 page = section >> 1; // 0-3
    Uint8 currentBank = mSelectedBank[page];
    Uint32 offset = (currentBank*constPageSize) + ((section & 1) * constSectionSize);
    //DBERR("page %u, bank: %u, offset: $%04X\n", page, currentBank, offset);
    activateReadSectionMemory(section, &mMemory[offset]); 
}

byte MemoryMapper::readByte(word address)
{
    Uint8 page = address >> 14; // 0-3
    Uint8 currentBank = mSelectedBank[page];
    Uint32 offset = currentBank*constPageSize;   // todo: re-calucation of offset can be prevented if seporate readByte() per page are used
    
    byte value = mMemory[offset+(address & constPageMask)];
    //DBERR("(m) read $%04X = $%02X\n", address, value);
    return value;
}

void MemoryMapper::writeByte(word address, byte value)
{

    bool print = false;
    if (address > 0xffff) 
    {
        address &= 0xffff;  // appearently after '0' is passed, sometimes 0x10000 is received somehow?
        print = true;
    }
    
    Uint8 page = address >> 14; // 0-3
    Uint8 currentBank = mSelectedBank[page];
    Uint32 offset = currentBank*constPageSize;   // todo: re-calucation of offset can be prevented if seporate writeByte() per page are used 
    
    if (print)
    {
        DBERR("address > 0xffff!: 0x%04X, offset: 0x%04X, currentBank: %u, page: %u\n", address, offset, currentBank, page);
    }
    
    mMemory[offset+(address & constPageMask)] = value;
    //DBERR("(m) write $%04X = $%02X\n", address, value);
}
