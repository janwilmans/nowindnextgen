#include "Event.h"
#include <string>

using namespace std;

Event::Event(emuTimeType aTime, EventDelegate aDelegate)
{
    mTime = aTime;
    mDelegate = aDelegate;
    mHigh = (mTime > upperBound);
}

Event::~Event(void)
{
}

void Event::Callback(emuTimeType aTime)
{
    mDelegate(aTime);
}

string Event::ToString()
{
    char temp[250];
    sprintf(temp, "itime: %u", mTime);
    return string(temp);
}
