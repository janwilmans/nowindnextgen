#include "SlotSelector.h"
#include "Bus.h"

using namespace nowind;

SlotSelector::SlotSelector(Bus& aBus) : IODevice(aBus), MemoryDevice(aBus)
{

}

SlotSelector::~SlotSelector()
{

}

void SlotSelector::prepare()
{

}
void SlotSelector::initialize()
{
    // initialize all slots, expanded or not with the 'VoidDevice'
}

void SlotSelector::prepare_shutdown()
{

}

byte SlotSelector::readSSSR()
{
    return 0xff;
}

void SlotSelector::writeSSSR(byte)
{
   
}

void SlotSelector::addMemoryDevice(MemoryDevice*, Uint8 slot, Uint8 subslot)
{
    //if (subslot) 
}

