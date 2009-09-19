//! Mapper.h
#ifndef MAPPER_H
#define MAPPER_H

#include "BusComponent.h"

namespace nowind {

class Mapper : public BusComponent
{
private:
    Uint8 mBanks;
public:
    Mapper(Bus& bus, Uint8 banks);

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
    virtual ~Mapper();
};

} // namespace nowind

#endif // MAPPER_H