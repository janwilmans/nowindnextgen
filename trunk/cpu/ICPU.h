//! ICPU.h
#ifndef ICPU_H
#define ICPU_H

class ICPU {

    virtual void ExecuteInstructionsUntil(emuTimeType) = 0;
};

#endif //ICPU_H
