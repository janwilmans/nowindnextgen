//! ICPU.h
#ifndef ICPU_H
#define ICPU_H

#include "msxtypes.h" 

class ICPU {

public:
    virtual emuTimeType ExecuteInstructionsUntil(emuTimeType) = 0;
};

#endif //ICPU_H
