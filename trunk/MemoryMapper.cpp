#include "MemoryMapper.h"
#include "IODevice.h"
#include "MemoryDevice.h"
#include "SlotSelector.h"
#include "debug.h"

using namespace nowind;
using namespace fastdelegate;

MemoryMapper::MemoryMapper(Bus& bus, Uint16 kilobytes) : IODevice(bus), MemoryDevice(bus)
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
    MemoryDevice::mBus.registerReadIO(0xfc, MakeDelegate(this, &MemoryMapper::readIO));
    MemoryDevice::mBus.registerReadIO(0xfd, MakeDelegate(this, &MemoryMapper::readIO));
    MemoryDevice::mBus.registerReadIO(0xfe, MakeDelegate(this, &MemoryMapper::readIO));
    MemoryDevice::mBus.registerReadIO(0xff, MakeDelegate(this, &MemoryMapper::readIO));
    
    MemoryDevice::mBus.registerWriteIO(0xfc, MakeDelegate(this, &MemoryMapper::writeIO));
    MemoryDevice::mBus.registerWriteIO(0xfd, MakeDelegate(this, &MemoryMapper::writeIO));
    MemoryDevice::mBus.registerWriteIO(0xfe, MakeDelegate(this, &MemoryMapper::writeIO));
    MemoryDevice::mBus.registerWriteIO(0xff, MakeDelegate(this, &MemoryMapper::writeIO));
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
    DBERR("MemoryMapper::activate section: %d\n", section);
    MemoryDevice::mBus.activateMemReadSection(section, MakeDelegate(this, &MemoryMapper::readByte));
    MemoryDevice::mBus.activateMemWriteSection(section, MakeDelegate(this, &MemoryMapper::writeByte));
    
    Uint8 page = section >> 1; // 0-3
    Uint8 currentBank = mSelectedBank[page];
    Uint32 offset = (currentBank*16*1024) + ((section & 1) * 8*1024);
    //DBERR("page %u, bank: %u, offset: $%04X\n", page, currentBank, offset);
    MemoryDevice::mBus.setReadSectionMemory(section, &mMemory[offset]); 
}

byte MemoryMapper::readByte(word address)
{
    Uint8 page = address >> 14; // 0-3
    Uint8 currentBank = mSelectedBank[page];
    Uint32 offset = currentBank*16*1024;   // todo: re-calucation of offset can be prevented if seporate readByte() per page are used
    
    byte value = mMemory[offset+(address & 0x3fff)];
    //DBERR("(m) read $%04X = $%02X\n", address, value);
    return value;
}

void MemoryMapper::writeByte(word address, byte value)
{
    Uint8 page = address >> 14; // 0-3
    Uint8 currentBank = mSelectedBank[page];
    Uint32 offset = currentBank*16*1024;   // todo: re-calucation of offset can be prevented if seporate writeByte() per page are used 
    mMemory[offset+(address & 0x3fff)] = value;
    //DBERR("(m) write $%04X = $%02X\n", address, value);
}

