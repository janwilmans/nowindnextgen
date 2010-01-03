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

private:
    Bus* mBus;
    Scheduler* mScheduler;
};

} // namespace nowind

#endif // EMULATOR_H