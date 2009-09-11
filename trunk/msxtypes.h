//! msxtypes.h
#ifndef MSXTYPES_H
#define MSXTYPES_H

#include "SDL_types.h"

typedef Uint8 byte;          // should be (exactly) an 8 bit type (todo: find out why)
typedef Uint16 word;         // should be (at least) an 16 bit type

typedef Uint32 emuTimeType;     // should be (at least) an 32 bit type
typedef Uint32 msTimeType;      // should be (at least) an 32 bit type

#endif

// todo: use SDL types for Uint16/Uint32 through out the emulator
//       instead of windows specific types or unsigned long / unsigned int
