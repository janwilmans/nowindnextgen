#include "Emulator.h"
#include "Scheduler.h"
#include "Event.h"
#include "cpu/DummyCpu.h"
#include "Bus.h"
#include "MemoryMapper.h"
#include "RomMemory.h"
#include "SlotSelector.h"
#include "cpu/Z80.h"
#include "cpu/NewZ80.h"
#include "video/V9938.h"

using namespace fastdelegate;
using namespace nowind;
using namespace std;

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

    // testing rule: 
    // if you pass the ownership of an object, use a pointer
    // if you want to pass access to an object, use a reference

    Bus* bus = new Bus(*mScheduler);
//    NewZ80* cpu = new NewZ80(*bus);
    Z80* cpu = new Z80(*bus);

    MemoryMapper* mapper = new MemoryMapper(*bus, 256);
	RomMemory* mainRom = new RomMemory(*bus, "mainrom.rom");
    SlotSelector* slotSelector = new SlotSelector(*bus);
    V9938* vdp = new V9938(*bus);
    
    //Ppi* ppi = new Ppi(*SlotSelector);

    /* initialization of all components */ 
    
    bus->prepare();
    cpu->prepare();
    mapper->prepare();
    slotSelector->prepare();
    mainRom->prepare();
    vdp->prepare();
    
    bus->initialize();
    cpu->initialize();
    mapper->initialize();
    slotSelector->initialize();
    mainRom->initialize();
    vdp->initialize();
    
    /* initialization of all components */ 
 
    // attach device to the required ports by calling IODevice::attachIO
    bus->addIODevice(mapper);           
    bus->addIODevice(slotSelector);
    slotSelector->addBusComponent(mapper, 3, 2);
	slotSelector->addBusComponent(mainRom, 0, 0);
    
    //cpu->setupBdosEnv("cpu/zexall/zexall.com"); // loads rom, everything should be ready before initialize is called
    //cpu->setupBdosEnv("asm/maptest.com");
    
    cpu->reset();
    cpu->setPC(0); //0x100 for zexall.com
    mScheduler->run(cpu);
    //mScheduler->runNice(cpu);

    // tests the scheduler using a dummy-cpu
    //test();
    
    /* shutdown */
    
    bus->prepare_shutdown();
    cpu->prepare_shutdown();
    mapper->prepare_shutdown();
    slotSelector->prepare_shutdown();   
    
     /* shutdown */
     exit(0);
}

// create and initialize _everything_
void Emulator::test(void)
{
    Uint32 a = 1;
    Uint32 b = 3000000000;
    Sint32 c = a - b;
    DBERR("test c, should be -2999999999, but will fail: %d\n", c);

    Uint64 d = 1;
    Uint64 e = 3000000000;
    Sint64 f = d - e;
    DBERR("test f, should be -2999999999: %lld\n", f);

    Bus* bus = new Bus(*mScheduler);
    DummyCpu* cpu = new DummyCpu(*bus);

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

    cpu->reset();
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
