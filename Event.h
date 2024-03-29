//! Event.h
#ifndef EVENT_H
#define EVENT_H

#include "basetypes.h"
#include "FastDelegate.h"
#include <string>

namespace nowind {

static const Uint32 emuTimeSize = sizeof(emuTimeType);
static const Uint32 emuHalfTime = 2^((8*emuTimeSize)/2);
static const Uint32 lowerBound = emuHalfTime/2;
static const Uint32 upperBound = emuHalfTime+lowerBound;

typedef fastdelegate::FastDelegate2<emuTimeType, emuTimeType> EventDelegate;   // returns void 

class Event
{
public:
    Event();
    Event(emuTimeType aTime, EventDelegate aDelegate);
    virtual ~Event(void);

    void Callback(emuTimeType emuTime, emuTimeType eventTime);
    std::string ToString();

    emuTimeType GetTime();

    // assignment operator
    Event& operator= (const Event& other);

    // < operator
    int operator< (const Event& other);
protected:
    emuTimeType mTime;
    bool mHigh;

    // exposed callback to allow bind()
    EventDelegate mDelegate;
};

} // namespace nowind 

#endif // EVENT_H
