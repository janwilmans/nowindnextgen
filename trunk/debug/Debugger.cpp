#include "Debugger.h"

#include "Emulator.h"
#include "Scheduler.h"
#include "SlotSelector.h"
#include "cpu/Z80.h"

using namespace nowind;
using namespace fastdelegate;

Debugger::Debugger(Emulator& emulator, Scheduler& scheduler, SlotSelector& slotSelector, Z80& z80) : 
mEmulator(emulator), 
mScheduler(scheduler),
mSlotSelector(slotSelector),
mZ80(z80)
{
}

void Debugger::enableInstructionLogger(emuTimeType aTime)
{
    preInstructionLogEvent();
    mScheduler.addEvent(aTime, MakeDelegate(this, &Debugger::instructionLogEvent));
}

void Debugger::preInstructionLogEvent()
{
    sprintf(mDebugString, "%04X %-15s ", mZ80.reg_pc, mZ80.getMnemonics(mZ80.reg_pc, mZ80.readWord(mZ80.reg_pc), mZ80.readWord(mZ80.reg_pc+2)).c_str());
}
 
void Debugger::instructionLogEvent(emuTimeType emuTime, emuTimeType eventTime)
{
    DBERR("%s", mDebugString);
    mZ80.dumpStateInfo(mSlotSelector);
    enableInstructionLogger(); // reschedule myself
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
