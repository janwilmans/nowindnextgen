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


void SlotSelector::addMemoryDeviceToSlot(MemoryDevice* aMemoryDevice, Uint8 slot, Uint8 subslot)
{
    // implementation choise: just one MemoryDevice in one slot/subslot
    for (Uint8 section=0; section<constSections; section++)
    {
        slotLayout[slot][subslot][section] = aMemoryDevice;
    }
}

void SlotSelector::addMemoryDevice(MemoryDevice* aMemoryDevice, Uint8 slot, Uint8 subslot)
{
    if (subslot > 0) mSlotExpanded[slot] = true;

    if (mSlotExpanded[slot] != 0)
    {
        addMemoryDeviceToSlot(aMemoryDevice, slot, subslot);
    }
    else
    {
        for (Uint8 i=0; i<4; i++)
        {
            addMemoryDeviceToSlot(aMemoryDevice, slot, i);
        }
    }
}

void SlotSelector::setPage(Uint8 page, Uint8 slot, Uint8 subslot)
{
    Uint8 section0 = page*2;
    Uint8 section1 = section0+1;
    
    slotLayout[slot][subslot][section0]->activate(section0);
    slotLayout[slot][subslot][section1]->activate(section1);
}
