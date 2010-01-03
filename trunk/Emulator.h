//! Emulator.h
#ifndef EMULATOR_H
#define EMULATOR_H

#include <list>
#include "Event.h"
#include "Component.h"

namespace nowind {

class Bus;
class Scheduler;
class SlotSelector;

class Emulator
{
public:
    Emulator(void);
    virtual ~Emulator(void);
    static Emulator* Instance();
    static emuTimeType emuTime;

    void initialize();
    void test();

    //test methods
    void interruptTestMethod(emuTimeType emuTime, emuTimeType eventTime);

    Bus& getBus() { return *mBus; }
    Scheduler& getScheduler() { return *mScheduler; }
    SlotSelector& getSlotSelector() { return *mSlotSelector; }   //todo: remove

private:
    Bus* mBus;
    Scheduler* mScheduler;
    SlotSelector* mSlotSelector;    //todo: remove
};

} // namespace nowind

#endif // EMULATOR_H