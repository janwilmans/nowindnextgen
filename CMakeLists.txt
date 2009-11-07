cmake_minimum_required (VERSION 2.6) 
project (NOWINDV2)

# Recurse into the "Hello" and "Demo" subdirectories. This does not actually
# cause another cmake executable to run. The same process will walk through
# the project's entire directory structure.
add_subdirectory (cpu)

add_executable(nowind 

basetypes.h
BusComponent.cpp
BusComponent.h
Bus.cpp
Bus.h
Component.cpp
Component.h
debug.cpp
debug.h
Emulator.cpp
Emulator.h
Event.cpp
Event.h
EventUnitTest.cpp
EventUnitTest.h
FastDelegateDemo.cpp
FastDelegate.h
MemoryMapper.cpp
MemoryMapper.h
nowind_nextgen.cpp
NullDevice.cpp
NullDevice.h
RomMemory.cpp
RomMemory.h
Scheduler.cpp
Scheduler.h
SlotSelector.cpp
SlotSelector.h
Template.cpp
Template.h

)



