cmake_minimum_required (VERSION 2.6) 
project (NOWINDV2)

add_subdirectory(cpu)
add_subdirectory(debug)
add_subdirectory(video)

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
FastDelegate.h
MemoryMapper.cpp
MemoryMapper.h
nowind_nextgen.cpp
NullComponent.cpp
NullComponent.h
RomMemory.cpp
RomMemory.h
Scheduler.cpp
Scheduler.h
SlotSelector.cpp
SlotSelector.h
ClockChip.cpp
ClockChip.h
${CPU_SRCS}
)
