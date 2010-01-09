cmake_minimum_required (VERSION 2.6) 
project (NOWIND_EMUV2)

INCLUDE_DIRECTORIES("${CMAKE_SOURCE_DIR}")
INCLUDE_DIRECTORIES("${CMAKE_SOURCE_DIR}/SDL/include")

## SET(EXECUTABLE_OUTPUT_PATH "${CMAKE_SOURCE_DIR}")

FIND_PACKAGE(SDL) 
FIND_PACKAGE(BOOST)

IF(SDL_FOUND) 
  INCLUDE_DIRECTORIES(${SDL_INCLUDE_DIR}) 
ELSE()
  MESSAGE (STATUS "SDL not found, trying fallback to windows defaults") 
  SET (SDL_LIBRARY "Sdl.lib SDLmain.lib user32.lib")
  LINK_DIRECTORIES("${CMAKE_SOURCE_DIR}/SDL/lib") 
ENDIF()

# todo: copy extra files to target dir     
SET(EXTRA_FILES mainrom.rom MSX2.ROM MSX2EXT.ROM)

add_executable(nowind 
   WIN32  # Only if you don't want the DOS prompt to appear in the background in Windows
   MACOSX_BUNDLE

basetypes.h
BusComponent.cpp
BusComponent.h
Bus.cpp
Bus.h
ClockChip.cpp
ClockChip.h
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
cpu/CPU.cpp
cpu/CPU.h
cpu/Z80.cpp
cpu/Z80.h
cpu/NewZ80.cpp
cpu/NewZ80.h
cpu/opcodes.inc
cpu/opcodesCB.inc
cpu/opcodesDD.inc
cpu/opcodesDDCB.inc
cpu/opcodesED.inc
video/V9938.cpp
video/V9938.h
debug/Debugger.cpp
debug/Debugger.h

)

TARGET_LINK_LIBRARIES(nowind ${SDL_LIBRARY}) 

SOURCE_GROUP("cpu" REGULAR_EXPRESSION cpu/.* )
SOURCE_GROUP("video" REGULAR_EXPRESSION video/.* )
SOURCE_GROUP("debugger" REGULAR_EXPRESSION debugger/.* )
SOURCE_GROUP("tests" FILES EventUnitTest.cpp EventUnitTest.h )
SOURCE_GROUP("main" FILES basetypes.h briljant_ideas.txt debug.cpp debug.h FastDelegate.h nowind_nextgen.cpp )
SOURCE_GROUP("infrastructure" FILES BusComponent.cpp BusComponent.h Bus.cpp Bus.h Event.cpp Event.h Emulator.cpp Emulator.h)

