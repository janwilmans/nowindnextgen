#include "Emulator.h"
#include "Scheduler.h"
#include "Event.h"
#include "cpu/DummyCpu.h"

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

    DummyCpu* cpu = new DummyCpu();

    mScheduler->addEvent(-5, MakeDelegate(this, &Emulator::interruptTestMethod));
    mScheduler->addEvent(-6, MakeDelegate(this, &Emulator::interruptTestMethod));
    mScheduler->addEvent(0, MakeDelegate(this, &Emulator::interruptTestMethod));
    mScheduler->addEvent(-1, MakeDelegate(this, &Emulator::endOfRangeEvent));

    mScheduler->runUsing(cpu);
}

void Emulator::endOfRangeEvent(emuTimeType aTime)
{
    printf("I am the endOfRangeEvent at emutime: %d\n", aTime);
    mScheduler->addEvent(-1, MakeDelegate(this, &Emulator::endOfRangeEvent));
}

void Emulator::interruptTestMethod(emuTimeType aTime)
{
    printf("I am the interruptTestMethod at emutime: %d\n", aTime);
}

Emulator::~Emulator(void)
{
}
