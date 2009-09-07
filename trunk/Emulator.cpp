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
    Emulator::emuTime = 0;

    mScheduler = new Scheduler();
    mScheduler->initialize();

    AddressBus* addressBus = new AddressBus();
    IOBus* ioBus = new IOBus();
    CPU* cpu = new Z80(*addressBus, *ioBus);
    mScheduler->run(cpu);
    //mScheduler->runNice(cpu);

    // tests the scheduler using a dummy-cpu
    //test();
}

// create and initialize _everything_
void Emulator::test(void)
{
    Uint32 a=1;
    Uint32 b=3000000000;
    Sint32 c = a - b;
    DBERR("test c, should be -2999999999, but will fail: %d\n", c);

    Uint64 d=1;
    Uint64 e=3000000000;
    Sint64 f = d - e;
    DBERR("test f, should be -2999999999: %lld\n", f);

    AddressBus* addressBus = new AddressBus();
    IOBus* ioBus = new IOBus();
    DummyCpu* cpu = new DummyCpu(*addressBus, *ioBus);

	mScheduler->addEvent(-1, MakeDelegate(this, &Emulator::interruptTestMethod));
	mScheduler->addEvent(-5, MakeDelegate(this, &Emulator::interruptTestMethod));
    mScheduler->addEvent(-6, MakeDelegate(this, &Emulator::interruptTestMethod));
    mScheduler->addEvent(-14, MakeDelegate(this, &Emulator::interruptTestMethod));
    mScheduler->addEvent(10, MakeDelegate(this, &Emulator::interruptTestMethod));

    Emulator::emuTime = -2;
    Emulator::emuTime++;
    DBERR("Emulator::emuTime: %u\n", Emulator::emuTime);
    Emulator::emuTime++;
    DBERR("Emulator::emuTime: %u\n", Emulator::emuTime);
    Emulator::emuTime++;
    DBERR("Emulator::emuTime: %u\n", Emulator::emuTime);
    Emulator::emuTime++;
    DBERR("Emulator::emuTime: %u\n", Emulator::emuTime);

    Emulator::emuTime = -20;

    mScheduler->run(cpu);
}

void Emulator::endOfRangeEvent(emuTimeType emuTime, emuTimeType eventTime)
{
	DBERR("I am the endOfRangeEvent for eventTime: %d executed at emutime: %d\n", eventTime, emuTime);
    mScheduler->addEvent(-1, MakeDelegate(this, &Emulator::endOfRangeEvent));
}

void Emulator::interruptTestMethod(emuTimeType emuTime, emuTimeType eventTime)
{
	DBERR("I am the interruptTestMethod for eventTime: %d executed at emutime: %d\n", eventTime, emuTime);
}

Emulator::~Emulator(void)
{
}
