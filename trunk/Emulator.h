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
	static emuTimeType emuTime;

    void initialize();
    void endOfRangeEvent(emuTimeType emuTime, emuTimeType eventTime);

    //test methods
    void interruptTestMethod(emuTimeType emuTime, emuTimeType eventTime);
private:
    Scheduler* mScheduler;
    EventList mEventList;

};
