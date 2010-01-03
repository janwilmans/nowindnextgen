//! Debugger.h
#ifndef DEBUGGER_H
#define DEBUGGER_H

#include "basetypes.h"
#include "Emulator.h"

namespace nowind {

class Scheduler;
class SlotSelector;
class Z80;

class Debugger 
{
public:
    Debugger(Emulator& emulator, Scheduler& scheduler, SlotSelector& slotSelector, Z80& z80);
    enum DebugAction { DebugActionEnableInstructionLogger };

    void enableInstructionLogger() { enableInstructionLogger(Emulator::emuTime+1); }
    void enableInstructionLogger(emuTimeType aTime);

    void preInstructionLogEvent();
    void instructionLogEvent(emuTimeType emuTime, emuTimeType eventTime);
    
    void eventAtEmutime(emuTimeType, DebugAction);
    void debugEvent(emuTimeType emuTime, emuTimeType eventTime);

    void eventAtRegPc(word, DebugAction);
    void checkEventRegPc(emuTimeType emuTime, emuTimeType eventTime);
    void executeDebugAction(DebugAction action);
    
    // the initialize method should create relations to other objects 
    // it can assume that 'prepare' has been called on all existing objects in the broker.
    virtual void initialize();

    // should do any actions needed before the destructor 
    // can be savely called, such as: unregister objects from the broker,
    // stop threads...etc.
    virtual void prepare_shutdown();

    // the destructor should release any allocated resources (memory/filehandles etc.) during runtime 
    virtual ~Debugger();
private:

    Emulator& mEmulator;
    Scheduler& mScheduler;
    SlotSelector& mSlotSelector;
    Z80& mZ80;

    char mDebugString[200];

    DebugAction mDebugAction;
    word mRegpc;
    DebugAction mRegpcAction;

};

} // namespace nowind

#endif // DEBUGGER_H