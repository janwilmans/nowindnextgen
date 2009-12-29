#include "V9938.h"
#include "debug.h"
#include "Bus.h"

using namespace nowind;
using namespace fastdelegate;

V9938::V9938(Bus& bus) : BusComponent(bus)
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

byte V9938::readPort0(word port)
{
    DBERR("readPort0\n");
    return 0xff;
}

byte V9938::readPort1(word port)
{
    DBERR("readPort1\n");
    return 0xff;
}
    
void V9938::writePort0(word port, byte value)
{
    DBERR("WritePort0: 0x%02x [%c]\n", value, value);
}

void V9938::writePort1(word port, byte value)
{
    DBERR("WritePort1: 0x%02x [%c]\n", value, value);
}

void V9938::writePort2(word port, byte value)
{
    DBERR("WritePort2: 0x%02x [%c]\n", value, value);
}

void V9938::writePort3(word port, byte value)
{
    DBERR("WritePort3: 0x%02x [%c]\n", value, value);
}

