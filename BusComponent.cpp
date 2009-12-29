#include "BusComponent.h"
#include "SlotSelector.h"

using namespace nowind;

BusComponent::BusComponent(Bus& bus) : mBus(bus)
{
    mSlot = 0;
    mSubSlot = 0;
}

void BusComponent::initialize()
{

}

void BusComponent::setSlot(SlotSelector* slotSelector, Uint8 slot, Uint8 subslot)
{   
    mSlotSelector = slotSelector;
    mSlot = slot;
    mSubSlot = subslot;
}

bool BusComponent::inExpandedSlot()
{
    return mSlotSelector->getSlotExpanded(mSlot);
}
