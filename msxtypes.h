//! msxtypes.h
#ifndef MSXTYPES_H
#define MSXTYPES_H

#include "SDL_types.h"

namespace nowind
{

typedef Uint32 byte;            // should be (at least) an 8 bit type
typedef Uint32 word;            // should be (at least) an 16 bit type

// 'typedef Uint8 byte' is 'safer' but slightly slower
// if only Uint8 works someone is making assumptions about the
// size of msx-memory locations (example: load rom into memory
// by accessing memory directly using (char *), this is wrong!)

typedef Uint32 emuTimeType;     // should be (at least) an 32 bit type
typedef Uint32 msTimeType;      // should be (at least) an 32 bit type

}

#endif

// todo: use SDL types for Uint16/Uint32 through out the emulator
//       instead of windows specific types or unsigned long / unsigned int
