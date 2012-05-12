//! BusComponent.h
#ifndef BUSCOMPONENT_H
#define BUSCOMPONENT_H

#include "Component.h"

namespace nowind {

class Bus;
class SlotSelector;

// an abstract class that defines methods to interact with the SlotSelector to provide memory-mapped-I/O
class BusComponent : public Component 
{
protected:
    Bus& mBus;
    Uint8 mSlot;
    Uint8 mSubSlot;
    SlotSelector* mSlotSelector;

    // used to update the Bus for this section's BusComponent, called by activateSection
    virtual void activate(Uint8 section) {}

public:
    BusComponent(Emulator& aEmulator);

    void activateSection(Uint8 section);
    
    void setSlot(SlotSelector* slotSelector, Uint8 slot, Uint8 subslot);
    bool inExpandedSlot();

    // release any allocated resources (memory/filehandles etc.) during runtime 
    virtual ~BusComponent() {}

    // inherited from Component but not implemented

    // the prepare method should create all objects and register them with the broker
    virtual void prepare() = 0;

    // the initialize method should create relations to other objects 
    // it can assume that 'prepare' has been called on all existing objects in the broker.
    virtual void initialize();

    // should do any actions needed before the destructor 
    // can be savely called, such as: unregister objects from the broker,
    // stop threads...etc.
    virtual void prepare_shutdown() = 0;

    // IO methods

    // used to attach this component to the Bus
	virtual void attachIO() {}

    // used to detach this component from the Bus
	virtual void detachIO() {}
	
	// Memory methods
	// address 0xffff in an expanded slot must always support memory-mapper IO to allow the SlotSelector to switch subslots, 
	// therefore memory-components must implement readByte/writeByte and can call activateReadSectionMemory in their 
	// activate() method to allow accelerated reads for all but the last section, even in expanded slots
	void activateReadSectionMemory(Uint8 section, byte* memory);

};

} // namespace nowind

#endif // BUSCOMPONENT_H
