#include "BusComponent.h"
#include "Bus.h"
#include "Emulator.h"
#include "SlotSelector.h"

using namespace nowind;

BusComponent::BusComponent(Emulator& aEmulator) : Component(aEmulator), 
mBus(aEmulator.getBus())
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

void BusComponent::activateSection(Uint8 section)
{
    mBus.activateSection(section, mSlot, mSubSlot, inExpandedSlot());
    activate(section);
}

void BusComponent::activateReadSectionMemory(Uint8 section, byte* memory)
{
    if (section == constMaxSection && inExpandedSlot())
    {
        // do not allow activateReadSectionMemory, because it would disable read/writes to the subslot-selection-register (0xffff)
        return;
    }
    mBus.activateReadSectionMemory(section, memory);
}
