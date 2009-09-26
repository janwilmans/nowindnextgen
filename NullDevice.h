//! NullDevice.h
#ifndef NULLDEVICE_H
#define NULLDEVICE_H

#include "IODevice.h"
#include "MemoryDevice.h"

namespace nowind {

class NullDevice : public IODevice, public MemoryDevice //, public AudioDevice
{
public:
    NullDevice(Bus& bus);

    // Component
    virtual void prepare();
    virtual void initialize();
    virtual void prepare_shutdown();

    // IODevice
    virtual void attachIO();
    virtual void detachIO();
    
    // the destructor should release any allocated resources (memory/filehandles etc.) during runtime 
    virtual ~NullDevice();

    byte readIO(word port);
    void writeIO(word port, byte value);

    byte readByte(word address);
    void writeByte(word address, byte value);

};

} // namespace nowind

#endif // NULLDEVICE_H