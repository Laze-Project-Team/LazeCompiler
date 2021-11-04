#pragma once
#ifdef __cplusplus
extern "C" {
#endif
#include "temp.h"
#include "types.h"

typedef struct F_access_ *F_access;
typedef struct F_frame_ *F_frame;

typedef struct F_accessList_ *F_accessList;
struct F_accessList_ 
{
    F_access head;
    F_accessList tail;
};

F_accessList boolToF_access(F_frame f, U_boolList boolList, Ty_tyList types);
F_frame F_newFrame(Temp_label name, U_boolList list, Ty_tyList types, bool isMethod);
Temp_label F_name(F_frame f);
F_accessList F_formals(F_frame f);
F_access F_allocLocal(F_frame f, bool escape, Ty_ty type, bool isLocal, bool isParam);
void toInFrame(F_access access, F_frame frame);
#ifdef __cplusplus
}
#endif
