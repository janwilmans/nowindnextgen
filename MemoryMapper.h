//! MemoryMapper.h
#ifndef MemoryMapper_H
#define MemoryMapper_H

#include "BusComponent.h"

namespace nowind {

class MemoryMapper : public BusComponent 
{
private:
    Uint8 mBanks;
    byte* mMemory;
    byte mSelectedBank[4];

protected:
    // BusComponent methods
    virtual void activate(Uint8 section);

public:
    MemoryMapper(Emulator& aEmulator, Uint16 kilobytes);

    // Component methods
    virtual void prepare();
    virtual void initialize();
    virtual void prepare_shutdown();

    // IO methods
    virtual void attachIO();
    virtual void detachIO();

    // the destructor should release any allocated resources (memory/filehandles etc.) during runtime 
    virtual ~MemoryMapper();

    byte readIO(word port);
    void writeIO(word port, byte value);
    
    byte readByte(word address);
    void writeByte(word address, byte value);

};

} // namespace nowind

#endif // MemoryMapper_H