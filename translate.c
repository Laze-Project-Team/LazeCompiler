#include "translate.h"
#include "math.h"
#include "string.h"
#define LAZEMAIN "placeholder"

static Tr_access newAccess(Tr_level level, F_access access)
{
    Tr_access retValue = checked_malloc(sizeof(*retValue));
    retValue -> level = level;
    retValue -> access = access;
    return retValue;
}

static Tr_accessList F_accessToTr_access(Tr_level level, F_accessList flist)
{
    Tr_accessList list = checked_malloc(sizeof(*list));
    Tr_accessList retValue = list;
    for(;flist -> tail; flist = flist -> tail)
    {
        list -> head = newAccess(level, flist -> head);
        list -> tail = checked_malloc(sizeof(*list -> tail));
        list = list -> tail;
        list -> head = NULL;
        list -> tail = NULL;
    }
    return retValue;
}

Tr_accessList Tr_AccessList(Tr_access head, Tr_accessList tail)
{
    Tr_accessList list = checked_malloc(sizeof(*list));
    list -> head = head;
    list -> tail = tail;
    return list;
}

Tr_level Tr_outermost(void)
{
    static Tr_level outermost = NULL;
    if(outermost == NULL)
    {
        outermost = checked_malloc(sizeof(*outermost));
        Temp_label label = S_Symbol(LAZEMAIN);
        outermost -> label = label;
        outermost -> frame = F_newFrame(label, NULL, NULL, 0);
        outermost -> parent = NULL;
        outermost -> formals = NULL;
    }
    return outermost;
}
Tr_level Tr_newLevel(Tr_level parent, Temp_label name, U_boolList formals, Ty_tyList params, bool isMethod, Ty_ty class)
{
    Tr_level newLevel = checked_malloc(sizeof(*newLevel));
    newLevel -> parent = parent;
    newLevel -> label = name;
    newLevel -> isMethod = isMethod;
    if(newLevel -> isMethod == FALSE){
        newLevel -> class = Ty_Void();
    }
    else
        newLevel -> class = class; 
    newLevel -> frame = F_newFrame(name,  U_BoolList(TRUE, formals), Ty_TyList(Ty_Void(), params), isMethod);
    newLevel -> formals = F_accessToTr_access(newLevel, F_formals(newLevel -> frame));
    return newLevel;
}
Tr_accessList Tr_formals(Tr_level level)
{
    return level -> formals;
}
Tr_access Tr_allocLocal(Tr_level level, bool escape, Ty_ty type)
{
    Tr_access access = checked_malloc(sizeof(*access));
    access -> level = level;
    if(type -> kind == Ty_void)
        return NULL;
    else if(type -> kind == Ty_array)
    {
        if(type -> u.array.size > 0)
        {
            for(int i = 0; i < type -> u.array.size; i++)
            {
                // printf("test %d\n", type -> u.array.type -> kind);
                if(i == 0)
                    access -> access = Tr_allocLocal(level, escape, type -> u.array.type) -> access;
                else
                    Tr_allocLocal(level, escape, type -> u.array.type);
            }
        }
    }
    else
        access -> access = F_allocLocal(level -> frame, escape, type, TRUE, FALSE);
    return access;
}

Tr_exp Tr_AssignStm(A_pos pos, Tr_access access, T_exp exp)
{
    Tr_exp p = checked_malloc(sizeof(*p));
    p -> kind = Tr_t_stm;
    // printf("%d", p -> kind);
    if(access -> access -> kind != inFrame){
        if(access -> level -> parent)
        {
            p -> u.stm = T_SetLocalStm(access -> access -> u.index, exp);
        }
        else if(!(access -> level -> parent))
        {
            p -> u.stm = T_SetGlobalStm(access -> access -> u.index, exp);
        }
    }
    else if(access -> access -> kind == inFrame)
    {
        int addr = access -> level -> frame -> offset + access -> access -> u.offset;
        // printf("%d", access -> access -> u.offset); 
        p -> u.stm = T_StoreStm(T_ConstExp(T_i32, A_IntExp(pos, addr)), exp);
    }
    return p;
}
Tr_exp Tr_AddrAssignStm(A_pos pos, T_exp addr, T_exp exp)
{
    Tr_exp p = checked_malloc(sizeof(*p));
    p -> kind = Tr_t_stm;
    p -> u.stm = T_StoreStm(addr, exp);
    return p;
}
Tr_exp Tr_IfStm(A_pos pos, T_exp test, T_stm then, T_stm elsee)
{
    Tr_exp p = checked_malloc(sizeof(*p));
    p -> kind = Tr_t_stm;
    p -> u.stm = T_IfStm(test, T_none, then, elsee);
    return p;
}
Tr_exp Tr_WhileStm(A_pos pos, T_exp test, T_stm body)
{
    Tr_exp p = checked_malloc(sizeof(*p));
    p -> kind = Tr_t_stm;
    p -> u.stm = T_BlockStm(T_LoopStm(test, body, -1, FALSE));
    return p;
}
Tr_exp Tr_ForStm(A_pos pos, T_stm assign, T_exp condition, T_stm increment, T_stm body)
{
    Tr_exp p = checked_malloc(sizeof(*p));
    p -> kind = Tr_t_stm;
    T_stmList tempLoop = checked_malloc(sizeof(*tempLoop));
    T_stmList insideLoop = tempLoop;
    tempLoop -> head = body;
    tempLoop -> tail = checked_malloc(sizeof(*tempLoop));
    tempLoop = tempLoop -> tail;
    tempLoop -> head = increment;
    tempLoop -> tail = checked_malloc(sizeof(*tempLoop));
    tempLoop = tempLoop -> tail;
    tempLoop -> head = NULL;
    tempLoop -> tail = NULL;
    T_stmList tempBlock = checked_malloc(sizeof(*tempBlock));
    T_stmList insideBlock = tempBlock;
    tempBlock -> head = assign;
    tempBlock -> tail = checked_malloc(sizeof(*tempBlock));
    tempBlock = tempBlock -> tail;
    tempBlock -> head = T_LoopStm(condition, T_SeqStm(insideLoop), -1, TRUE);
    tempBlock -> tail = checked_malloc(sizeof(*tempBlock));
    tempBlock = tempBlock -> tail;
    tempBlock -> head = NULL;
    tempBlock -> tail = NULL;
    p -> u.stm = T_BlockStm(T_SeqStm(insideBlock));
    return p;
}
Tr_exp Tr_BreakStm(A_pos pos, int depth)
{
    Tr_exp p = checked_malloc(sizeof(*p));
    p -> kind = Tr_t_stm;
    p -> u.stm = T_BreakStm(1);
    return p;
}
Tr_exp Tr_ContinueStm(A_pos pos)
{
    Tr_exp p = checked_malloc(sizeof(*p));
    p -> kind = Tr_t_stm;
    p -> u.stm = T_BreakStm(0);
    return p;
}
Tr_exp Tr_CompoundStm(A_pos pos, T_stmList stmlist)
{
    Tr_exp p = checked_malloc(sizeof(*p));
    p -> kind = Tr_t_stm;
    p -> u.stm = T_SeqStm(stmlist);
    return p;
}
// Tr_exp Tr_DeclarationStm(A_pos pos, A_dec dec);
Tr_exp Tr_CallStm(A_pos pos, int index, string func, T_expList args)
{
    // printf("Tr_CallStm %d\n", index);
    Tr_exp p = checked_malloc(sizeof(*p));
    p -> kind = Tr_t_stm;
    p -> u.stm = T_CallStm(index, func, args);
    return p;
}
Tr_exp Tr_ReturnStm(A_pos pos, T_exp exp)
{
    Tr_exp p = checked_malloc(sizeof(*p));
    p -> kind = Tr_t_stm;
    p -> u.stm = T_ReturnStm(exp);
    return p;
}
Tr_exp Tr_LoopStm(A_pos pos)
{
    Tr_exp p = checked_malloc(sizeof(*p));
    p -> kind = Tr_t_stm;
    p -> u.stm = T_ReturnStm(NULL);
    return p;
}
Tr_exp Tr_MemCopyStm(A_pos pos, T_exp dest, T_exp src, int size)
{
    Tr_exp p = checked_malloc(sizeof(*p));
    p -> kind = Tr_t_stm;
    p -> u.stm = T_CopyStm(dest, src, T_ConstExp(T_i32, A_IntExp(pos, size)));
    return p;
}
Tr_exp Tr_NoStm(A_pos pos)
{
    Tr_exp p = checked_malloc(sizeof(*p));
    p -> kind = Tr_t_stm;
    p -> u.stm = NULL;
    return p;
}
Tr_exp Tr_VarExp(A_pos pos, T_type type, Tr_access access, bool isGlobal, bool isArray)
{
    Tr_exp p = checked_malloc(sizeof(*p));
    p -> kind = Tr_t_exp;
    if(access -> access -> kind == inLocal)
    {
      //printf("bro ur local\n");
        p -> u.exp = T_GetLocalExp(type, access -> access -> u.index);
    }
    else if(isGlobal)
    {
      //printf("isGlobal\n");
        p -> u.exp = T_GetGlobalExp(type, access -> access -> u.index);
    }
    else if(access -> access -> kind == inFrame)
    {
        if(access -> access -> isParam){
            p -> u.exp = T_GetLocalExp(type, access -> access -> u.index);
        }
        else
        {
            if(isArray)
            {
                int addr = access -> level -> frame -> offset + access -> access -> u.offset;
                p -> u.exp = T_ConstExp(T_i32, A_IntExp(pos, addr));
            }
            else
            {
                int addr = access -> level -> frame -> offset + access -> access -> u.offset;
                p -> u.exp = T_LoadExp(type, T_ConstExp(T_i32, A_IntExp(pos, addr)));
            }
        }
    }
    return p;
}
Tr_exp Tr_NilExp(A_pos pos)
{
    Tr_exp p = checked_malloc(sizeof(*p));
    p -> kind = Tr_t_exp;
    p -> u.exp = T_ConstExp(T_i32, A_IntExp(pos, 0));
    return p;
}
Tr_exp Tr_IntExp(A_pos pos, int i)
{
    Tr_exp p = checked_malloc(sizeof(*p));
    p -> kind = Tr_t_exp;
    p -> u.exp = T_ConstExp(T_i64, A_IntExp(pos,i));
    return p;
}
Tr_exp Tr_CharExp(A_pos pos, char *c)
{
    Tr_exp p = checked_malloc(sizeof(*p));
    p -> kind = Tr_t_exp;
    int charVal = 0;
    if(strlen(c) == 1)
    {
        charVal+=c[0];
    }
    else if(strlen(c) == 2)
    {
        charVal+=(256+c[0])*256;
        charVal+=(256+c[1]);
    }
    else if(strlen(c) == 3)
    {
        charVal+=(256+c[2])*256*256;
        charVal+=(256+c[1])*256;
        charVal+=(256+c[0]);
    }
    else
    {
      //printf("dfasikldfjasoifj\n");
    }
    p -> u.exp = T_ConstExp(T_i32, A_IntExp(pos, charVal));
    return p;
}
Tr_exp Tr_AddrExp(A_pos pos, int i)
{
    Tr_exp p = checked_malloc(sizeof(*p));
    p -> kind = Tr_t_exp;
    p -> u.exp = T_ConstExp(T_i32, A_IntExp(pos, i));
    return p;
}
Tr_exp Tr_DerefExp(A_pos pos, T_exp addr, T_type type)
{
    Tr_exp p = checked_malloc(sizeof(*p));
    p -> kind = Tr_t_exp;
    p -> u.exp = T_LoadExp(type, addr);
    return p;
}

// Tr_exp Tr_StringExp(A_pos pos, string s);
Tr_exp Tr_RealExp(A_pos pos, double f)
{
    Tr_exp p = checked_malloc(sizeof(*p));
    p -> kind = Tr_t_exp;
    p -> u.exp = T_ConstExp(T_f64, A_RealExp(pos, f));
    return p;
}
Tr_exp Tr_BoolExp(A_pos pos, bool b)
{
    Tr_exp p = checked_malloc(sizeof(*p));
    p -> kind = Tr_t_exp;
    p -> u.exp = T_ConstExp(T_i32, A_BoolExp(pos, b));
    return p;
}
Tr_exp Tr_CallExp(A_pos pos, T_type type, int index, string func, T_expList args)
{
    // printf("Tr_CallExp %d\n", index);
    Tr_exp p = checked_malloc(sizeof(*p));
    p -> kind = Tr_t_exp;
    p -> u.exp = T_CallExp(type, index, func, args);
    return p;
}
Tr_exp Tr_OpExp(A_pos pos, T_type type, T_binOp oper, T_exp left, T_exp right)
{
    Tr_exp p = checked_malloc(sizeof(*p));
    p -> kind = Tr_t_exp;
    p -> u.exp = T_BinOpExp(type, oper, left, right);
    return p;
}
// Tr_exp Tr_RecordExp(A_pos pos, S_symbol typ, A_efieldList fields);
Tr_exp Tr_SeqExp(A_pos pos, A_expList seq);
Tr_exp Tr_AssignExp(A_pos pos, A_var var, A_exp exp);
Tr_exp Tr_IfExp(A_pos pos, T_type type, T_exp test, T_exp then, T_exp elsee)
{
    Tr_exp p = checked_malloc(sizeof(*p));
    p -> kind = Tr_t_exp;
    p -> u.exp = T_IfExp(type, test, then, elsee);
    return p;
}
Tr_exp Tr_ArrayExp(A_pos pos, T_expList list)
{
    Tr_exp p = checked_malloc(sizeof(*p));
    p -> kind = Tr_t_exp;
    p -> u.exp = T_SeqExp(list -> head -> type, list);
    return p;
}


// Tr_exp Tr_FunctionDec(A_pos pos, T_typeList params, T_typeList locals, T_type result, T_stm body, bool isMain, int index)
// {
//     Tr_exp p = checked_malloc(sizeof(*p));
//     p -> kind = Tr_t_fundec;
//     p -> u.fundec = T_Fundec(params, locals, result, body, isMain, index);
//     return p;
// }
Tr_exp Tr_NoExp(A_pos pos)
{
    Tr_exp p = checked_malloc(sizeof(*p));
    p -> kind = Tr_t_exp;
    p -> u.exp = NULL;
    return p;
}