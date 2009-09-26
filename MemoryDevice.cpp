#include "MemoryDevice.h"
#include "SlotSelector.h"

using namespace nowind;

MemoryDevice::MemoryDevice(Bus& bus) : mBus(bus)
{

}

void MemoryDevice::initialize()
{

}

void MemoryDevice::setSlot(SlotSelector* slotSelector, Uint8 slot, Uint8 subslot)
{   
    mSlotSelector = slotSelector;
    mSlot = slot;
    mSubSlot = subslot;
}

bool MemoryDevice::inExpandedSlot()
{
    return mSlotSelector->getSlotExpanded(mSlot);
}
