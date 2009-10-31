#include "BusComponent.h"
#include "SlotSelector.h"

using namespace nowind;

BusComponent::BusComponent(Bus& bus) : mBus(bus)
{

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
