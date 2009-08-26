#pragma once

#include "msxtypes.h"
#include "FastDelegate.h"
#include <string>

static const Uint32 emuTimeSize = sizeof(emuTimeType);
static const Uint32 emuHalfTime = 2^((8*emuTimeSize)/2);
static const Uint32 lowerBound = emuHalfTime/2;
static const Uint32 upperBound = emuHalfTime+lowerBound;

typedef fastdelegate::FastDelegate1<emuTimeType> EventDelegate;   // returns void 

class Event
{
public:
    Event(emuTimeType aTime, EventDelegate aDelegate);
    virtual ~Event(void);

    void Callback(emuTimeType aTime);
    std::string ToString();

protected:
    emuTimeType mTime;
    bool mHigh;

    // exposed callback to allow bind()
    EventDelegate mDelegate;
};
