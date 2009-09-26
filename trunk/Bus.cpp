#include "Bus.h"
#include "Scheduler.h"
#include "Component.h"
#include "IODevice.h"

using namespace nowind;

Bus::Bus(Scheduler& aScheduler) :  
mScheduler(aScheduler)
{

}

Bus::~Bus()
{

}

void Bus::registerReadIO(Uint8 port, IOReadDelegate aDelegate)
{
    mIORead[port] = aDelegate;
}

void Bus::registerWriteIO(Uint8 port, IOWriteDelegate aDelegate)
{
    mIOWrite[port] = aDelegate;
}

byte Bus::readIO(Uint8 port)
{
    return 0xff; //mIORead[port]();
}

void Bus::writeIO(Uint8 port, byte value)
{
    //mIOWrite[port](value);
}

void Bus::addIODevice(IODevice * aIODevice)
{
    aIODevice->attachIO();
}
