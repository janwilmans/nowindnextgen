//! Debugger.h
#ifndef DEBUGGER_H
#define DEBUGGER_H

#include "Emulator.h"
#include "Scheduler.h"
#include "SlotSelector.h"
#include "cpu/CPU.h"

namespace nowind {

class Emulator;
class Scheduler;
class SlotSelector;
class Z80;

class Debugger 
{
public:
    Debugger(Emulator& emulator, Scheduler& scheduler, SlotSelector& slotSelector, Z80& z80);
    enum DebugAction { DebugActionEnableInstructionLogger };

    void enableInstructionLogger();
    void instructionLogEvent();
    
    void eventAtEmutime(emuTimeType, DebugAction);
    void eventAtRegPc(word, DebugAction);

    void checkEventRegPc();
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

    word mRegpc;
    DebugAction mRegpcAction;

};

} // namespace nowind

#endif // DEBUGGER_H