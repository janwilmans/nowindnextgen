// nowind_nextgen.cpp : Defines the entry point for the console application.
//
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <tchar.h>

#include "Interrupt.h"
#include "Scheduler.h"

void testInterrupt(const char * aName, Interrupt& aInterrupt, emuTimeType aTime)
{
    bool lExpired = aInterrupt.IsExpired(aTime);
    if (lExpired)
    {
        printf("%s: %s EXPIRED!\n", aName, aInterrupt.ToString().c_str());
    }
    else
    {
        printf("%s: %s waiting.\n", aName, aInterrupt.ToString().c_str());
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

    emuTimeType emuTime = 20;
    Interrupt lInt1(5);          // expired
    Interrupt lInt2(20);         // expired
    Interrupt lInt3(25);         // not expired

    testInterrupt("Int1", lInt1, emuTime);
    testInterrupt("Int2", lInt2, emuTime);
    testInterrupt("Int3", lInt3, emuTime);

    emuTime = -20;
    Interrupt lInt4(5);          // not expired
    Interrupt lInt5(20);         // not expired
    Interrupt lInt6(25);         // not expired

    testInterrupt("Int4", lInt4, emuTime);
    testInterrupt("Int5", lInt5, emuTime);
    testInterrupt("Int6", lInt6, emuTime);

    emuTime = -20;
    Interrupt lInt7(-25);        // expired
    Interrupt lInt8(-20);        // expired
    Interrupt lInt9(-15);        // not expired

    testInterrupt("Int7", lInt7, emuTime);
    testInterrupt("Int8", lInt8, emuTime);
    testInterrupt("Int9", lInt9, emuTime);

    system("pause");
	return 0;
}

