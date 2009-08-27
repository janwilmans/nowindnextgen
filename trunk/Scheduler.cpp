#include "Scheduler.h"
#include "Debug.h"
#include <stdio.h>

static Uint32 Ranges = 4;

Scheduler::Scheduler()
{
}

Scheduler::~Scheduler(void)
{
}

void Scheduler::addEvent(emuTimeType aIntTime, EventDelegate aCallback)
{
    mEventList.push_back(Event(aIntTime, aCallback));
}

bool Scheduler::getNextEvent(emuTimeType aTime, Event& nextEvent, emuTimeType& nextTime)
{
    if (mEventList.begin() == mEventList.end())
    {
        return false;
    }

    nextEvent = *mEventList.begin();
    nextTime = nextEvent.GetTime();
    mEventList.pop_front();    
    return true;
}

void Scheduler::testrun(emuTimeType startTime, Uint32 aTimes)
{
    emuTimeType emuTime = startTime;

    Event nextEvent;
    emuTimeType nextEventTime = 0;
    bool lResult = getNextEvent(startTime, nextEvent, nextEventTime);
    NW_ASSERT(lResult);

    Uint32 i = 0;
    while (i < aTimes)
    {
        printf("Emutime: %u\n", emuTime);
        while (nextEventTime >= emuTime)
        {
            printf("lEvent: %s\n", nextEvent.ToString().c_str());
            nextEvent.Callback(i);
            bool lResult = getNextEvent(startTime, nextEvent, nextEventTime);
            NW_ASSERT(lResult);
        }
        i++;
        emuTime++;
    }
}
