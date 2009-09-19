//! basetypes.h
#ifndef BASETYPES_H
#define BASETYPES_H

#include "SDL_types.h"
#include "FastDelegate.h"
 
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

typedef fastdelegate::FastDelegate1<word, byte> MemReadDelegate;   // 1 parameter, returns byte 
typedef fastdelegate::FastDelegate2<word, byte> MemWriteDelegate;   // 2 parameters, returns void 

typedef fastdelegate::FastDelegate1<word, byte> IOReadDelegate;   // 1 parameter, returns byte 
typedef fastdelegate::FastDelegate2<word, byte> IOWriteDelegate;   // 2 parameters, returns void 

// banksize  banks  ones 
//  1 KB     64     6
//  2 KB     32     5
//  4 KB     16     4
//  8 KB     8      3

static const Uint32 constSectionSize = 8*1024;
static const Uint32 constSections = (64*1024) / constSectionSize;
static const Uint32 constOnes = 3;      // amount of 1's in (constBanks-1)
static const Uint32 constSectionShift = 16-constOnes;  // amount to shift >> to convert absolute address to bank address

}

#endif // BASETYPES_H

// todo: use SDL types for Uint16/Uint32 through out the emulator
//       instead of windows specific types or unsigned long / unsigned int
