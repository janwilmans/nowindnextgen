//! V9938.h
#ifndef V9938_H
#define V9938_H

#include "BusComponent.h"

namespace nowind {

class V9938 : public BusComponent 
{
public:
    V9938(Bus& bus);

    // the prepare method should create all objects and register them with the broker
    virtual void prepare();

    // the initialize method should create relations to other objects 
    // it can assume that 'prepare' has been called on all existing objects in the broker.
    virtual void initialize();

    // should do any actions needed before the destructor 
    // can be savely called, such as: unregister objects from the broker,
    // stop threads...etc.
    virtual void prepare_shutdown();

    // IO methods
    virtual void attachIO();
    virtual void detachIO();

    // the destructor should release any allocated resources (memory/filehandles etc.) during runtime 
    virtual ~V9938();

private:
    byte readPort0(word port);
    byte readPort1(word port);
    
    void writePort0(word port, byte value);
    void writePort1(word port, byte value);
    void writePort2(word port, byte value);
    void writePort3(word port, byte value);

};

} // namespace nowind

#endif // V9938_H