//! IODevice.h
#ifndef IODEVICE_H
#define IODEVICE_H

#include "Component.h"
#include "Bus.h"

namespace nowind {

// an abstract class that defines methods to allow Components to 
// be attached to a Bus to provide IO-bus-I/O
class IODevice : public Component
{
protected:
    Bus& mBus;
public:
    IODevice(Bus& bus);

    // used to attach this component to the Bus
    virtual void attachIO() = 0;

    // used to detach this component from the Bus
    virtual void detachIO() = 0;

    // release any allocated resources (memory/filehandles etc.) during runtime 
    virtual ~IODevice() {}

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

};

} // namespace nowind

#endif // IODEVICE_H