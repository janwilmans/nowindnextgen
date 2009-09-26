//! SlotSelector.h
#ifndef SLOTSELECTOR_H
#define SLOTSELECTOR_H

#include "IODevice.h"
#include "MemoryDevice.h"

namespace nowind {

class Bus;

class SlotSelector : public IODevice, public MemoryDevice
{
public:
    SlotSelector(Bus& bus);

    // the prepare method should create all objects and register them with the broker
    virtual void prepare();

    // the initialize method should create relations to other objects 
    // it can assume that 'prepare' has been called on all existing objects in the broker.
    virtual void initialize();

    // should do any actions needed before the destructor 
    // can be savely called, such as: unregister objects from the broker,
    // stop threads...etc.
    virtual void prepare_shutdown();

    // the destructor should release any allocated resources (memory/filehandles etc.) during runtime 
    virtual ~SlotSelector();

    // proposal: get/set methods allowed in the header if they are purely get/setters
    bool getSlotExpanded(Uint8 slot) { return mSlotExpanded[slot] != 0; }
    void setSlotExpanded(Uint8 slot, bool expanded) { mSlotExpanded[slot] = expanded; }

    // add a memory-mapped device
    void addMemoryDevice(MemoryDevice*, Uint8 slot, Uint8 subslot);

    byte readSSSR();
    void writeSSSR(byte);

private:

    // contains the currently active mainslot and slotsub for each page
    Uint8 mSelectedMainSlot[4];
    Uint8 mSelectedSubSlot[4];

    // whether a slot is expanded or not
    bool mSlotExpanded[4];

    BusComponent* slotLayout[4][4][constSections]; 
};

} // namespace nowind

#endif // SLOTSELECTOR_H