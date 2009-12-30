#include "Event.h"
#include <string>

using namespace std;
using namespace nowind;

Event::Event()
{
    mTime = 0;
    mDelegate = 0;
}

Event::Event(emuTimeType aTime, EventDelegate aDelegate)
{
    mTime = aTime;
    mDelegate = aDelegate;
}

Event::~Event(void)
{
}

void Event::Callback(emuTimeType emuTime, emuTimeType eventTime)
{
    if (mDelegate != 0)
    {
        mDelegate(emuTime, eventTime);
    }
}

string Event::ToString()
{
    char temp[250];
    sprintf(temp, "itime: %u", mTime);
    return string(temp);
}

emuTimeType Event::GetTime()
{
    return mTime;
}

Event& Event::operator= (const Event & other)
{
    if (this != &other) // prevent self-assignment
    {
        mTime = other.mTime;
        mDelegate = other.mDelegate;
    }
    return *this;
}

int Event::operator< (const Event& other)
{
    return mTime < other.mTime;
}

