#include "env.hpp"

int funcs;

E_enventry E_VarEntry(Tr_access access, Ty_ty ty)
{
    E_enventry p = (E_enventry)checked_malloc(sizeof(*p));
    p -> kind = E_varentry;
    p -> u.var.ty = ty;
    p -> u.var.access = access;
    return p;
}
E_enventry E_FuncEntry(Tr_level level, Temp_label label, Ty_tyList params, Tr_access result, Ty_ty returnType)
{
    E_enventry p = (E_enventry)checked_malloc(sizeof(*p));
    p -> kind = E_funcentry;
    p -> u.func.formals = params;
    p -> u.func.result = result;
    p -> u.func.returnType = returnType;
    p -> u.func.level = level;
    p -> u.func.label = label;
    p -> u.func.index = funcs;
    funcs++;
    return p;
}
E_enventry E_ClassEntry(S_symbol name, int size, S_table varTypes, S_table methods, Ty_ty type)
{
    E_enventry p = (E_enventry)checked_malloc(sizeof(*p));
    p -> kind = E_classentry;
    p -> u.classs.name = name;
    p -> u.classs.size = size;
    p -> u.classs.varTypes = varTypes;
    p -> u.classs.methods = methods;
    p -> u.classs.type = type;
    return p;
}
E_enventry E_TemplateEntry(S_symbol name, A_dec dec)
{
    E_enventry p = (E_enventry)checked_malloc(sizeof(*p));
    p -> kind = E_templateentry;
    p -> u.templatee.dec = dec;
    if(dec -> kind == A_functionDec)
    {
        p -> u.templatee.kind = E_functemplate;
    }
    else if(dec -> kind == A_classDec)
    {
        p -> u.templatee.kind == E_classtemplate;
    }
    p -> u.templatee.name = name;
    p -> u.templatee.specific = S_empty();
    p -> u.templatee.venv = S_empty();
    p -> u.templatee.tenv = S_empty();
    return p;
}
E_enventry E_PolyEntry(Ty_ty type)
{
    E_enventry p = (E_enventry)checked_malloc(sizeof(*p));
    p -> kind = E_polyentry;
    p -> u.poly = type;
    return p;
}
E_enventry E_ListEntry(E_enventry firstEntry){
    E_enventry p = (E_enventry)checked_malloc(sizeof(*p));
    p -> kind = E_listEntry;
    p -> u.list = new std::vector<E_enventry>();
    p -> u.list -> push_back(firstEntry);
    return p;
}

//default types
S_table E_base_tenv(void)
{
    S_table tenv = S_empty();
    return tenv;
}
//default functions
S_table E_base_fenv(void)
{
    funcs = 0;
    S_table venv = S_empty();
    return venv;
}