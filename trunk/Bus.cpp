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
    for (Uint16 port=0; port<256; port++)
    {
        registerReadIO(port, MakeDelegate(mNullComponent, &NullComponent::readIO));
        registerWriteIO(port, MakeDelegate(mNullComponent, &NullComponent::writeIO));
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
    aIODevice->attachIO();
}

void Bus::registerMemRead(Uint8 section, MemReadDelegate* aDelegate)
{
    mMemRead[section] = aDelegate;
}

void Bus::registerMemWrite(Uint8 section, MemWriteDelegate* aDelegate)
{
    mMemWrite[section] = aDelegate;
}

void Bus::activateMemReadSection(Uint8 section, MemReadDelegate aDelegate)
{
	//mMemoryMappedIOSection[section] = true;   // move mMemoryMappedIOSection into Bus
    *mMemRead[section] = aDelegate;
}

void Bus::activateMemWriteSection(Uint8 section, MemWriteDelegate aDelegate)
{
    *mMemWrite[section] = aDelegate;
}

void Bus::deactivateMemReadSection(Uint8 section)
{
    *mMemRead[section] = MakeDelegate(mNullComponent, &NullComponent::readByte);
}


void Bus::deactivateMemWriteSection(Uint8 section)
{
    *mMemWrite[section] = MakeDelegate(mNullComponent, &NullComponent::writeByte);
}


void Bus::registerSSSRRead(SSSRReadDelegate* aDelegate)
{
    mSSSRRead = aDelegate;
}

void Bus::registerSSSRWrite(SSSRWriteDelegate* aDelegate)
{
    mSSSRWrite = aDelegate;
}

void Bus::activateSSSRRead(SSSRReadDelegate aDelegate)
{
   (*mSSSRRead) = aDelegate;
}

void Bus::activateSSSRWrite(SSSRWriteDelegate aDelegate)
{
   (*mSSSRWrite) = aDelegate;
}

void Bus::registerReadSectionMemory(Uint8 section, byte** readSectionMemory)
{
    mReadSectionMemory[section] = readSectionMemory;
}

void Bus::setReadSectionMemory(Uint8 section, byte* memory)
{
	//*mMemRead[section] = 0;	// cause crash if called
	//mMemoryMappedIOSection[section] = false;
    *mReadSectionMemory[section] = memory;
}
