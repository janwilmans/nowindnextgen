#pragma once

#include <list>
#include "Event.h"

typedef std::list<Event> EventList;

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
    EventList mEventList;

};
