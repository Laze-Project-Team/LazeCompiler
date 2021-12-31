#pragma once
#ifdef __cplusplus
extern "C" {
#endif
#include "temp.h"
#include "types.h"
#include "frame.h"
enum accessType {inFrame, inLocal, inGlobal};
struct F_access_
{
    enum accessType kind;
    Ty_ty type;
    bool isParam;
    int paramIndex;
    union
    {
        int offset;
        int index;
    } u;
};
struct F_frame_
{
    Temp_label name;
    F_accessList accessList;
    Ty_tyList localsType;
    Ty_tyList localsTypeTemp;
    int offset;
    int frameSize;
    int locals;
};
#ifdef __cplusplus
}
#endif
