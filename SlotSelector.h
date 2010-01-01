//! SlotSelector.h
#ifndef SLOTSELECTOR_H
#define SLOTSELECTOR_H

#include "BusComponent.h"

namespace nowind {

class Bus;
class NullComponent;

class SlotSelector : public BusComponent
{
public:
    SlotSelector(Emulator& aEmulator);

    // IO methods
    virtual void attachIO();
    virtual void detachIO();

    // Component methods    
    virtual void prepare();
    virtual void initialize();
    virtual void prepare_shutdown();

    // the destructor should release any allocated resources (memory/filehandles etc.) during runtime 
    virtual ~SlotSelector();

    // proposal: get/set methods allowed in the header if they are purely get/setters
    bool getSlotExpanded(Uint8 slot) { return mSlotExpanded[slot]; }
    void setSlotExpanded(Uint8 slot, bool expanded) { mSlotExpanded[slot] = expanded; }

    // add a memory-mapped device
    void addBusComponent(BusComponent*, Uint8 slot, Uint8 subslot);

    byte readSSSR();
    void writeSSSR(byte);

    byte readIO(word port);
    void writeIO(word port, byte value);

    void activatePage(Uint8 page);
    
    Uint8 getActivateSubslots(Uint8 slot);

private:
    void activatePage(Uint8 page, Uint8 slot, Uint8 subslot);
    void activateCurrent();
    void addBusComponentToSlot(BusComponent* aBusComponent, Uint8 slot, Uint8 subslot);

    // contains the currently active mainslot and slotsub for each page
    Uint8 mSelectedMainSlot[4];
    Uint8 mSelectedSubSlot[4];

    // whether a slot is expanded or not
    bool mSlotExpanded[4];

    BusComponent* slotLayout[4][4][constSections]; 
    byte mA8Value;
    byte mSSSR[4];
    
};

} // namespace nowind

#endif // SLOTSELECTOR_H