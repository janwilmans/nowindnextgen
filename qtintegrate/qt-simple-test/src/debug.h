//! Debug.h
#ifndef DEBUG_H
#define DEBUG_H

#ifdef _DEBUG
    #define NW_ASSERT(assertion) nw_assert(__FILE__, __LINE__, assertion, #assertion);
#else
    #define NW_ASSERT(assertion) //
#endif

#define DBERR nw_debug

void nw_debug(const char *cFormat, ...);
void nw_assert(const char* aFilename, int aLine, bool aAssertion, const char* aAssertionText);

#if __GNUC__ == 4
    #define likely(x)       __builtin_expect(!!(x), 1)
    #define unlikely(x)     __builtin_expect(!!(x), 0)
#else
    #define likely(x)       (x)
    #define unlikely(x)     (x)
#endif

#endif // DEBUG_H