#include "Interrupt.h"
#include <string>

using namespace std;

Interrupt::Interrupt(emuTimeType aTime)
{
    mTime = aTime;
}

bool Interrupt::IsExpired(emuTimeType aTime)
{
    return mTime <= aTime;
}

Interrupt::~Interrupt(void)
{
}

string Interrupt::ToString()
{
    char temp[250];
    sprintf(temp, "itime: %u", mTime);
    return string(temp);
}
