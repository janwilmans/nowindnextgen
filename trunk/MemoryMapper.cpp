#include "MemoryMapper.h"
#include "IODevice.h"
#include "MemoryDevice.h"

using namespace nowind;
using namespace fastdelegate;

MemoryMapper::MemoryMapper(Bus& bus, Uint16 kilobytes) : IODevice(bus), MemoryDevice(bus)
{
    NW_ASSERT(((kilobytes/(16))*(16)) == kilobytes);  // kilobytes is a multiple of 16
    mBanks = kilobytes/16;
    mMemory = new byte[mBanks*16*1024];
    memset(mMemory, 0, mBanks*16*1024);

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
        break;
    case 0xfd:
        mSelectedBank[1] = value;
        break;
    case 0xfe:
        mSelectedBank[2] = value;
        break;
    case 0xff:
        mSelectedBank[3] = value;
        break;
    default:
        NW_ASSERT(false);
        break;
    } 
}

void MemoryMapper::activate(Uint8 section)
{
    DBERR("MemoryMapper::activate, for section: %d\n", section);
    MemoryDevice::mBus.activateMemReadSection(section, MakeDelegate(this, &MemoryMapper::readByte));
    MemoryDevice::mBus.activateMemWriteSection(section, MakeDelegate(this, &MemoryMapper::writeByte));

    if (section == constMaxSection)
    {
        DBERR("MemoryMapper::activate, activate SSSR\n", section);
        MemoryDevice::mBus.activateSSSRRead(MakeDelegate(this, &MemoryMapper::readSSSR));
        MemoryDevice::mBus.activateSSSRWrite(MakeDelegate(this, &MemoryMapper::writeSSSR));
    }
}

byte MemoryMapper::readByte(word address)
{
    Uint32 offset = mSelectedBank[address >> 24];
    return mMemory[offset+(address & 0x3fff)];
}

void MemoryMapper::writeByte(word address, byte value)
{
    Uint32 offset = mSelectedBank[address >> 24];
    mMemory[offset+(address & 0x3fff)] = value;
    //DBERR("write $%04X = $%02X\n", address, value);
}

byte MemoryMapper::readSSSR()
{
    return 0xFF;
}

void MemoryMapper::writeSSSR(byte value)
{

}