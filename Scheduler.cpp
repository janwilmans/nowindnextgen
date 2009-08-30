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

void Scheduler::runUsing(ICPU *aCpu)
{
    emuTimeType emuTime = 0;
    EventIterator possibleNextEvent = mEventList.end();
    bool lEndOfRangeEvent = true;
    if (mEventList.begin() != mEventList.end())
    {
        // the event list is not empty
        for (EventIterator i=mEventList.begin();i != mEventList.end();i++)
        {
            Event lEvent = *i;
            emuTimeType eventEmuTime = lEvent.GetTime();
            if (emuTime > upperBound && eventEmuTime < lowerBound)
            {
                // exceptional case where even though eventEmuTime < emuTime,
                // it should not be executed. (emutime is nearing the end of its range)
                continue;
            }
            if (eventEmuTime >= emuTime) 
            {
                if (possibleNextEvent == mEventList.end()) 
                {
                    possibleNextEvent = i;
                }
                else
                {
                    
                }
                // we have found an event we should execute, but it might not be the first / oldest

            }
            //if (lEvent.GetTime() >

        }
    }
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
            bool lResult = getNextEvent(emuTime, nextEvent, nextEventTime);
            //if (nextEvent.isEndOfRange()) 
  
            NW_ASSERT(lResult);
        }
        i++;
        emuTime++;
    }
}
