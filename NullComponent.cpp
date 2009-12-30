#include "NullComponent.h"
#include "Bus.h"
#include "debug.h"

using namespace nowind;
using namespace fastdelegate;

NullComponent::NullComponent(Emulator& aEmulator) : BusComponent(aEmulator)
{
    
}

NullComponent::~NullComponent()
{
}

void NullComponent::prepare()
{
}
void NullComponent::initialize()
{
}

void NullComponent::prepare_shutdown()
{
}


byte NullComponent::readIO(word port)
{
    DBERR("NULL IO read from port $%02X ($FF returned)\n", port);
    return 0xff;
}

void NullComponent::writeIO(word port, byte value)
{
    DBERR("NULL IO write to port $%02X = $%02X\n", port, value);
}

byte NullComponent::readByte(word address)
{
    //DBERR("NULL read from %u-%u at $%04X ($FF returned)\n", mSlot, mSubSlot, address);
    return 0xff;
}

void NullComponent::writeByte(word address, byte value)
{
    //DBERR("NULL write into %u-%u at $%04X = $%02X\n", mSlot, mSubSlot, address, value);
}

void NullComponent::activate(Uint8 section)
{
    mBus.activateMemReadSection(section, MakeDelegate(this, &NullComponent::readByte));
    mBus.activateMemWriteSection(section, MakeDelegate(this, &NullComponent::writeByte));
}