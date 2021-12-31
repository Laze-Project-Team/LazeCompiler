#pragma once
#ifdef __cplusplus
extern "C" {
#endif
#include "absyn.h"
#include "symbol.h"
#include "types.h"

void ESC_findEscape(A_exp exp);
bool ESC_checkEscapeFromType(Ty_ty type);
bool ESC_checkEscapeFromA_ty(A_ty type);
#ifdef __cplusplus
}
#endif