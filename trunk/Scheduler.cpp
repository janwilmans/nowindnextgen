#include "Scheduler.h"
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

void Scheduler::run()
{
    int t=0;
    for (EventIterator i = mEventList.begin();i != mEventList.end(); i++)
    {
        t++;
        Event& lEvent = *i;
        printf("lEvent: %s\n", lEvent.ToString().c_str());
        lEvent.Callback(t);
    }
}

/*
// moved to Schedule to prevent duplicated calculations
bool Event::IsExpired(emuTimeType aEmuTime)
{
    if (mHigh && (aEmuTime < lowerBound)) return true;
    if (mTime < aEmuTime) return false;
    return true;
}
*/
