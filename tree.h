#pragma once
#include "absyn.h"
#include "util.h"

typedef struct T_stm_ *T_stm;
typedef struct T_exp_ *T_exp;
typedef struct T_module_ *T_module;
typedef struct T_stmList_ *T_stmList;
typedef struct T_expList_ *T_expList;
typedef struct T_moduleList_ *T_moduleList; 

struct T_stmList_
{
    T_stm head;
    T_stmList tail;
};
struct T_expList_
{
    T_exp head;
    T_expList tail;
};
struct T_moduleList_
{
    T_module head;
    T_moduleList tail;
};

typedef struct T_fundec_ *T_fundec;

typedef enum {T_i32, T_i64, T_f32, T_f64, T_none} T_type;
typedef enum {T_add, T_sub, T_mul, T_div_s, T_div_u, T_rem_s, T_rem_u, 
            T_eq, T_ne, T_lt_s, T_lt_u, T_gt_s, T_gt_u, T_le_s, T_le_u, T_ge_s, T_ge_u, T_and, T_or} T_binOp;
typedef enum {T_abs, T_neg, T_ceil, T_floor, T_trunc, T_nearest, T_sqrt} T_uniOp;
typedef struct T_typeList_ *T_typeList;
struct T_typeList_
{
    T_type head;
    T_typeList tail;
};

struct T_stm_
{
    enum { T_ifStm, T_blockStm, T_loopStm, T_binOpStm, T_setLocalStm,
             T_setGlobalStm, T_seqStm, T_storeStm, T_breakStm, T_callStm, T_returnStm, T_copyStm } kind;
    union
    {
        struct { T_exp test; T_type result; T_stm then; T_stm elsee;} iff;
        struct { T_stm body; } block;
        struct { T_exp test; T_stm body; int index; bool isFor; } loop;
        struct { T_type type; T_binOp oper; T_exp lhs; T_exp rhs; } binOp;
        struct { int index; T_exp exp; } setLocal;
        struct { int index; T_exp exp; } setGlobal;
        T_stmList seq;
        struct { T_exp addr; T_exp data; } store;
        int depth;
        struct { int index; string label; T_expList args; } call;
        T_exp returnn;
        struct {T_exp dest; T_exp src; T_exp size; } copy;
    } u;
};

struct T_exp_
{
    enum { T_binOpExp, T_uniOpExp, T_constExp,
         T_getLocalExp, T_getGlobalExp, T_callExp, 
         T_ifExp, T_convertExp, T_loadExp, T_seqExp } kind;
    T_type type;
    union
    {
        struct { T_binOp oper; T_exp lhs; T_exp rhs; } binOp;
        struct { T_uniOp oper; T_exp exp; } uniOp;
        struct { A_exp exp; } constt;
        struct { int index; } getLocal;
        struct { int index; } getGlobal;
        struct { int index; string label; T_expList args; } call;
        struct { T_exp test; T_exp then; T_exp elsee; } iff;
        struct {T_exp exp;} convert;
        struct {T_exp addr;} load;
        struct {T_expList list;} seq;
    } u;
};

struct T_fundec_
{
    int index;
    T_typeList params;
    T_typeList locals;
    T_type result;
    T_stm body;
    string name;
    T_exp var;
};

struct T_module_
{
    enum { T_typee, T_func, T_table, T_mem, T_global, T_elem, T_data,
             T_start, T_import, T_export, T_seqMod } kind;
    union
    {
        T_fundec func;

        struct {int pageSize;} memory;
        struct {T_type type; T_exp exp; } global;

        struct { T_exp exp; string data; } data;

        struct { string module; string name; T_module import; } import;
        struct { string name; int export; } export;
        T_moduleList seq;
    } u;
};

T_stm T_IfStm(T_exp test, T_type result, T_stm then, T_stm elsee);
T_stm T_BlockStm(T_stm body);
T_stm T_LoopStm(T_exp test, T_stm body, int index, bool isFor);
T_stm T_BinOpStm(T_type type, T_binOp oper, T_exp lhs, T_exp rhs);
T_stm T_SetLocalStm(int index, T_exp exp);
T_stm T_SetGlobalStm(int index, T_exp exp);
T_stm T_SeqStm(T_stmList list);
T_stm T_StoreStm(T_exp addr, T_exp data);
T_stm T_BreakStm(int depth);
T_stm T_CallStm(int index, string label, T_expList args);
T_stm T_ReturnStm(T_exp exp);
T_stm T_CopyStm(T_exp dest, T_exp src, T_exp size);

T_exp T_BinOpExp(T_type type, T_binOp oper, T_exp lhs, T_exp rhs);
T_exp T_UniOpExp(T_type type, T_uniOp oper, T_exp exp);
T_exp T_ConstExp(T_type type, A_exp exp);
T_exp T_GetLocalExp(T_type type, int index);
T_exp T_GetGlobalExp(T_type type, int index);
T_exp T_CallExp(T_type type, int index, string label, T_expList args);
T_exp T_IfExp(T_type type, T_exp test, T_exp then, T_exp elsee);
T_exp T_ConvertExp(T_type type, T_exp exp);
T_exp T_LoadExp(T_type type, T_exp addr);
T_exp T_SeqExp(T_type type, T_expList list);

T_fundec T_Fundec(T_typeList params, T_typeList locals, T_type result, T_stm body, string name, int index, T_exp var);

T_module T_FuncMod(T_fundec func);
T_module T_GlobalMod(T_type type, T_exp exp);
T_module T_MemMod(int pageSize);
T_module T_DataMod(T_exp exp, string data);
T_module T_ImportMod(string module, string name, T_module import);
T_module T_ExportMod(string name, int export);
T_module T_SeqMod(T_moduleList list);

T_stmList T_StmList(T_stm head, T_stmList tail);
T_expList T_ExpList(T_exp head, T_expList tail);
T_moduleList T_ModuleList(T_module head, T_moduleList tail);
T_typeList T_TypeList(T_type head, T_typeList tail);