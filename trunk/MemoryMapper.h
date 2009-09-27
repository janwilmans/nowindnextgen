//! MemoryMapper.h
#ifndef MemoryMapper_H
#define MemoryMapper_H

#include "Component.h"
#include "IODevice.h"
#include "MemoryDevice.h"

namespace nowind {

class MemoryMapper : public IODevice, public MemoryDevice 
{
private:
    Uint8 mBanks;
    byte* mMemory;
    byte mSelectedBank[4];

public:
    MemoryMapper(Bus& bus, Uint16 kilobytes);

    // Component methods
    virtual void prepare();
    virtual void initialize();
    virtual void prepare_shutdown();

    // IODevice methods
    virtual void attachIO();
    virtual void detachIO();

    // MemoryDevice methods
    virtual void activate(Uint8 section);

    // the destructor should release any allocated resources (memory/filehandles etc.) during runtime 
    virtual ~MemoryMapper();

    byte readIO(word port);
    void writeIO(word port, byte value);
    
    byte readByte(word address);
    void writeByte(word address, byte value);

};

} // namespace nowind

#endif // MemoryMapper_H