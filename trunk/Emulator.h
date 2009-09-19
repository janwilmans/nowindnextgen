//! Emulator.h
#ifndef EMULATOR_H
#define EMULATOR_H

#include <list>
#include "Event.h"
#include "Component.h"

typedef std::list<Event> EventList;

namespace nowind {

//forward declarations
class Scheduler;

class Emulator //: public Component
{
    public:
    Emulator(void);
    virtual ~Emulator(void);
    static Emulator* Instance();
    static emuTimeType emuTime;

    void initialize();
    void test();
    void endOfRangeEvent(emuTimeType emuTime, emuTimeType eventTime);

    //test methods
    void interruptTestMethod(emuTimeType emuTime, emuTimeType eventTime);
    private:
    Scheduler* mScheduler;
    EventList mEventList;

};

} // namespace nowind

#endif // EMULATOR_H