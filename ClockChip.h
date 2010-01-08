//! ClockChip.h
#ifndef CLOCKCHIP_H
#define CLOCKCHIP_H

#include <time.h>
#include "BusComponent.h"

namespace nowind {

class ClockChip : public BusComponent
{
public:
    ClockChip(Emulator& aEmulator);

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
    virtual ~ClockChip();

private:
	byte readData(word port);
	void writeAddress(word port, byte value);
	void writeData(word port, byte value);

	byte mAddress;
	time_t mTimeDiff;

	byte mModeRegister;
	byte mTestRegister;
	byte mDataRegister[13][3];
};

} // namespace nowind

#endif // CLOCKCHIP_H