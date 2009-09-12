#define MAXMSG 2000

#include <stdio.h>
#include <stdarg.h>

#ifdef _MSC_VER

#include <windows.h>
#include <assert.h>

// sprintf_s (and also _snprintf_s) are not c99 compliant on MSC
// we implement it by wrapping _vscprintf and _vsnprintf_s
int nw_vsnprintf(char *str, size_t size, const char *format, va_list ap)
{
    size_t count;
    count = _vscprintf(format, ap);
    _vsnprintf_s(str, size, _TRUNCATE, format, ap);
    return count;
}

#else // not MSV_VER

int nw_vsnprintf(char *str, size_t size, const char *format, va_list ap)
{
    return ::vsnprintf(str, size, format, ap);
}
#endif // _MSC_VER

int nw_snprintf(char* str, size_t size, const char* format, ...)
{
    size_t count;
    va_list ap;
    va_start(ap, format);
    count = nw_vsnprintf(str, size, format, ap);
    va_end(ap);
    return count;
}

void nw_debug(const char *cFormat, ...)
{
    va_list arg_list;
    char scratch[MAXMSG];
    va_start (arg_list, cFormat);
    nw_vsnprintf(scratch, MAXMSG, cFormat, arg_list);
    va_end (arg_list);

    fprintf(stderr, scratch);
    #ifdef _MSC_VER
    OutputDebugStringA(scratch);
    #endif

}

void nw_assert(const char* aFilename, int aLine, bool aAssertion, const char* aAssertionText)
{
    char scratch[MAXMSG];
    if (aAssertion == false)
    {
        sprintf(scratch, "assertion '%s' failed at %s:%d\n", aAssertionText, aFilename, aLine);
        nw_debug(scratch);
        #ifdef _MSC_VER
        MessageBoxA(0, scratch, "Assertion Failed!", MB_OK);
        #endif
        assert(false);
    }
}
