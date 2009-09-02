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
	// events can only be scheduled in the future!
	NW_ASSERT (aIntTime > Emulator::emuTime);

    mEventList.push_back(Event(aIntTime, aCallback));
	mEventList.sort();
}

void Scheduler::runUsing(ICPU *aCpu)
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
