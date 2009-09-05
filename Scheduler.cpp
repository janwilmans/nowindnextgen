#include "Scheduler.h"
#include "Debug.h"
#include <stdio.h>

static Uint32 Ranges = 4;

using namespace fastdelegate;

Scheduler::Scheduler()
{
}

Scheduler::~Scheduler(void)
{
}

void Scheduler::addEvent(emuTimeType aEventTime, EventDelegate aCallback)
{
	// events can (and should) only be scheduled in the future!
	NW_ASSERT (aEventTime > Emulator::emuTime);

    mEventList.push_back(Event(aEventTime, aCallback));
	mEventList.sort();
}

void Scheduler::endOfRange(emuTimeType emuTime, emuTimeType aEventTime)
{
    // end of range, at execly the last number in the range?
    if (emuTime == aEventTime)
    {
        Emulator::emuTime = 0;        
    }
}

void Scheduler::run(ICPU *aCpu)
{
    emuTimeType& emuTime = Emulator::emuTime;
    Event endOfRangeEvent(-1, MakeDelegate(this, &Scheduler::endOfRange));
    Event* currentEvent = 0;
	emuTimeType nextEventTime = 0;
	EventIterator current = mEventList.begin();
	for (;;)  // for ever
	{
        if (currentEvent != 0) // is there an active event right now?
        {
            currentEvent->Callback(Emulator::emuTime, currentEvent->GetTime());
            currentEvent = 0; // event callback done.
            if (current != mEventList.end()) 
            {
                mEventList.erase(current);
            }
        }

        // by default, schedule the end-of-range event
        nextEventTime = endOfRangeEvent.GetTime();
        currentEvent = &endOfRangeEvent;
        current = mEventList.end();

        for (EventIterator i=mEventList.begin();i != mEventList.end(); i++)
        {
            emuTimeType eventEmuTime = i->GetTime();
            if (emuTime > upperBound && eventEmuTime < lowerBound)
            {
                // exceptional case where even though eventEmuTime < emuTime,
                // it should not be executed. (emutime is nearing the end of its range)
                continue;
            }
            if (eventEmuTime >= emuTime)
            {
                // found an expired event
                if (eventEmuTime < nextEventTime) 
                {
                    nextEventTime = eventEmuTime;
                    currentEvent = &(*i);
                    current = i;
                }

            }
        }
		Emulator::emuTime = aCpu->ExecuteInstructionsUntil(nextEventTime);
	}
}

void Scheduler::runNice(ICPU *aCpu)
{
	emuTimeType nextEventTime = 0;
	EventIterator previousEvent = mEventList.begin();
	for (;;)  // for ever
	{
		// determine current event
		EventIterator currentEvent = previousEvent;
		currentEvent++;

		// execute event
		currentEvent->Callback(Emulator::emuTime, currentEvent->GetTime());

		// delete previous event
		mEventList.erase(previousEvent);

		previousEvent = currentEvent;
		
		// deteremine next event
		EventIterator nextEvent = currentEvent;
		nextEvent++;
		if (nextEvent == mEventList.end()) {
			nextEvent = mEventList.begin();
		}

		nextEventTime = nextEvent->GetTime();
		Emulator::emuTime = aCpu->ExecuteInstructionsUntil(nextEventTime);
		
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
            nextEvent.Callback(i,i);
            bool lResult = getNextEvent(emuTime, nextEvent, nextEventTime);
            //if (nextEvent.isEndOfRange()) 
  
            NW_ASSERT(lResult);
        }
        i++;
        emuTime++;
    }
}
