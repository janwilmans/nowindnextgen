//! NullComponent.h
#ifndef NullComponent_H
#define NullComponent_H


#include "BusComponent.h"

namespace nowind {

class NullComponent : public BusComponent //, public AudioDevice
{
public:
    NullComponent(Emulator& aEmulator);

    // Component
    virtual void prepare();
    virtual void initialize();
    virtual void prepare_shutdown();
    
    // the destructor should release any allocated resources (memory/filehandles etc.) during runtime 
    virtual ~NullComponent();

    byte readIO(word port);
    void writeIO(word port, byte value);

    byte readByte(word address);
    void writeByte(word address, byte value);
    
    void activate(Uint8 section);

};

} // namespace nowind

#endif // NullComponent_H