//! BusComponent.h
#ifndef BUSCOMPONENT_H
#define BUSCOMPONENT_H

#include "Component.h"
#include "Bus.h"

namespace nowind {

// an abstract class that defines methods to allow Components to attach to a Bus
class BusComponent : public Component 
{
protected:
    Bus& mBus;
public:
    BusComponent(Bus& bus);

    // used to attach this component to the Bus
    virtual void attach() = 0;

    // the initialize method should create relations to other objects 
    // it can assume that 'prepare' has been called on all existing objects in the broker.
    virtual void detach() = 0;

    // the destructor should release any allocated resources (memory/filehandles etc.) during runtime 
    virtual ~BusComponent() {}


    // inherited from Component but not implemented


    // the prepare method should create all objects and register them with the broker
    virtual void prepare() = 0;

    // the initialize method should create relations to other objects 
    // it can assume that 'prepare' has been called on all existing objects in the broker.
    virtual void initialize() = 0;

    // should do any actions needed before the destructor 
    // can be savely called, such as: unregister objects from the broker,
    // stop threads...etc.
    virtual void prepare_shutdown() = 0;

};

} // namespace nowind

#endif // BUSCOMPONENT_H