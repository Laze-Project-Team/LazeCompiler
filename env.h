#pragma once
#ifdef __cplusplus
extern "C" {
#endif
#include "errormsg.h"
#include "types.h"
#include "symbol.h"
#include "absyn.h"
#include "translate.h"

extern int funcs;
typedef struct E_enventry_ *E_enventry;
enum E_entryType{E_varentry, E_funcentry, E_classentry, E_methodentry, E_templateentry, E_polyentry};
enum E_templateType{E_functemplate, E_classtemplate};
struct E_enventry_ 
{
    enum E_entryType kind;
    union
    {
        struct {
            S_symbol name;
            int size;
            //S_table of Ty_member
            S_table varTypes;
            //Table of E_funcentry
            S_table methods;
            Ty_ty type;
        } classs;
        struct {Ty_ty ty; Tr_access access;} var;
        struct {Ty_tyList formals; Ty_ty returnType; Tr_access result; Tr_level level; Temp_label label; int index; } func;
        struct 
        {
            enum E_templateType kind;
            A_dec dec;
            S_symbol name;
            //key: type, value: E_enventry for classs and func
            S_table specific;
            //the venv and tenv when the templatee was declared.
            S_table venv, tenv;
        } templatee;
        Ty_ty poly;
    } u;
};

E_enventry E_VarEntry(Tr_access access, Ty_ty ty);
E_enventry E_FuncEntry(Tr_level level, Temp_label label, Ty_tyList params, Tr_access result, Ty_ty returnType);
E_enventry E_ClassEntry(S_symbol name, int size, S_table varTypes, S_table methods, Ty_ty type);
E_enventry E_TemplateEntry(S_symbol name, A_dec dec);
E_enventry E_PolyEntry(Ty_ty type);

E_enventry E_UpdateTemp(E_enventry entry, Ty_ty type);

//default types
S_table E_base_tenv(void);
//default functions
S_table E_base_fenv(void);

#ifdef __cplusplus
}
#endif