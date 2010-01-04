#include "ClockChip.h"
#include "debug.h"
#include "Bus.h"

using namespace nowind;
using namespace fastdelegate;

ClockChip::ClockChip(Emulator& aEmulator) : BusComponent(aEmulator)
{
	DBERR("YYOOOOO!!!!!!!\n");
}

ClockChip::~ClockChip()
{

}

void ClockChip::prepare()
{

}
void ClockChip::initialize()
{

}

void ClockChip::prepare_shutdown()
{

}

void ClockChip::attachIO()
{
    mBus.registerReadIO(0xb5, MakeDelegate(this, &ClockChip::readData));
        
    mBus.registerWriteIO(0xb4, MakeDelegate(this, &ClockChip::writeAddress));
    mBus.registerWriteIO(0xb5, MakeDelegate(this, &ClockChip::writeData));
}

void ClockChip::detachIO()
{

}

byte ClockChip::readData(word /*port*/)
{
	byte value = 0xff;
	DBERR("read register[%u] 0x%02x\n", address, value);
	return value;
}

void ClockChip::writeAddress(word port, byte value)
{
	// write address
	address = value & 15;
	DBERR("write address 0x%02x\n", address);
}

void ClockChip::writeData(word port, byte value)
{
	value &= 15;
	DBERR("write register[%u] with 0x%02x\n", address, value);
}

