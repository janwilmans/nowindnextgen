#include "Emulator.h"
#include "Scheduler.h"
#include "Event.h"

using namespace fastdelegate;

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
    mScheduler->addEvent(0, MakeDelegate(this, &Emulator::interruptTestMethod));

    mScheduler->run();
}


void Emulator::interruptTestMethod(emuTimeType aTime)
{
    printf("I am the interruptTestMethod with %d\n", aTime);
}

Emulator::~Emulator(void)
{
}
