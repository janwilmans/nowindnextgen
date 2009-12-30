//! RomMemory.h
#ifndef RomMemory_H
#define RomMemory_H

#include "Component.h"
#include "BusComponent.h"
#include <string>

namespace nowind {

class RomMemory : public BusComponent 
{
private:
    byte* mMemory;
	std::string mFilename;
	std::string mID;

    void loadRom(std::string);
    byte readByte(word address);

public:
	RomMemory(Emulator& aEmulator, std::string filename);

    // Component methods
    virtual void prepare();
    virtual void initialize();
    virtual void prepare_shutdown();

    // BusComponent methods
    virtual void activate(Uint8 section);

    // the destructor should release any allocated resources (memory/filehandles etc.) during runtime 
    virtual ~RomMemory();

};

} // namespace nowind

#endif // RomMemory_H