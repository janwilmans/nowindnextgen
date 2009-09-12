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

void Scheduler::initialize(void)
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
    // no longer needed?
}

void Scheduler::run(CPU *aCpu)
{
    emuTimeType& emuTime = Emulator::emuTime;
    for (;;)  // for ever
    {
        // default to (emuTime + x) where x is a 'while' into the future,
        // todo: verify if 2^31-1 is a good default
        emuTimeType nextEventTime = emuTime + 200000;
        Sint32 diff = 0;
        for (EventIterator i = mEventList.begin();i != mEventList.end(); i++)
        {
            emuTimeType eventEmuTime = i->GetTime();
            diff = eventEmuTime - emuTime;
            DBERR(" eventEmuTime: %u (%i), diff: %i\n", eventEmuTime, eventEmuTime, diff);

            if (diff > 0)
            {
                // found an upcoming event
                Sint32 diff2 = nextEventTime - eventEmuTime;
                if (diff2 > 0)
                {
                    // event closer then current next-event
                    nextEventTime = eventEmuTime;
                    DBERR("possible next event: %u continue...\n", nextEventTime);
                    continue;
                }
                continue;
            }
            // -2000 is arbitrary, it must be at least -(x+1) where x is the maximum
            // instruction size in tstates / emutime
            if (diff > -2000)
            {
                // found expired event
                DBERR("expired event found: %u, execute!\n", emuTime);

                i->Callback(emuTime, i->GetTime());
                i = mEventList.erase(i);
            }
        }
        //DBERR("execute from %u (%i) until: %u (%i)\n", emuTime, emuTime, nextEventTime, nextEventTime);
        emuTime = aCpu->ExecuteInstructionsUntil(emuTime, nextEventTime);
    }
}

void Scheduler::runNice(CPU *aCpu)
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
        if (nextEvent == mEventList.end())
        {
            nextEvent = mEventList.begin();
        }

        nextEventTime = nextEvent->GetTime();
        Emulator::emuTime = aCpu->ExecuteInstructionsUntil(Emulator::emuTime, nextEventTime);

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
        DBERR("Emutime: %u\n", emuTime);
        while (nextEventTime >= emuTime)
        {
            DBERR("lEvent: %s\n", nextEvent.ToString().c_str());
            nextEvent.Callback(i, i);
            bool lResult = getNextEvent(emuTime, nextEvent, nextEventTime);
            //if (nextEvent.isEndOfRange())

            NW_ASSERT(lResult);
        }
        i++;
        emuTime++;
    }
}
