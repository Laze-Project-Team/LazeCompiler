#include "escape.h"

static void traverseStm(S_table env, int depth, A_stm stm);
static void traverseExp(S_table env, int depth, A_exp eexp);
static void traverseDec(S_table env, int depth, A_dec dec);
static void traverseVar(S_table env, int depth, A_var var);

void ESC_findEscape(A_exp exp)
{

}

bool ESC_checkEscapeFromType(Ty_ty type)
{
    bool result = FALSE;
    if(type->kind == Ty_name)
    {
        if(type -> u.name.sym == S_Symbol("real") || type -> u.name.sym == S_Symbol("int") || type -> u.name.sym == S_Symbol("bool") || type -> u.name.sym == S_Symbol("char") || type -> u.name.sym == S_Symbol("short"))
        {
            result = FALSE;
        }
        else
        {
            result = TRUE;
        }
    }
    else if(type -> kind == Ty_int || type -> kind == Ty_real || type -> kind == Ty_bool || type -> kind == Ty_char || type -> kind == Ty_short)
    {
        result = FALSE;
    }
    else if(type -> kind == Ty_pointer)
    {
        result = FALSE;
    }
    else
    {
      //printf("%d 377777777777777777777777777777777\n", type -> kind);
        result = TRUE;
    }
    return result;
}

bool ESC_checkEscapeFromA_ty(A_ty type)
{
    bool result = FALSE;
    if(type->kind == A_nameTy)
    {
        if(type -> u.name == S_Symbol("real") || type -> u.name == S_Symbol("int") || type -> u.name == S_Symbol("bool") || type -> u.name == S_Symbol("char") || type -> u.name == S_Symbol("short"))
        {
            result = FALSE;
        }
        else
        {
            result = TRUE;
        }
    }
    else if(type -> kind == A_pointerTy)
    {
        result = FALSE;
    }
    else
    {
        result = TRUE;
    }
    return result;
}