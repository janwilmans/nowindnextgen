// nowind_nextgen.cpp : Defines the entry point for the console application.
//
#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>

// dont use SDLmain
#undef main 

#include "Event.h"
#include "debug.h"
#include "Emulator.h"

int main(int argc, char **argv)
{
    SDL_Init(SDL_INIT_TIMER);
    
    nowind::Emulator::Instance()->initialize();
    system("pause");
    return 0;
}

#ifdef WIN32

#include <windows.h>
#include <tchar.h>

int WINAPI WinMain(      
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPSTR lpCmdLine,
    int nCmdShow)
{
    return main(__argc, __argv);
}

#endif
