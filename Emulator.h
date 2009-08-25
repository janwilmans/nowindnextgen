#pragma once

#include <list>
#include "Interrupt.h"

typedef std::list<Interrupt> InterruptList;

//forward declarations
class Scheduler;

class Emulator
{
public:
    Emulator(void);
    virtual ~Emulator(void);
    static Emulator* Instance();

    void initialize();

    //test methods
    void interruptTestMethod(emuTimeType aTime);
private:
    Scheduler* mScheduler;
    InterruptList mInterruptList;

};
