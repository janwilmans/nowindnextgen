#include "Debugger.h"
#include "cpu/Z80.h"

using namespace nowind;

Debugger::Debugger(Emulator& emulator, Scheduler& scheduler, SlotSelector& slotSelector, Z80& z80) : 
mEmulator(emulator), 
mScheduler(scheduler),
mSlotSelector(slotSelector),
mZ80(z80)
{
}

void Debugger::enableInstructionLogger()
{

}

void Debugger::instructionLogEvent()
{

}

void Debugger::eventAtEmutime(emuTimeType aTime, DebugAction action)
{

}

void Debugger::eventAtRegPc(word value, DebugAction action)
{
    //todo: schedule event checkEventRegPc
    mRegpc = value;
    mRegpcAction = action;
}

void Debugger::checkEventRegPc()
{
    if (mZ80.reg_pc == mRegpc) 
    {
        executeDebugAction(mRegpcAction);
    }
}

void Debugger::executeDebugAction(DebugAction action)
{
    switch(action)
    {
    case DebugActionEnableInstructionLogger:
        enableInstructionLogger();
        break;
    }
}

Debugger::~Debugger()
{

}

void Debugger::initialize()
{

}

void Debugger::prepare_shutdown()
{

}
