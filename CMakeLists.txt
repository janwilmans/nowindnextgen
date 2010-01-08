cmake_minimum_required (VERSION 2.6) 
project (NOWIND_EMUV2)

   MACRO(COPY_IF_DIFFERENT FROM_DIR TO_DIR FILES TARGETS TAGS)
   # Macro to implement copy_if_different for a list of files
   # Arguments - 
   #   FROM_DIR - this is the source directory
   #   TO_DIR   - this is the destination directory
   #   FILES    - names of the files to copy 
   #              TODO: add globing. 
   #   TARGETS  - List of targets
   #   TAGS     - Since only the file name is used
   #              to generate rules, pre-pend a user 
   #              supplied tag to prevent duplicate rule errors. 
   SET(AddTargets "")
   FOREACH(SRC ${FILES})
       GET_FILENAME_COMPONENT(SRCFILE ${SRC} NAME) 
       # Command to copy the files to ${STEP1}/src, if changed.
       SET(TARGET  "${TAGS}/${SRCFILE}")
       IF("${FROM_DIR}" STREQUAL "")
           SET(FROM ${SRC})
       ELSE("${FROM_DIR}" STREQUAL "")
           SET(FROM ${FROM_DIR}/${SRC})
       ENDIF("${FROM_DIR}" STREQUAL "")        
       IF("${TO_DIR}" STREQUAL "")
           SET(TO ${SRCFILE})
       ELSE("${TO_DIR}" STREQUAL "")
           SET(TO   ${TO_DIR}/${SRCFILE})
       ENDIF("${TO_DIR}" STREQUAL "")
       ADD_CUSTOM_COMMAND(
           OUTPUT  ${TARGET}
           COMMAND ${CMAKE_COMMAND}
           ARGS    -E copy_if_different ${FROM} ${TO}
           COMMENT "Copying ${SRCFILE} ${TO_DIR}"
           )
       SET(AddTargets ${AddTargets} ${TARGET})
   ENDFOREACH(SRC ${FILES})
   SET(${TARGETS} ${AddTargets})
   ENDMACRO(COPY_IF_DIFFERENT FROM_DIR TO_DIR FILES TARGETS TAGS)

INCLUDE_DIRECTORIES("${CMAKE_SOURCE_DIR}")
INCLUDE_DIRECTORIES("${CMAKE_SOURCE_DIR}/SDL/include")

## SET(EXECUTABLE_OUTPUT_PATH "${CMAKE_SOURCE_DIR}")

FIND_PACKAGE(SDL) 

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
ClockChip.cpp
ClockChip.h
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

)

TARGET_LINK_LIBRARIES(nowind ${SDL_LIBRARY}) 

SOURCE_GROUP("cpu" REGULAR_EXPRESSION cpu/.* )
