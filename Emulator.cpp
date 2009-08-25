#include "Emulator.h"
#include "Scheduler.h"
#include "Interrupt.h"

Emulator::Emulator(void)
{
}

Emulator* Emulator::Instance()
{
	static Emulator lInstance;
	return &lInstance;
}

// create and initialize _everything_
void Emulator::initialize(void)
{
    mScheduler = new Scheduler();
}


void Emulator::interruptTestMethod(emuTimeType aTime)
{

}

Emulator::~Emulator(void)
{
}
