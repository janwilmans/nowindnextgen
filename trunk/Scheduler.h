#pragma once

#include "basetypes.h"
#include "Event.h"
#include "cpu/Z80.h"
#include <list>

typedef std::list<Event>::iterator EventIterator;

class Scheduler
{
    public:
    Scheduler();
    virtual ~Scheduler(void);

    void initialize();

    void addEvent(emuTimeType aIntTime, EventDelegate aCallback);
    bool getNextEvent(emuTimeType, Event&, emuTimeType&);
    void testrun(emuTimeType, Uint32);
    void endOfRange(emuTimeType emuTime, emuTimeType aEventTime);
    void run(CPU*);
    void runNice(CPU*);
    protected:
    emuTimeType mTime;
    Uint32 mRange;

    std::list<Event> mEventList;

};
