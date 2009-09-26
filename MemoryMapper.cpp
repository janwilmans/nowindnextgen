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
    return memory[address];
}

void MemoryMapper::writeByte(word address, byte value)
{
    memory[address] = value;
    //DBERR("write $%04X = $%02X\n", address, value);
}

byte MemoryMapper::readSSSR()
{
    return 0xFF;
}

void MemoryMapper::writeSSSR(byte value)
{

}