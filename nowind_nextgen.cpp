// nowind_nextgen.cpp : Defines the entry point for the console application.
//
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <tchar.h>

#include "Event.h"
#include "Emulator.h"

void testEvent(const char * aName, Event& aEvent, emuTimeType aTime)
{
    bool lExpired = false; // aEvent.IsExpired(aTime);
    if (lExpired)
    {
        printf("%s: %s EXPIRED!\n", aName, aEvent.ToString().c_str());
    }
    else
    {
        printf("%s: %s waiting.\n", aName, aEvent.ToString().c_str());
    }
}


int _tmain(int argc, _TCHAR* argv[])
{
/*
    Uint32 range = 256;
    unsigned char max = range-1;

    unsigned char t1 = 245;
    _tprintf(_T("    t1      t2      dt       dt1      dt2\n"));
    for (unsigned char t2=0;t2<16;t2++)
    {
        signed char dt = t2-t1;
        if (dt < 0) dt += range;
        signed char dt1 = dt & max;
        unsigned char dt2 = t2-t1;
        //dt2 &= max;

        _tprintf(_T("t1: %02u, t2: %02u, dt: %02i, dt1: %02i, dt2: %02u\n"), t1, t2, dt, dt1, dt2);
    }

*/

    /*
    emuTimeType emuTime = 20;
    Event lInt1(5);          // expired
    Event lInt2(20);         // expired
    Event lInt3(25);         // not expired

    testEvent("Int1", lInt1, emuTime);
    testEvent("Int2", lInt2, emuTime);
    testEvent("Int3", lInt3, emuTime);

    emuTime = -20;
    Event lInt4(5);          // not expired
    Event lInt5(20);         // not expired
    Event lInt6(25);         // not expired

    testEvent("Int4", lInt4, emuTime);
    testEvent("Int5", lInt5, emuTime);
    testEvent("Int6", lInt6, emuTime);

    emuTime = -20;
    Event lInt7(-25);        // expired
    Event lInt8(-20);        // expired
    Event lInt9(-15);        // not expired

    testEvent("Int7", lInt7, emuTime);
    testEvent("Int8", lInt8, emuTime);
    testEvent("Int9", lInt9, emuTime);

    */

    Emulator::Instance()->initialize();
    system("pause");
	return 0;
}

