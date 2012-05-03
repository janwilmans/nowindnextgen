//! Component.h
#ifndef COMPONENT_H
#define COMPONENT_H

#include "basetypes.h"

namespace nowind {

class Scheduler;
class Emulator;

// a Component is an emulated object that exists in the real world
// it has a name and can provide a descriptive 'state-string'
// these are probably the objects that are going to need 'persistancy' for
// creating snapshots. 
class Component 
{
public:
    Component(Emulator& aEmulator);

    // the prepare method should create all objects and register them with the broker
    virtual void prepare() = 0;

    // the initialize method should create relations to other objects 
    // it can assume that 'prepare' has been called on all existing objects in the broker.
    virtual void initialize() = 0;

    // should do any actions needed before the destructor 
    // can be savely called, such as: unregister objects from the broker,
    // stop threads...etc.
    virtual void prepare_shutdown() = 0;

    // the destructor should release any allocated resources (memory/filehandles etc.) during runtime 
    virtual ~Component() {}
    
protected:
    Emulator& mEmulator;
    Scheduler& mScheduler;
};

} // namespace nowind

#endif //COMPONENT_H
