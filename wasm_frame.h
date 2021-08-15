#pragma once
#include "temp.h"
#include "types.h"
#include "frame.h"

struct F_access_
{
    enum {inFrame, inLocal, inGlobal} kind;
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
