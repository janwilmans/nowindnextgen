#include "Emulator.h"
#include "Scheduler.h"
#include "Event.h"
#include "cpu/DummyCpu.h"

using namespace fastdelegate;

emuTimeType Emulator::emuTime = 0;

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

	mScheduler->addEvent(-1, MakeDelegate(this, &Emulator::interruptTestMethod));
	mScheduler->addEvent(-5, MakeDelegate(this, &Emulator::interruptTestMethod));
    mScheduler->addEvent(-6, MakeDelegate(this, &Emulator::interruptTestMethod));
    mScheduler->addEvent(-14, MakeDelegate(this, &Emulator::interruptTestMethod));
    mScheduler->addEvent(100, MakeDelegate(this, &Emulator::endOfRangeEvent));

    Emulator::emuTime = -20;
    mScheduler->run(cpu);
    //mScheduler->runNice(cpu);
}

void Emulator::endOfRangeEvent(emuTimeType emuTime, emuTimeType eventTime)
{
	printf("I am the endOfRangeEvent for eventTime: %d executed at emutime: %d\n", eventTime, emuTime);
    mScheduler->addEvent(-1, MakeDelegate(this, &Emulator::endOfRangeEvent));
}

void Emulator::interruptTestMethod(emuTimeType emuTime, emuTimeType eventTime)
{
	printf("I am the interruptTestMethod for eventTime: %d executed at emutime: %d\n", eventTime, emuTime);
}

Emulator::~Emulator(void)
{
}
