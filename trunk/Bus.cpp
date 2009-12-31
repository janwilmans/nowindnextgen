#include "Bus.h"
#include "Scheduler.h"
#include "BusComponent.h"

using namespace nowind;
using namespace fastdelegate;

Bus::Bus(Emulator& aEmulator) :  
mEmulator(aEmulator),
mScheduler(aEmulator.getScheduler())
{
}

Bus::~Bus()
{

}

void Bus::prepare()
{
	mNullComponent = new NullComponent(mEmulator);
    for (Uint16 port=0; port<255; port++)
    {
        registerReadIO(port, MakeDelegate(mNullComponent, &NullComponent::readIO));
        registerWriteIO(port, MakeDelegate(mNullComponent, &NullComponent::writeIO));
    }

    for (Uint8 section=0; section<constSections; section++)
    {
        mMemoryMappedIOSection[section] = true;
    }    
}

void Bus::initialize()
{
	for (int i=0;i<8;i++)
	{
	    deactivateMemReadSection(i);
	}
}

void Bus::prepare_shutdown()
{
	delete mNullComponent;
}

void Bus::registerReadIO(Uint16 port, IOReadDelegate aDelegate)
{
    mIORead[port] = aDelegate;
}

void Bus::registerWriteIO(Uint16 port, IOWriteDelegate aDelegate)
{
    mIOWrite[port] = aDelegate;
}

byte Bus::readIO(word port)
{
    return mIORead[port](port);
}

void Bus::writeIO(word port, byte value)
{
    mIOWrite[port](port, value);
}

void Bus::addIODevice(BusComponent* aIODevice)
{
    //todo: add device to a list-of-devices?
    aIODevice->attachIO();
}

void Bus::activateMemReadSection(Uint8 section, MemReadDelegate aDelegate)
{
	mMemoryMappedIOSection[section] = true;
	mReadSection[section] = aDelegate;
}

void Bus::activateMemWriteSection(Uint8 section, MemWriteDelegate aDelegate)
{
	mWriteSection[section] = aDelegate;
}

void Bus::deactivateMemReadSection(Uint8 section)
{
    mMemoryMappedIOSection[section] = true;
    mReadSection[section] = MakeDelegate(mNullComponent, &NullComponent::readByte);
}

void Bus::deactivateMemWriteSection(Uint8 section)
{
    mWriteSection[section] = MakeDelegate(mNullComponent, &NullComponent::writeByte);
}

void Bus::activateSSSRRead(SSSRReadDelegate aDelegate)
{
    mSSSRReadDel = aDelegate;
}

void Bus::activateSSSRWrite(SSSRWriteDelegate aDelegate)
{
    mSSSRWriteDel = aDelegate;
}

void Bus::setReadSectionMemory(Uint8 section, byte* memory)
{
	//mMemoryMappedIOSection[section] = false;              //bug:  mMemoryMappedIOSection = false works with zexall, but not for the mainrom?
    mReadSectionMemory[section] = memory;
}
