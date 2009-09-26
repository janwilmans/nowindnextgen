#include "MemoryMapper.h"
#include "IODevice.h"
#include "MemoryDevice.h"

using namespace nowind;

MemoryMapper::MemoryMapper(Bus& bus, Uint16 kilobytes) : IODevice(bus), MemoryDevice(bus)
{
    NW_ASSERT(((kilobytes/(16))*(16)) == kilobytes);  // kilobytes is a multiple of 16
    mBanks = kilobytes/16;
    mMemory = new byte[mBanks*16*1024];
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

}
