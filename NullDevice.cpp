#include "NullDevice.h"


using namespace nowind;

NullDevice::NullDevice(Bus& bus) : IODevice(bus), MemoryDevice(bus)
{

}

NullDevice::~NullDevice()
{

}

void NullDevice::prepare()
{

}
void NullDevice::initialize()
{

}

void NullDevice::prepare_shutdown()
{

}

byte NullDevice::readIO(word port)
{
    DBERR("NULL IO read from port $%02X ($FF returned)\n", port);
    return 0xff;
}

void NullDevice::writeIO(word port, byte value)
{
    DBERR("NULL IO write to port $%02X = $%02X\n", port, value);
}

byte NullDevice::readByte(word address)
{
    DBERR("NULL read from %u-%u at $%04X ($FF returned)\n", mSlot, mSubSlot, address);
    return 0xff;
}

void NullDevice::writeByte(word address, byte value)
{
    DBERR("NULL write into %u-%u at $%04X = $%02X\n", mSlot, mSubSlot, address, value);
}

