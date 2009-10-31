//! Scheduler.h
#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "basetypes.h"
#include "Event.h"
#include <list>

namespace nowind {

class CPU;

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

} // namespace nowind

#endif // SCHEDULER_H