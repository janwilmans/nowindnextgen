// nowind_nextgen.cpp : Defines the entry point for the console application.
//
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <tchar.h>

#include "Event.h"
#include "debug.h"
#include "Emulator.h"

int _tmain(int argc, _TCHAR* argv[])
{

	//SDL_Init(SDL_INIT_TIMER);
    
    Emulator::Instance()->initialize();
    system("pause");
	return 0;
}
