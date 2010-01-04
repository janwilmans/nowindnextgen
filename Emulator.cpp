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
#include "clockChip.h"
#include "debug/Debugger.h"

using namespace fastdelegate;
using namespace nowind;
using namespace std;

emuTimeType Emulator::emuTime = 0;

/*
XML serialization:

- minixml.org?
- boost.org?
*/


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
    mBus = new Bus(*this);

    // testing rule: 
    // if you pass the ownership of an object, use a pointer
    // if you want to pass access to an object, use a reference

//    NewZ80* cpu = new NewZ80(*bus);
    Z80* cpu = new Z80(*this);

    MemoryMapper* mapper = new MemoryMapper(*this, 256);
	//RomMemory* mainRom = new RomMemory(*this, "mainrom.rom");
	RomMemory* mainRom = new RomMemory(*this, "MSX2.ROM");
	RomMemory* subRom = new RomMemory(*this, "MSX2EXT.ROM");
    
	SlotSelector* slotSelector = new SlotSelector(*this);
    slotSelector->setSlotExpanded(0, false);
    slotSelector->setSlotExpanded(1, false);
    slotSelector->setSlotExpanded(2, false);
    slotSelector->setSlotExpanded(3, true);
    
    V9938* vdp = new V9938(*this);
	ClockChip* clockChip = new ClockChip(*this);
    
    //Ppi* ppi = new Ppi(*SlotSelector);

    /* initialization of all components */ 
    
    mBus->prepare();
    cpu->prepare();
    mapper->prepare();
    slotSelector->prepare();
    mainRom->prepare();
	subRom->prepare();
    vdp->prepare();
	clockChip->prepare();
    
    mBus->initialize();
    cpu->initialize();
    mapper->initialize();
    slotSelector->initialize();
    mainRom->initialize();
	subRom->initialize();
    vdp->initialize();
	clockChip->initialize();
    
    /* initialization of all components */ 
 
    // attach device to the required ports by calling IODevice::attachIO
    mBus->addIODevice(mapper);           
    mBus->addIODevice(slotSelector);
    mBus->addIODevice(vdp);
	mBus->addIODevice(clockChip);

    
    // todo: find out why assert occurs!? 
    slotSelector->addBusComponent(mapper, 0, 0);
    cpu->setupBdosEnv("cpu/zexall/zexall.com"); // loads rom, everything should be ready before initialize is called
    cpu->reset();
    cpu->setPC(0); //0x100 for zexall.com


/*
    // zexall      
    slotSelector->addBusComponent(mapper, 0, 0);
    cpu->setupBdosEnv("cpu/zexall/zexall.com"); // loads rom, everything should be ready before initialize is called
    cpu->reset();
    cpu->setPC(0x100);
*/

/*
    // normal      
    slotSelector->addBusComponent(mapper, 3, 2);
	slotSelector->addBusComponent(mainRom, 0, 0);
	slotSelector->addBusComponent(subRom, 3, 0);
    cpu->reset();
    cpu->setPC(0);
*/    
    Debugger* debugger = new Debugger(*this, *mScheduler, *slotSelector, *cpu);

    //debugger->enableInstructionLogger();
    //debugger->eventAtEmutime(10*3579545, Debugger::DebugActionEnableInstructionLogger); // after 10 seconds of emutime
    //debugger->eventAtRegPc(0x03, Debugger::DebugActionEnableInstructionLogger);
    
    mScheduler->run(cpu);
    //mScheduler->runNice(cpu);
    // tests the scheduler using a dummy-cpu
    //test();
    
    /* shutdown */
    
    mBus->prepare_shutdown();
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

    Bus* bus = new Bus(*this);
    DummyCpu* cpu = new DummyCpu(*this);

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

void Emulator::interruptTestMethod(emuTimeType emuTime, emuTimeType eventTime)
{
    DBERR("I am the interruptTestMethod for eventTime: %d executed at emutime: %d\n", eventTime, emuTime);
}

Emulator::~Emulator(void)
{
}
