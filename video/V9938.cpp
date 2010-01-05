#include "V9938.h"
#include "debug.h"
#include "Bus.h"

using namespace nowind;
using namespace fastdelegate;

V9938::V9938(Emulator& aEmulator) : BusComponent(aEmulator)
{

}

V9938::~V9938()
{

}

void V9938::prepare()
{

}
void V9938::initialize()
{

}

void V9938::prepare_shutdown()
{

}

void V9938::attachIO()
{
    mBus.registerReadIO(0x98, MakeDelegate(this, &V9938::readPort0));
    mBus.registerReadIO(0x99, MakeDelegate(this, &V9938::readPort1));
    
    mBus.registerWriteIO(0x98, MakeDelegate(this, &V9938::writePort0));
    mBus.registerWriteIO(0x99, MakeDelegate(this, &V9938::writePort1));
    mBus.registerWriteIO(0x9a, MakeDelegate(this, &V9938::writePort2));
    mBus.registerWriteIO(0x9b, MakeDelegate(this, &V9938::writePort3));
}

void V9938::detachIO()
{

}

byte V9938::readPort0(word /*port*/)
{
    //DBERR("readPort0\n");
    port1DataLatched = false;
	byte val = vramLatch;
	vramLatch = vram[incrementVramPointer()];
	return val;
}

byte V9938::readPort1(word /*port*/)
{
    DBERR("readPort1\n");
	return 0xff;
}
    
void V9938::writePort0(word port, byte value)
{
    //DBERR("WritePort0: 0x%02x [%c]\n", value, value);
	if ((value > 31) && (value < 128))
	{
		DBERR("VRAM [%c]\n", value);
	}
	port1DataLatched = false;
	vram[incrementVramPointer()] = value;
	vramLatch = value;
}

void V9938::writePort1(word port, byte value)
{
    DBERR("WritePort1: 0x%02x [%c]\n", value, value);
	if (port1DataLatched)
	{
		port1DataLatched = false;
		if (value & 0x80)
		{
			if (value & 0x40) 
			{
				// TODO: check what happens (SNOW26 demo and space manbow/undeadline?)
				NW_ASSERT(false);
			}
			else
			{
				//writeRegister(value & 0x3f, dataLatch);
				DBERR("writeRegister %d [0x%02x]\n", value & 0x3f, dataLatch);
			}
		}
		else
		{
			vramPointer = ((value & 0x3f) << 8) | dataLatch;
			if (!(value & 0x40)) vramLatch = vram[incrementVramPointer()];
		}

	}
	else
	{
		port1DataLatched = true;
		dataLatch = value;
	}
}

void V9938::writePort2(word port, byte value)
{
    DBERR("WritePort2: 0x%02x [%c]\n", value, value);
}

void V9938::writePort3(word port, byte value)
{
    DBERR("WritePort3: 0x%02x [%c]\n", value, value);
}

Uint32 V9938::incrementVramPointer()
{
	// Todo: check what really happens with reg14
	// Is it only incremented in MSX2 screens? Or is it always incremented, but are the high order bits masked in MSX1 modes?

	vramPointer++;
	vramPointer &= 0x3fff; // Todo: in MSX2 128 kB should be addressable
	return vramPointer;
}
