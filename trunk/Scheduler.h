#pragma once

#include "msxtypes.h"
#include "Event.h"
#include <list>

typedef std::list<Event>::iterator EventIterator;

class Scheduler
{
public:
    Scheduler();
    virtual ~Scheduler(void);

    void addEvent(emuTimeType aIntTime, EventDelegate aCallback);
    void run();
protected:
    emuTimeType mTime;
    Uint32 mRange;

    std::list<Event> mEventList;

};
