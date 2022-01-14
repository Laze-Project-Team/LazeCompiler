#include <stdio.h>
#include <string.h>
#include "semantic.hpp"

extern int funcs;
extern int memorySize;
extern std::string _mainFuncName;
extern std::string _stringClassName;

extern A_decList absyn_root = NULL;
static T_moduleList list = NULL;
static T_moduleList result = NULL;
static T_moduleList tableList = NULL;
static T_moduleList resultTableList = NULL;
static int tableIndex = 0;
static T_moduleList typeList = NULL;
static T_moduleList resultTypeList = NULL;
static int typeIndex = 0;
static bool hasLoop = FALSE;
static T_module loopFunc = NULL;
static T_moduleList resultScopeFuncs = NULL;
static T_moduleList scopeFuncs = NULL;
static T_stmList loopVarsStart = NULL;
static T_stmList loopVarsEnd = NULL;
static T_stmList stringLiterals = NULL;
static T_stmList stringLiteralsResult = NULL;

struct expty expTy(Tr_exp exp, Ty_ty ty)
{
    struct expty e;
    e.exp = exp;
    e.ty = ty;
    return e;
}

S_symbolList symList;
void assignAll(S_symbol sym, void *binding)
{
    Ty_member mem = (Ty_member)binding;
    symList = S_SymbolList(sym, symList);
}
S_table tempTable;
void copyTable(S_symbol sym, void *binding){
    E_enventry entry = (E_enventry)binding;
    S_enter(tempTable, sym, binding);
}
void printAll(S_symbol sym, void *binding)
{
  //printf("%s printf all\n", S_name(sym));
}
static Ty_ty actual_ty(Ty_ty ty);
static Ty_tyList makeParamTypeList(S_table venv, S_table tenv, A_fieldList params, Tr_level level);
static Ty_ty checkSymType(S_symbol sym);
static S_symbol returnSymFromType(Ty_ty);
static U_boolList makeEscapeList(A_fieldList list);
static T_type convertType(Ty_ty type);
static T_typeList convertAllType(Ty_tyList types);
static bool compType(Ty_ty lhs, Ty_ty rhs);
static T_exp convertAssign(Ty_ty type, struct expty exp, int pos);
static A_ty reverseArrayTy(A_ty type);
static A_var reverseSubVar(A_var var);
static A_classMemberList flipClassMemberList(A_classMemberList list);
static T_moduleList flipModList(T_moduleList list);
static S_symbol actual_name(A_var);
static S_symbol getName(A_var);
static S_symbol operToSym(A_oper oper);

T_moduleList SEM_transProg(A_decList declist)
{
    stringLiterals = T_StmList(NULL, NULL);
    stringLiteralsResult = stringLiterals;
    scopeFuncs = T_ModuleList(NULL, NULL);
    resultScopeFuncs = scopeFuncs;
    tableList = T_ModuleList(NULL, NULL);
    resultTableList = tableList;
    typeList = T_ModuleList(NULL, NULL);
    resultTypeList = typeList;
    loopFunc = (T_module)checked_malloc(sizeof(*loopFunc));
    loopFunc = T_FuncMod(NULL);
    loopVarsStart = T_StmList(NULL, NULL);
    loopVarsEnd = loopVarsStart;
    list = T_ModuleList(NULL, NULL);
    result = list;
    T_module mainFunc = NULL;
    S_table venv = E_base_fenv(), tenv = E_base_tenv();
    if (declist == NULL)
    {
        EM_error(0, "code.none");
    }
    A_decList decList = declist;
    list->head = T_ImportMod("js", "mem", T_MemMod(100));
    list->tail = T_ModuleList(NULL, NULL);
    list = list->tail;
    list->head = NULL;
    list->tail = NULL;
    int importDone = 0;
    for (; decList != NULL; decList = decList->tail)
    {
        A_dec dec = decList->head;
        if (dec == NULL)
        {
            continue;
        }
        // printf("dec kind %d\n", dec -> kind);
        T_module decMod = transDec(venv, tenv, dec, Tr_outermost(), FALSE, venv, Ty_Void());
        if (decMod)
        {
            if(!importDone){
                if(decMod -> kind != T_import){
                    importDone = 1;
                }
            }
            if(importDone && decMod -> kind == T_import){
                EM_error(dec -> pos, "code.import");
            }
            if (decMod->kind == T_func)
            {
                if (strcmp(decMod->u.func->name, "main") == 0)
                {
                    mainFunc = decMod;
                }
                if (strcmp(decMod->u.func->name, "loop") == 0)
                {
                    loopFunc = decMod;
                }
            }
            list->head = decMod;
            list->tail = T_ModuleList(NULL, NULL);
            list = list->tail;
            list->head = NULL;
            list->tail = NULL;
        }
    }
    if(!mainFunc){
        EM_error(0, "code.nomain");
    }
    list->head = T_ExportMod("main", mainFunc -> u.func -> index);
    list->tail = T_ModuleList(NULL, NULL);
    list = list->tail;
    // list->head = loopFunc;
    // list->tail = checked_malloc(sizeof(*list));
    // list = list->tail;
    list -> head = T_SeqMod(resultScopeFuncs);
    list -> tail = T_ModuleList(NULL, NULL);
    list = list -> tail;
    list -> head = T_SeqMod(resultTypeList);
    list -> tail = T_ModuleList(NULL, NULL);
    list = list -> tail;
    list -> head = T_FuncMod(T_Fundec(T_TypeList(T_none, NULL), T_TypeList(T_none, NULL), T_none, T_SeqStm(stringLiteralsResult), "__stringLiterals", funcs, NULL));
    funcs+=1;
    list -> tail = T_ModuleList(NULL, NULL);
    list = list -> tail;
    list -> head = T_ExportMod("__stringLiterals", --funcs);
    list -> tail = T_ModuleList(NULL, NULL);
    list = list -> tail;
    list -> head = T_ElemMod(Tr_AddrExp(0, 0) -> u.exp, resultTableList);
    list -> tail = T_ModuleList(NULL, NULL);
    list = list -> tail;
    list -> head = T_TableMod(tableIndex + 1);
    list -> tail = T_ModuleList(NULL, NULL);
    list = list -> tail;
    if (hasLoop)
    {
        list->head = T_ExportMod("loop", loopFunc -> u.func -> index);
        list->tail = T_ModuleList(NULL, NULL);
        list = list->tail;
    }
    list->head = NULL;
    list->tail = NULL;
    return result;
}

struct expty transStm(S_table venv, S_table tenv, A_stm stm, Tr_level level, bool isLoop, Ty_ty classs)
{
    assert(stm);
        switch (stm->kind)
        {
        case A_compoundStm:
        {
            //debug(stm->pos, "Compound Statement");
            A_stmList stmlist;
            T_stmList list = T_StmList(NULL, NULL);
            T_stmList result = list;
            list->head = NULL;
            list->tail = NULL;
            int i = 0;
            S_beginScope(venv);
            S_beginScope(tenv);
            for (i = 0, stmlist = stm->u.compound; stmlist != NULL; stmlist = stmlist->tail, i++)
            {
                if (stmlist->head != NULL)
                {
                    A_stm stmHead = stmlist->head;
                    // printf("%d stmHead\n", stmHead->kind);
                    struct expty exp = transStm(venv, tenv, stmHead, level, isLoop, classs);
                  //printf("hey %d\n", exp.exp -> kind);
                    if (exp.exp->kind == Tr_t_stm)
                    {
                        list->head = exp.exp->u.stm;
                        list->tail = T_StmList(NULL, NULL);
                        list = list->tail;
                        list->head = NULL;
                        list->tail = NULL;
                    }
                }
            }
            // S_dump(venv, &printAll);

            S_endScope(venv);
            S_endScope(tenv);
            return expTy(Tr_CompoundStm(stm->pos, result), Ty_Void());
            break;
        }
        case A_assignStm:
        {
            //debug(stm->pos, "Assign Statement");
            A_var var = stm->u.assign.var;
            struct expty varExpty = transVar(venv, tenv, var, level, isLoop, FALSE, stm -> u.assign.isDec, classs);
            S_symbol varName = NULL;
            if(varExpty.ty -> kind == Ty_name){
                E_enventry classEntry = (E_enventry)S_look(tenv, varExpty.ty -> u.name.sym);
                if(classEntry){
                    E_enventry funcEntry = (E_enventry)S_look(classEntry -> u.classs.methods, S_Symbol("="));
                    if(funcEntry){
                        if(funcEntry -> u.func.formals -> head){
                            Ty_ty lhs = funcEntry -> u.func.formals -> head;
                            struct expty assignExpty = transExp(venv, tenv, stm -> u.assign.exp, level, isLoop, classs);
                            if(compType(lhs, assignExpty.ty)){
                                if(stm -> u.assign.exp -> kind == A_stringExp){
                                    T_stmList assignList = T_StmList(NULL, NULL);
                                    T_stmList result = assignList;
                                    int arrayAddr = memorySize;
                                    T_expList list = assignExpty.exp -> u.exp -> u.seq.list;
                                    for(int i = 0; i < assignExpty.ty -> u.pointer -> u.array.size; i++)
                                    {
                                        int elementAddr = arrayAddr + i * assignExpty.ty -> u.pointer -> u.array.type -> size;
                                        memorySize += assignExpty.ty -> u.pointer -> u.array.type -> size;
                                        level -> frame -> frameSize += assignExpty.ty -> u.pointer -> u.array.type -> size;
                                        T_exp addrExp = Tr_AddrExp(stm -> pos, elementAddr)->u.exp;
                                        assignList -> head = Tr_AddrAssignStm(stm -> pos, addrExp, list -> head)->u.stm;
                                        assignList -> tail = T_StmList(NULL, NULL);
                                        assignList = assignList -> tail;
                                        assignList -> head = NULL;
                                        assignList -> tail = NULL;
                                        list = list -> tail;
                                    }
                                    struct expty callStm = transStm(venv, tenv, A_CallStm(stm -> pos, A_VarExp(stm -> pos, A_FieldVar(stm -> pos, var, S_Symbol("="))), A_ExpList(A_AddrExp(stm -> u.assign.exp -> pos, arrayAddr), A_ExpList(A_IntExp(stm -> u.assign.exp -> pos, assignExpty.ty -> u.pointer -> u.array.size), NULL))), level, isLoop, classs);
                                    assignList -> head = callStm.exp -> u.stm;
                                    assignList -> tail = T_StmList(NULL, NULL);
                                    assignList = assignList -> tail;
                                    assignList -> head = NULL;
                                    assignList -> tail = NULL;
                                    return expTy(Tr_CompoundStm(stm -> pos, result), Ty_Void());
                                }
                                return transStm(venv, tenv, A_CallStm(stm -> pos, A_VarExp(stm -> pos, A_FieldVar(stm -> pos, var, S_Symbol("="))), A_ExpList(stm -> u.assign.exp, NULL)), level, isLoop, classs);
                            }
                        }
                        
                    }
                }
            }
            if (var->kind == A_derefVar)
            {
                E_enventry varType = (E_enventry)S_look(venv, var->u.deref->u.simple);
                if(!varType)
                {
                    T_exp addr = varExpty.exp -> u.exp;
                    return expTy(Tr_AddrAssignStm(stm -> pos, addr, transExp(venv, tenv, stm->u.assign.exp, level, isLoop, classs).exp->u.exp), varExpty.ty);
                }
                return expTy(Tr_AddrAssignStm(stm->pos, Tr_VarExp(stm->pos, T_i32, varType->u.var.access, FALSE, FALSE)->u.exp, transExp(venv, tenv, stm->u.assign.exp, level, isLoop, classs).exp->u.exp), varType->u.var.ty);
            }
            else if (var->kind == A_subscriptVar)
            {
                E_enventry array = (E_enventry)S_look(venv, var->u.subscript.name);
                struct expty exp = transExp(venv, tenv, var->u.subscript.exp, level, isLoop, classs);
                if (stm->u.assign.exp)
                {
                    struct expty assignVal = transExp(venv, tenv, stm->u.assign.exp, level, isLoop, classs);
                    if(stm -> u.assign.exp -> kind == A_arrayExp)
                    {
                        T_stmList assignList = T_StmList(NULL, NULL);
                        T_stmList result = assignList;
                        T_expList list = assignVal.exp -> u.exp -> u.seq.list;
                        A_exp expp = NULL;
                        for(int i = 0; i < assignVal.ty -> u.array.size; i++)
                        {
                            expp = stm -> u.assign.exp -> u.array.list -> head;
                            A_var assignVar = NULL;
                            A_stm assign = NULL;
                            if(!stm -> u.assign.isDec)
                                assign = A_AssignStm(stm -> pos, A_SubscriptVar(stm -> pos, var, A_IntExp(stm -> pos, i)), expp, stm -> u.assign.isDec);
                            else
                                assign = A_AssignStm(stm -> pos, A_SubscriptVar(stm -> pos, A_SimpleVar(stm -> pos, var -> u.subscript.name), A_IntExp(stm -> pos, i)), expp, FALSE);
                            assign -> u.assign.isArrayExp = TRUE;
                            assignList -> head = transStm(venv, tenv, assign, level, isLoop, classs).exp -> u.stm;
                            assignList -> tail = T_StmList(NULL, NULL);
                            assignList = assignList -> tail;
                            assignList -> head = NULL;
                            assignList -> tail = NULL;
                            list = list -> tail;
                            stm -> u.assign.exp -> u.array.list = stm -> u.assign.exp -> u.array.list -> tail;
                        }
                        return expTy(Tr_CompoundStm(stm -> pos, result), Ty_Void());
                    }
                    else if(stm -> u.assign.exp -> kind == A_stringExp)
                    {
                        T_stmList assignList = T_StmList(NULL, NULL);
                        T_stmList result = assignList;
                        Tr_exp addrExp = NULL;
                        if(array && var -> u.subscript.var -> kind == A_simpleVar){
                            int arrayAddr = array->u.var.access->access->u.offset +
                            array->u.var.access->level->frame->offset;
                            addrExp = Tr_AddrExp(stm -> pos, arrayAddr);
                        }
                        else{
                            addrExp = transVar(venv, tenv, var, level, isLoop, FALSE, stm -> u.assign.isDec, classs).exp;
                        }
                        T_expList list = assignVal.exp -> u.exp -> u.seq.list;
                      //printf("%d size 276\n", assignVal.ty -> u.array.size);
                        for(int i = 0; i < assignVal.ty -> u.pointer -> u.array.size; i++)
                        {
                            int offset = i * assignVal.ty -> u.pointer -> u.array.type -> size;
                            assignList -> head = Tr_AddrAssignStm(stm -> pos, Tr_OpExp(stm -> pos, T_i32, T_add, addrExp->u.exp, Tr_AddrExp(stm -> pos, offset)->u.exp)->u.exp, list -> head)->u.stm;
                            assignList -> tail = T_StmList(NULL, NULL);
                            assignList = assignList -> tail;
                            assignList -> head = NULL;
                            assignList -> tail = NULL;
                            list = list -> tail;
                        }
                        return expTy(Tr_CompoundStm(stm -> pos, result), Ty_Void());
                    }
                    
                    struct expty varExp = transVar(venv, tenv, var, level, isLoop, TRUE, FALSE, classs);
                    struct expty assignExpty = transExp(venv, tenv, stm->u.assign.exp, level, isLoop, classs);
                    if((varExp.ty -> kind == Ty_name && assignExpty.ty -> kind == Ty_name) || (varExp.ty -> kind == Ty_poly && assignExpty.ty -> kind == Ty_poly))
                    {
                        if(compType(varExp.ty, assignExpty.ty))
                        {
                            if(assignExpty.exp -> u.exp -> kind == T_loadExp)
                                return expTy(Tr_MemCopyStm(stm -> pos, varExp.exp -> u.exp, assignExpty.exp -> u.exp -> u.load.addr, varExp.ty -> size), Ty_Void());
                            return expTy(Tr_MemCopyStm(stm -> pos, varExp.exp -> u.exp, assignExpty.exp -> u.exp, varExp.ty -> size), Ty_Void());
                        }
                        else
                        {
                            S_symbol varExpName = NULL;
                            S_symbol assignExpName = NULL;
                            if(varExp.ty -> kind == Ty_name){
                                varExpName = varExp.ty -> u.name.sym;
                            }
                            else if(varExp.ty -> kind == Ty_poly){
                                varExpName = varExp.ty -> u.poly.name;
                            }
                            if(assignExpty.ty -> kind == Ty_name){
                                assignExpName = assignExpty.ty -> u.name.sym;
                            }
                            else if(assignExpty.ty -> kind == Ty_poly){
                                assignExpName = assignExpty.ty -> u.poly.name;
                            }
                            EM_error(stm -> pos, "type.nomatch %s %s", S_name(varExpName), S_name(assignExpName));
                        }
                    }
                    if(!compType(varExp.ty, assignExpty.ty))
                    {
                        return expTy(Tr_AddrAssignStm(stm -> pos, varExp.exp -> u.exp, convertAssign(varExp.ty, assignExpty, stm -> pos)), Ty_Void());
                    }
                    return expTy(Tr_AddrAssignStm(stm->pos, varExp.exp->u.exp, assignExpty.exp->u.exp), Ty_Void());
                }
                else
                    return transVar(venv, tenv, var, level, isLoop, FALSE, stm -> u.assign.isDec, classs);
            }
            else if(var -> kind == A_simpleVar || var -> kind == A_fieldVar || var -> kind == A_arrowFieldVar)
            {
                E_enventry varType = (E_enventry)S_look(venv, var->u.simple);
                if (stm->u.assign.exp)
                {
                    A_exp exp = stm->u.assign.exp;
                    int index = 0;
                    E_enventry addrVar;
                    bool wasLocal = FALSE;
                    if (exp->kind == A_addressExp){
                        if (addrVar = (E_enventry)S_look(venv, exp->u.address->u.simple))
                            if (addrVar->u.var.access->access->kind != inFrame){
                                wasLocal = TRUE;
                                index = addrVar->u.var.access->access->u.index;
                            }
                    }
                    struct expty assignVal = transExp(venv, tenv, exp, level, isLoop, classs);
                    if (exp->kind == A_addressExp)
                    {
                        if(varType){
                            if(wasLocal){
                                return expTy(Tr_CompoundStm(stm->pos, T_StmList(Tr_AssignStm(stm->pos, addrVar->u.var.access,
                                    T_GetLocalExp(convertType(addrVar->u.var.ty), index))
                                    ->u.stm,
                                    T_StmList(Tr_AssignStm(stm->pos, varType->u.var.access, assignVal.exp->u.exp)->u.stm,
                                        T_StmList(NULL, NULL)))),
                                    Ty_Void());
                            }
                            else
                            {
                                return expTy(Tr_AssignStm(stm->pos, varType->u.var.access, assignVal.exp->u.exp), Ty_Void());
                            }
                        }
                        else{
                            Tr_exp varExp = transVar(venv, tenv, var, level, isLoop, FALSE, stm -> u.assign.isDec, classs).exp;
                            return expTy(Tr_AddrAssignStm(stm -> pos, varExp->u.exp->u.load.addr, assignVal.exp->u.exp), Ty_Void());
                        }
                    }
                    else if(exp -> kind == A_stringExp)
                    {
                        T_stmList assignList = T_StmList(NULL, NULL);
                        T_stmList result = assignList;
                        int arrayAddr = 0;
                        if(varType){
                            arrayAddr = varType -> u.var.access -> access -> u.offset +
                            varType -> u.var.access -> level -> frame -> offset;
                        }
                        T_expList list = assignVal.exp -> u.exp -> u.seq.list;
                        for(int i = 0; i < assignVal.ty -> u.pointer -> u.array.size; i++)
                        {
                            int elementAddr = arrayAddr + i * assignVal.ty -> u.pointer -> u.array.type -> size;
                            T_exp addrExp;
                            if(varType){
                                addrExp = Tr_AddrExp(stm -> pos, elementAddr)->u.exp;
                            }
                            else{
                                addrExp = transVar(venv, tenv, var, level, isLoop, TRUE, FALSE, classs).exp -> u.exp -> u.load.addr;
                                addrExp = Tr_OpExp(stm -> pos, T_i32, T_add, addrExp, Tr_AddrExp(stm -> pos, i * assignVal.ty -> u.pointer -> u.array.type -> size)->u.exp)->u.exp;
                            }
                            assignList -> head = Tr_AddrAssignStm(stm -> pos, addrExp, list -> head)->u.stm;
                            assignList -> tail = T_StmList(NULL, NULL);
                            assignList = assignList -> tail;
                            assignList -> head = NULL;
                            assignList -> tail = NULL;
                            list = list -> tail;
                        }
                        return expTy(Tr_CompoundStm(stm -> pos, result), Ty_Void());
                    }
                    else if(exp -> kind == A_arrayExp)
                    {
                        T_stmList assignList = T_StmList(NULL, NULL);
                        T_stmList result = assignList;
                        T_expList list = NULL;
                        A_expList explist = exp -> u.array.list;
                        if(assignVal.exp -> u.exp)
                        {
                            list = assignVal.exp -> u.exp -> u.seq.list;
                            for(int i = 0; i < assignVal.ty -> u.array.size; i++)
                            {
                                A_exp expp = explist -> head;
                                A_var var;
                                if(varType){
                                    var = stm -> u.assign.var;
                                }
                                else{
                                    if(stm -> u.assign.var -> kind == A_simpleVar)
                                        var = A_LvalSimpleVar(stm -> pos, stm -> u.assign.var -> u.simple);
                                    else if(stm -> u.assign.var -> kind == A_fieldVar)
                                        var = stm -> u.assign.var;
                                    else if(stm -> u.assign.var -> kind == A_arrowFieldVar)
                                        var = stm -> u.assign.var;
                                }
                                assignList -> head = transStm(venv, tenv, A_AssignStm(stm -> pos, A_SubscriptVar(stm -> pos, var, A_IntExp(stm -> pos, i)), expp, stm -> u.assign.isDec), level, isLoop, classs).exp -> u.stm;
                                assignList -> tail = T_StmList(NULL, NULL);
                                assignList = assignList -> tail;
                                assignList -> head = NULL;
                                assignList -> tail = NULL;
                                list = list -> tail;
                                explist = explist -> tail;
                            }
                        }
                        return expTy(Tr_CompoundStm(stm -> pos, result), Ty_Void());
                        break;
                    }
                    else
                    {
                        struct expty varExpty = transVar(venv, tenv, var, level, isLoop, FALSE, stm -> u.assign.isDec, classs);
                        if((varExpty.ty -> kind == Ty_name && assignVal.ty -> kind == Ty_name) || (varExpty.ty -> kind == Ty_poly && assignVal.ty -> kind == Ty_poly))
                        {
                            if(compType(varExpty.ty, assignVal.ty))
                            {
                                if(assignVal.exp -> u.exp -> kind == T_loadExp)
                                    return expTy(Tr_MemCopyStm(stm -> pos, varExpty.exp -> u.exp, assignVal.exp -> u.exp -> u.load.addr, varExpty.ty -> size), Ty_Void());
                                return expTy(Tr_MemCopyStm(stm -> pos, varExpty.exp -> u.exp, assignVal.exp -> u.exp, varExpty.ty -> size), Ty_Void());
                            }
                            else
                            {
                                S_symbol varExpName = NULL;
                                S_symbol assignExpName = NULL;
                                if(varExpty.ty -> kind == Ty_name){
                                    varExpName = varExpty.ty -> u.name.sym;
                                }
                                else if(varExpty.ty -> kind == Ty_poly){
                                    varExpName = varExpty.ty -> u.poly.name;
                                }
                                if(assignVal.ty -> kind == Ty_name){
                                    assignExpName = assignVal.ty -> u.name.sym;
                                }
                                else if(assignVal.ty -> kind == Ty_poly){
                                    assignExpName = assignVal.ty -> u.poly.name;
                                }
                                EM_error(stm -> pos, "type.nomatch %s %s", S_name(varExpName), S_name(assignExpName));
                            }
                        }
                        if (!compType(varExpty.ty, assignVal.ty))
                        {
                            if(varType){
                                return expTy(Tr_AssignStm(stm -> pos, varType -> u.var.access, convertAssign(varType -> u.var.ty, assignVal, exp -> pos)), Ty_Void());
                            }
                            else{
                                return expTy(Tr_AddrAssignStm(stm -> pos, varExpty.exp->u.exp->u.load.addr, convertAssign(varExpty.ty, assignVal, exp -> pos)), Ty_Void());
                            }
                        }
                        if(varExpty.ty -> kind == Ty_name)
                        {
                            return expTy(Tr_MemCopyStm(stm -> pos, varExpty.exp -> u.exp, assignVal.exp -> u.exp, varExpty.ty -> size), Ty_Void());
                        }
                        else if(varExpty.ty -> kind == Ty_array)
                        {
                            if(varExpty.exp -> u.exp -> kind == T_loadExp)
                                return expTy(Tr_MemCopyStm(stm -> pos, varExpty.exp -> u.exp -> u.load.addr, assignVal.exp -> u.exp, varExpty.ty -> size), Ty_Void());
                            return expTy(Tr_MemCopyStm(stm -> pos, varExpty.exp -> u.exp, assignVal.exp -> u.exp, varExpty.ty -> size), Ty_Void());
                        }
                        if(varType){
                            return expTy(Tr_AssignStm(stm->pos, varType->u.var.access, assignVal.exp->u.exp), Ty_Void());
                        }
                        else
                        {
                            if(varExpty.exp -> u.exp -> kind == T_loadExp)
                                return expTy(Tr_AddrAssignStm(stm -> pos, varExpty.exp->u.exp->u.load.addr, assignVal.exp -> u.exp), Ty_Void());
                            return expTy(Tr_AddrAssignStm(stm -> pos, varExpty.exp->u.exp, assignVal.exp -> u.exp), Ty_Void());
                        }
                    }
                }
                else
                {
                    return expTy(Tr_VarExp(stm->pos, convertType(varType->u.var.ty), varType->u.var.access, FALSE, FALSE), varType->u.var.ty);
                }
            }
        }
        case A_declarationStm:
        {
            //debug(stm->pos, "Declaration Statement");
            struct expty exp;
            // S_beginScope(venv);
            // S_beginScope(tenv);

            if (stm->u.declaration.dec->kind == A_varDec)
            {
                Ty_ty varDecType = transTy(venv, tenv, stm -> u.declaration.dec -> u.var.typ, level);
                if((varDecType -> kind == Ty_name || varDecType -> kind == Ty_poly) && stm -> u.declaration.dec -> u.var.var -> kind == A_simpleVar && !(stm -> u.declaration.dec -> u.var.init)){
                    // std::cout << S_name(stm -> u.declaration.dec -> u.var.var -> u.simple) << std::endl;
                    A_stm newStm = A_DeclarationStm(stm -> pos, A_ObjectDec(stm -> pos, stm -> u.declaration.dec -> u.var.typ, 
                    stm -> u.declaration.dec -> u.var.var -> u.simple, A_ExpList(NULL, NULL)));
                    exp = transStm(venv, tenv, newStm, level, isLoop, classs);
                    return exp;
                }
                T_module dec = transDec(venv, tenv, stm->u.declaration.dec, level, isLoop, venv, Ty_Void());
                if(stm -> u.declaration.dec -> u.var.init)
                    exp = transStm(venv, tenv, A_AssignStm(stm->pos, stm->u.declaration.dec->u.var.var, stm->u.declaration.dec->u.var.init, TRUE), level, FALSE, classs);
                else
                    exp = expTy(Tr_NoStm(stm -> pos), Ty_Void());
            }
            else if (stm->u.declaration.dec->kind == A_functionDec)
            {
                T_module dec = transDec(venv, tenv, stm->u.declaration.dec, level, TRUE, venv, Ty_Void());
                hasLoop = TRUE;
                loopFunc = dec;
                list -> head = dec;
                list -> tail = T_ModuleList(NULL, NULL);
                list = list -> tail;
                list -> head = NULL;
                list -> tail = NULL;
                exp = expTy(Tr_CompoundStm(stm -> pos, loopVarsStart), Ty_Void());
            }
            else if(stm -> u.declaration.dec -> kind == A_objectDec)
            {
                A_dec d = stm -> u.declaration.dec;
                
                transDec(venv, tenv, d, level, isLoop, venv, classs);
                S_symbol name = NULL;
                Ty_ty type = transTy(venv, tenv, d -> u.object.className, level);
                if(type -> kind == Ty_name)
                    name = d -> u.object.className -> u.name;
                else if(type -> kind == Ty_poly)
                    name = d-> u.object.className -> u.poly.name;
                if(name){
                    exp = transStm(venv, tenv, A_CallStm(d -> pos, A_VarExp(d -> pos, A_FieldVar(d -> pos, A_SimpleVar(d -> pos, d -> u.object.name), name)), d -> u.object.explist), level, isLoop, classs);
                }
            }
            // S_endScope(venv);
            // S_beginScope(tenv);
            return exp;
            break;
        }
        case A_ifStm:
        {
            //debug(stm->pos, "If Statement");
            if (transExp(venv, tenv, stm->u.iff.test, level, isLoop, classs).ty != Ty_Bool())
            {
                EM_error(stm->pos, "if.boolean");
            }
            struct expty then = transStm(venv, tenv, stm->u.iff.then, level, isLoop, classs);
            struct expty elsee = expTy(NULL, Ty_Void());
            if (stm->u.iff.elsee != NULL)
            {
                elsee = transStm(venv, tenv, stm->u.iff.elsee, level, isLoop, classs);
                return expTy(Tr_IfStm(stm->pos, transExp(venv, tenv, stm->u.iff.test, level, isLoop, classs).exp->u.exp,
                    then.exp->u.stm, elsee.exp->u.stm),
                    Ty_Void());
            }
            return expTy(Tr_IfStm(stm->pos, transExp(venv, tenv, stm->u.iff.test, level, isLoop, classs).exp->u.exp,
                then.exp->u.stm, NULL),
                Ty_Void());
        }
        case A_whileStm:
        {
            //debug(stm->pos, "While Statement");
            struct expty body;
            if (transExp(venv, tenv, stm->u.whilee.test, level, isLoop, classs).ty != Ty_Bool())
            {
                EM_error(stm->pos, "while.boolean");
            }
            body = transStm(venv, tenv, stm->u.whilee.body, level, isLoop, classs);
            return expTy(Tr_WhileStm(stm->pos, transExp(venv, tenv, stm->u.whilee.test, level, isLoop, classs).exp->u.exp,
                body.exp->u.stm),
                Ty_Void());
        }
        case A_forStm:
        {
            //debug(stm->pos, "For Statement");
            A_stm assignStm = stm->u.forr.assign;
            struct expty assign = transStm(venv, tenv, assignStm, level, isLoop, classs);
            struct expty condition = transExp(venv, tenv, stm->u.forr.condition, level, isLoop, classs);
            if (condition.ty != Ty_Bool())
            {
                EM_error(stm->pos, "for.boolean");
            }
            struct expty increment = transStm(venv, tenv, stm->u.forr.increment, level, isLoop, classs);
            struct expty body = transStm(venv, tenv, stm->u.forr.body, level, isLoop, classs);
            // E_enventry assignVar = S_look(venv, assignExp -> u.assign.var -> u.simple);
            return expTy(Tr_ForStm(stm->pos, assign.exp->u.stm,
                condition.exp->u.exp, increment.exp->u.stm, body.exp->u.stm),
                Ty_Void());
        }
        case A_breakStm:
        {
            //debug(stm->pos, "Break Statement");
            return expTy(Tr_BreakStm(stm->pos, 1), Ty_Void());
        }
        case A_continueStm:
        {
            //debug(stm->pos, "Continue Statement");
            return expTy(Tr_ContinueStm(stm->pos), Ty_Void());
        }
        case A_loopStm:
        {
            return transStm(venv, tenv, A_DeclarationStm(stm -> pos, A_FunctionDec(stm -> pos, A_FundecList(A_Fundec(stm -> pos, S_Symbol("loop"), A_FieldList(NULL, NULL), A_FieldList(NULL, NULL), stm -> u.loop.body), NULL))), level, isLoop, classs);
            // return expTy(Tr_LoopStm(stm->pos), Ty_Void());
        }
        case A_callStm:
        {
            T_expList expList = T_ExpList(NULL, NULL);
            T_expList result = expList;
            bool isCalledInMethod = FALSE;
            //debug(stm->pos, "Call Statement");
            
            if(stm -> u.call.func -> kind == A_varExp || stm -> u.call.func -> kind == A_fieldExp || stm -> u.call.func -> kind == A_arrowFieldExp){
                S_symbol name = NULL;
                E_enventry entry = NULL;
                if(stm -> u.call.func -> kind == A_varExp){
                    A_var var = stm -> u.call.func -> u.var;
                    if(var -> kind == A_simpleVar){
                        name = var -> u.simple;
                        entry = (E_enventry)S_look(venv, name);
                        if(entry == NULL){
                            E_enventry classEntry = NULL;
                            if(classs -> kind == Ty_name){
                                classEntry = (E_enventry)S_look(tenv, classs -> u.name.sym);
                            }
                            if(classEntry){
                                isCalledInMethod = TRUE;
                                entry = (E_enventry)S_look(classEntry -> u.classs.methods, var -> u.simple);
                            }
                        }
                    }
                    else if(var -> kind == A_fieldVar || var -> kind == A_arrowFieldVar){
                        struct expty field;
                        Ty_ty type = NULL;
                        if(var -> kind == A_fieldVar){
                            field = transVar(venv, tenv, var->u.field.var, level, isLoop, FALSE, FALSE, classs);
                            type = field.ty;
                            name = var -> u.field.sym;
                        }
                        else if(var -> kind == A_arrowFieldVar){
                            field = transVar(venv, tenv, var->u.arrowfield.pointer, level, isLoop, FALSE, FALSE, classs);
                            if(field.ty -> kind != Ty_pointer){
                                EM_error(var -> pos, "operator.arrow");
                            }
                            type = field.ty -> u.pointer;
                            name = var -> u.arrowfield.member;
                        }
                        if(type -> kind == Ty_name || type -> kind == Ty_poly)
                        {
                            E_enventry classEntry = NULL;
                            if(type -> kind == Ty_name)
                                classEntry = (E_enventry)S_look(tenv, type -> u.name.sym);
                            else if(type -> kind == Ty_poly)
                            {
                                E_enventry templateEntry = (E_enventry)S_look(tenv, type -> u.poly.name);
                                name = var -> u.field.sym;
                                if(templateEntry && templateEntry -> kind == E_templateentry)
                                {
                                    classEntry = (E_enventry)TAB_search(templateEntry -> u.templatee.specific, type -> u.poly.typeParam);
                                    // printf("%d fdjaslkdfjasldfjilasjfilasdjfioajd\n", type -> u.poly.typeParam -> kind);
                                }
                                else
                                {
                                    EM_error(stm -> pos, "noexist.template %s", S_name(type -> u.poly.name));
                                }
                            }
                            
                            if(classEntry && classEntry -> kind == E_classentry){
                                // printf("var -> u.field.sym %s\n", S_name(var -> u.field.sym));
                                entry = (E_enventry)S_look(classEntry -> u.classs.methods, var -> u.field.sym);
                            }
                            else if(classEntry && classEntry -> kind == E_polyentry){
                                if(classEntry -> u.poly -> kind == Ty_name)
                                {
                                    E_enventry classEntry = (E_enventry)S_look(tenv, type -> u.name.sym);
                                    if(classEntry && classEntry -> kind == E_classentry)
                                        entry = (E_enventry)S_look(classEntry -> u.classs.methods, var -> u.field.sym);
                                    else
                                        EM_error(stm -> pos, "noexist.class %s", S_name(type -> u.name.sym));
                                }
                                else
                                {
                                    EM_error(stm -> pos, "type.notclass %s", S_name(type -> u.name.sym));
                                }
                            }
                            else{
                                EM_error(stm -> pos, "noexist.class %s", S_name(type -> u.name.sym));
                            }
                        }
                        else
                        {
                            EM_error(stm -> pos, "type.notclass %s", S_name(name));
                        }
                    }
                }
                else if(stm -> u.call.func -> kind == A_fieldExp || stm -> u.call.func -> kind == A_arrowFieldExp){
                    struct expty fieldExpty;
                    Ty_ty type = NULL;
                    if(stm -> u.call.func -> kind == A_fieldExp){
                        name = stm -> u.call.func -> u.field.member;
                        fieldExpty = transExp(venv, tenv, stm -> u.call.func -> u.field.field, level, isLoop, classs);
                        type = fieldExpty.ty;
                    }
                    else if(stm -> u.call.func -> kind == A_arrowFieldExp){
                        name = stm -> u.call.func -> u.arrowfield.member;
                        fieldExpty = transExp(venv, tenv, stm -> u.call.func -> u.arrowfield.pointer, level, isLoop, classs);
                        if(fieldExpty.ty -> kind != Ty_pointer){
                                // printf("aaaaaaaaaaaaaaaaaaa %s\n", S_name(fieldExpty.ty -> u.name.sym));

                            EM_error(stm -> pos, "operator.arrow");
                        }
                        type = fieldExpty.ty -> u.pointer;
                    }
                        
                    if(type -> kind == Ty_name || type -> kind == Ty_poly)
                    {
                        E_enventry classEntry = NULL;
                        if(type -> kind == Ty_name)
                        {
                            classEntry = (E_enventry)S_look(tenv, type -> u.name.sym);
                        }
                        else if(type -> kind == Ty_poly)
                        {
                            E_enventry templateEntry = (E_enventry)S_look(tenv, type -> u.poly.name);
                            if(templateEntry && templateEntry -> kind == E_templateentry)
                            {
                                classEntry = (E_enventry)TAB_search(templateEntry -> u.templatee.specific, type -> u.poly.typeParam);
                            }
                            else
                            {
                                EM_error(stm -> pos, "noexist.template %s", S_name(type -> u.poly.name));
                            }
                        }
                        if(classEntry && classEntry -> kind == E_classentry){
                            if(stm -> u.call.func -> kind == A_fieldExp)
                                entry = (E_enventry)S_look(classEntry -> u.classs.methods, name);
                            else if(stm -> u.call.func -> kind == A_arrowFieldExp)
                                entry = (E_enventry)S_look(classEntry -> u.classs.methods, name);
                        }
                        else{
                            EM_error(stm -> pos, "noexist.class %s", S_name(type -> u.name.sym));
                        }
                    }
                    else
                    {
                        EM_error(stm -> pos, "noexist.member %s", S_name(name));
                    }
                }
                if(entry != NULL && entry -> kind == E_listEntry && entry -> u.list -> size() > 0){
                    for(const auto &entries: *(entry -> u.list)){
                        if(entries -> kind == E_funcentry){
                            Ty_tyList paramType = entries -> u.func.formals;
                            A_expList args = stm -> u.call.args;
                            bool functionOk = true;
                            for(;args && paramType; paramType = paramType -> tail, args = args -> tail){
                                if (args -> head == NULL && paramType -> head != NULL)
                                {
                                    EM_error(stm->pos, "func.few");
                                    break;
                                }
                                else if (args -> head != NULL && paramType -> head == NULL)
                                {
                                    EM_error(stm->pos, "func.many");
                                    break;
                                }
                                Ty_ty argType = transExp(venv, tenv, args -> head, level, isLoop, classs).ty;
                                if(compType(paramType -> head, argType)){
                                    continue;
                                }
                                else if(args -> head -> kind == A_stringExp && paramType -> head -> kind == Ty_name){
                                    if(strcmp(_stringClassName.c_str(), S_name(paramType -> head -> u.name.sym)) == 0){
                                        continue;
                                    }
                                }
                                else{
                                    functionOk = false;
                                    break;
                                }
                            }
                            if(functionOk){
                                entry = entries;
                                break;
                            }
                            else{
                                entry = NULL;
                            }
                        }
                    }
                }
                if (entry != NULL && (entry->kind == E_funcentry || (entry->kind == E_templateentry && entry -> u.templatee.dec -> kind == A_functionDec)))
                {
                    E_enventry templateTemp = (E_enventry)checked_malloc(sizeof(*templateTemp));
                    templateTemp = NULL;
                    A_expList expArgs = NULL;
                    Ty_tyList entryArgsTy = NULL;
                    if(entry -> kind == E_templateentry)
                    {
                        templateTemp = entry;
                        A_fundec fundec = templateTemp -> u.templatee.dec -> u.function -> head;
                        Ty_ty type = Ty_Void();
                        entryArgsTy = makeParamTypeList(venv, tenv, fundec -> params, level);
                        for(A_expList argList = stm -> u.call.args; entryArgsTy && argList; entryArgsTy = entryArgsTy -> tail, argList = argList -> tail)
                        {
                            if (argList -> head == NULL && entryArgsTy -> head != NULL)
                            {
                                EM_error(stm->pos, "func.few");
                                break;
                            }
                            else if (argList -> head != NULL && entryArgsTy -> head == NULL)
                            {
                                EM_error(stm->pos, "func.many");
                                break;
                            }
                            if(entryArgsTy -> head -> kind == Ty_name)
                            {
                              //printf("%s templatee.name\n", S_name(entry -> u.templatee.name));
                                if(entryArgsTy -> head -> u.name.sym == entry -> u.templatee.name)
                                {
                                    struct expty arg = transExp(venv, tenv, argList -> head, level , isLoop, classs);
                                  //printf("%d type kind\n", arg.ty -> kind);
                                    entry = (E_enventry)TAB_search(entry -> u.templatee.specific, arg.ty);
                                    if(entry){
                                        break;
                                    }
                                    S_enter(tenv, templateTemp -> u.templatee.name, E_PolyEntry(arg.ty));
                                    type = arg.ty;
                                    break;
                                }
                            }
                        }
                        T_module dec = transDec(venv, tenv, templateTemp -> u.templatee.dec, level, isLoop, venv, classs);
                        list -> head = dec;
                        list -> tail = T_ModuleList(NULL, NULL);
                        list = list -> tail;
                        list -> head = NULL;
                        list -> tail = NULL;
                        entry = (E_enventry)S_look(venv, name);
                        TAB_insert(templateTemp -> u.templatee.specific, type, entry);
                    }
                    int i = 1;
                    for (i = 1, expArgs = stm->u.call.args, entryArgsTy = entry->u.func.formals; expArgs != NULL && entryArgsTy != NULL;
                        expArgs = expArgs->tail, entryArgsTy = entryArgsTy->tail, i++)
                    {
                        A_exp exp = expArgs->head;
                        Ty_ty type = entryArgsTy->head;

                        if (exp == NULL && type != NULL)
                        {
                            EM_error(stm->pos, "func.few");
                            break;
                        }
                        else if (exp != NULL && type == NULL)
                        {
                            EM_error(stm->pos, "func.many");
                            break;
                        }

                        struct expty expp = transExp(venv, tenv, exp, level, isLoop, classs);
                        T_exp translatedAddrExp = expp.exp->u.exp -> u.load.addr;
                        
                        if(exp -> kind == A_stringExp && type -> kind == Ty_name ){
                            if(type -> kind == Ty_name){
                                stringLiterals -> head = transStm(venv, tenv, A_DeclarationStm(exp -> pos, A_VarDec(exp -> pos, A_AssignStm(exp -> pos, A_SimpleVar(exp -> pos, S_Symbol("__string_literal")), exp, TRUE), A_NameTy(exp -> pos, type -> u.name.sym))), level, isLoop, classs).exp -> u.stm;
                                stringLiterals -> tail = T_StmList(NULL, NULL);
                                stringLiterals = stringLiterals -> tail;
                                stringLiterals -> head = NULL;
                                stringLiterals -> tail = NULL;
                            }
                            exp = A_VarExp(exp -> pos, A_SimpleVar(exp -> pos, S_Symbol("__string_literal")));
                            expp = transExp(venv, tenv, exp, level, isLoop, classs);
                        }
                        if(expp.ty -> kind == Ty_array && expp.exp -> u.exp -> kind == T_loadExp){
                            expList->head = translatedAddrExp;
                        }
                        else{
                            expList->head = expp.exp->u.exp;
                        }
                        // printf("%d expList -> head\n", expp.ty -> kind );
                        expList->tail = T_ExpList(NULL, NULL);

                        if (exp != NULL && type != NULL)
                        {
                          //printf("argType.ty -> kind %d\n", type -> kind);
                            if (!compType(type, expp.ty))
                            {
                                expList -> head = convertAssign(type, expp, exp -> pos);
                            }
                        }
                        expList = expList->tail;

                        expList->head = NULL;
                        expList->tail = NULL;
                    }
                    if(stm -> u.call.func -> kind == A_varExp && stm -> u.call.func -> u.var -> kind == A_fieldVar){
                        struct expty field = transVar(venv, tenv, stm -> u.call.func -> u.var->u.field.var, level, isLoop, FALSE, FALSE, classs);
                        result = T_ExpList(field.exp -> u.exp, result);
                    }
                    if(stm -> u.call.func -> kind == A_varExp && stm -> u.call.func -> u.var -> kind == A_arrowFieldVar){
                        struct expty field = transVar(venv, tenv, stm -> u.call.func -> u.var->u.arrowfield.pointer, level, isLoop, FALSE, FALSE, classs);
                        result = T_ExpList(field.exp -> u.exp, result);
                    }
                    if(isCalledInMethod){
                        result = T_ExpList(T_GetLocalExp(T_i32, 0), result);
                    }
                    if(stm -> u.call.func -> kind == A_fieldExp)
                    {
                        struct expty field = transExp(venv, tenv, stm -> u.call.func -> u.field.field, level, isLoop, classs);
                        result = T_ExpList(field.exp -> u.exp, result);
                    }
                    if(stm -> u.call.func -> kind == A_arrowFieldExp)
                    {
                        struct expty field = transExp(venv, tenv, stm -> u.call.func -> u.arrowfield.pointer, level, isLoop, classs);
                        result = T_ExpList(field.exp -> u.exp, result);
                    }
                    if (expArgs == NULL && entryArgsTy != NULL)
                    {
                        if (entryArgsTy->head != NULL)
                        {
                            EM_error(stm->pos, "func.few");
                        }
                    }
                    else if (expArgs != NULL && entryArgsTy == NULL)
                    {
                        if (expArgs->head != NULL)
                        {
                            EM_error(stm->pos, "func.many");
                        }
                    }
                    if(templateTemp)
                    {
                        S_enter(venv, name, templateTemp);
                    }
                    if (entry->u.func.result)
                        return expTy(Tr_CallStm(stm->pos, entry->u.func.index, S_name(entry->u.func.label), result), entry->u.func.returnType);
                    return expTy(Tr_CallStm(stm->pos, entry->u.func.index, S_name(entry->u.func.label), result), Ty_Void());
                }
                else if(entry != NULL && entry -> kind == E_varentry && entry -> u.var.ty -> kind == Ty_func)
                {
                    A_expList expArgs = NULL;
                    Ty_tyList entryArgsTy = NULL;
                    int i;
                    for (i = 1, expArgs = stm->u.call.args, entryArgsTy = entry->u.var.ty -> u.func.params; expArgs != NULL && entryArgsTy != NULL;
                        expArgs = expArgs->tail, entryArgsTy = entryArgsTy->tail, i++)
                    {
                        A_exp exp = expArgs->head;
                        struct expty expp = transExp(venv, tenv, exp, level, isLoop, classs);
                        if(expp.ty -> kind == Ty_array && expp.exp -> u.exp -> kind == T_loadExp){   
                            expList->head = expp.exp->u.exp -> u.load.addr;
                        }
                        else{
                            expList->head = expp.exp->u.exp;
                        }
                        // printf("%d expList -> head\n", expp.ty -> kind );
                        expList->tail = T_ExpList(NULL, NULL);
                        Ty_ty type = entryArgsTy->head;

                        if (exp != NULL && type != NULL)
                        {
                            struct expty argType = transExp(venv, tenv, exp, level, isLoop, classs);
                          //printf("argType.ty -> kind %d\n", type -> kind);
                            if (!compType(type, argType.ty))
                            {
                                expList -> head = convertAssign(type, argType, exp -> pos);
                            }

                        }
                        else
                        {
                            if (exp == NULL && type != NULL)
                            {
                                EM_error(stm->pos, "func.few");
                                break;
                            }
                            else if (exp != NULL && type == NULL)
                            {
                                EM_error(stm->pos, "func.many");
                                break;
                            }
                        }
                        expList = expList->tail;

                        expList->head = NULL;
                        expList->tail = NULL;
                    }
                    return expTy(Tr_CallIndirectStm(stm -> pos, Tr_VarExp(stm -> pos, T_i32, entry -> u.var.access, FALSE, FALSE) -> u.exp, result, entry -> u.var.ty -> u.func.typeIndex), entry -> u.var.ty -> u.func.result);
                }
                else
                {
                    // printf("%d entry -> kind", entry -> kind);
                    EM_error(stm->pos, "noexist.func %s", S_name(name));
                }
            }
            else
                EM_error(stm -> pos, "func.cannotcall");
        }
        case A_returnStm:
        {
            //debug(stm->pos, "Return Statement");
            if (stm->u.returnn.ret){
                struct expty returnExp = transExp(venv, tenv, stm->u.returnn.ret, level, isLoop, classs);
                if(returnExp.ty -> kind == Ty_name || returnExp.ty -> kind == Ty_poly || returnExp.ty -> kind == Ty_array)
                {
                    if(returnExp.exp -> u.exp -> kind == T_loadExp)
                        return expTy(Tr_ReturnStm(stm->pos, returnExp.exp -> u.exp -> u.load.addr), Ty_Void());
                    else
                        return expTy(Tr_ReturnStm(stm -> pos, returnExp.exp -> u.exp), Ty_Void());
                }
                return expTy(Tr_ReturnStm(stm->pos, returnExp.exp -> u.exp), Ty_Void());
            }
            else
            {
                assert(level);
                Temp_label label = level->label;
                E_enventry func = (E_enventry)S_look(venv, label);
                // printf("%s\n", S_name(label));
                if (func->kind == E_funcentry)
                    if (func->u.func.result)
                        return expTy(Tr_ReturnStm(stm->pos, Tr_VarExp(stm->pos, convertType(func->u.func.result->access->type), func->u.func.result, FALSE, FALSE)->u.exp), Ty_Void());
                    else
                        return expTy(Tr_ReturnStm(stm->pos, NULL), Ty_Void());
            }
        }
        }
}

struct expty transVar(S_table venv, S_table tenv, A_var v, Tr_level level, bool isLoop, bool reverse, bool isDec, Ty_ty classs)
{
    assert(v);
    static int depth = 0;
    switch (v->kind)
    {
    case A_simpleVar:
    {
        // printf("%s var name\n", S_name(v -> u.simple));
        E_enventry x = (E_enventry)S_look(venv, v->u.simple);
        if (x && x->kind == E_varentry)
        {
            // printf("%d type name\n", x -> u.var.ty -> kind);
            if (x->u.var.ty->kind == Ty_array || x -> u.var.ty -> kind == Ty_name || x -> u.var.ty -> kind == Ty_poly)
            {
                // printf("7111111111111114\n");
                int arrayAddr = x->u.var.access->level->frame->offset + x->u.var.access->access->u.offset;
                // printf("================================%s %d 79999999999999999999999999999999999\n", S_name(v->u.simple), arrayAddr);
                return expTy(Tr_AddrExp(v -> pos, arrayAddr), x -> u.var.ty);
                // return expTy(Tr_VarExp(v->pos, convertType(x -> u.var.ty), x -> u.var.access, FALSE, TRUE), x->u.var.ty);
            }
            else
            {
                if (!isLoop)
                {
                    if(classs != Ty_Void()){
                        if(v -> u.simple == S_Symbol("this")){
                            Tr_exp exp = (Tr_exp)checked_malloc(sizeof(*exp));
                            exp->kind = Tr_t_exp;
                            exp->u.exp = T_GetLocalExp(T_i32, 0);
                            return expTy(exp, Ty_Pointer(classs));
                        }
                    }
                    if (x->u.var.access->level->parent)
                        return expTy(Tr_VarExp(v->pos, convertType(x->u.var.ty), x->u.var.access, FALSE, FALSE), x->u.var.ty);
                    else
                        return expTy(Tr_VarExp(v->pos, convertType(x->u.var.ty), x->u.var.access, TRUE, FALSE), x->u.var.ty);
                }
                else
                {
                    if(strcmp(S_name(x -> u.var.access -> level -> label), "loop") != 0)
                    {
                        if(x -> u.var.access -> access -> kind != inFrame)
                        {
                            int arrayAddr = x->u.var.access->level->frame->offset + x->u.var.access->level -> frame -> frameSize;
                          //printf("not inframe %d\n", arrayAddr);
                            loopVarsEnd -> head = Tr_AddrAssignStm(v -> pos, Tr_AddrExp(v -> pos, arrayAddr)->u.exp, Tr_VarExp(v->pos, convertType(x->u.var.ty), x->u.var.access, FALSE, FALSE)->u.exp)->u.stm;
                            loopVarsEnd -> tail = T_StmList(NULL, NULL);
                            loopVarsEnd = loopVarsEnd -> tail;
                            toInFrame(x->u.var.access->access, x->u.var.access->level->frame);
                        }
                        //debug(v -> pos, "%s %s %d var type", S_name(x -> u.var.access -> level -> label), S_name(v -> u.simple), x -> u.var.access -> access -> kind);
                        return expTy(Tr_DerefExp(v->pos, Tr_AddrExp(v -> pos, x -> u.var.access -> access -> u.offset + x -> u.var.access -> level -> frame -> offset)->u.exp, convertType(x->u.var.access->access->type)), x->u.var.access->access->type);
                    }
                    else
                    {
                        if (x->u.var.access->level->parent)
                            return expTy(Tr_VarExp(v->pos, convertType(x->u.var.ty), x->u.var.access, FALSE, FALSE), actual_ty(x->u.var.ty));
                        else
                            return expTy(Tr_VarExp(v->pos, convertType(x->u.var.ty), x->u.var.access, TRUE, FALSE), actual_ty(x->u.var.ty));
                    }
                    ////////asssiiiiiiiiiiiign to memorryyyyyyyy
                    
                }
            }
        }
        else if (!x)
        {
            if(classs == Ty_Void())
            {
                EM_error(v->pos, "noexist.var %s", S_name(v -> u.simple));
            }
            if(classs -> kind == Ty_name || classs -> kind == Ty_poly){

                E_enventry classEntry = NULL;
                if(classs -> kind == Ty_name){
                    classEntry = (E_enventry)S_look(tenv, classs->u.name.sym);
                }
                else if(classs -> kind == Ty_poly){
                    E_enventry templateEntry = (E_enventry)S_look(tenv, classs -> u.poly.name);
                    if(templateEntry && templateEntry -> kind == E_templateentry)
                    {
                        classEntry = (E_enventry)TAB_search(templateEntry -> u.templatee.specific, classs -> u.poly.typeParam);
                    }
                    else
                    {
                        // printf("aaaaaaaaaaaaaaaaaa %d\n", templateEntry -> kind);
                        EM_error(v -> pos, "noexist.template %s", S_name(classs -> u.poly.name));
                    }
                }
                // printf("%d classs name\n", classs -> kind);
                if(classEntry){
                    // printf("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaa %d\n", classEntry -> u.classs.size);
                    Ty_member member = (Ty_member)S_look(classEntry -> u.classs.varTypes, v -> u.simple);
                    if(member)
                    {
                        if(v -> lvalue || member -> ty -> kind == Ty_array || member -> ty -> kind == Ty_poly || member -> ty -> kind == Ty_name){
                            return expTy(Tr_OpExp(v -> pos, T_i32, T_add, T_GetLocalExp(T_i32, 0), Tr_AddrExp(v -> pos, member -> offset)->u.exp), member -> ty);
                        }
                        return expTy(Tr_DerefExp(v -> pos, Tr_OpExp(v -> pos, T_i32, T_add, T_GetLocalExp(T_i32, 0), Tr_AddrExp(v -> pos, member -> offset)->u.exp)->u.exp, convertType(member -> ty)), member -> ty);
                    }
                    
                    if(v -> u.simple == S_Symbol("this"))
                    {
                        return transVar(venv, tenv, v, level, isLoop, FALSE, FALSE, classs);
                    }
                }
                EM_error(v->pos, "noexist.var %s", S_name(v -> u.simple));
            }
        }
        else if (x->kind != E_varentry)
        {
            EM_error(v->pos, "noexist.var %s", S_name(v -> u.simple));
        }
        else
        {
            EM_error(v->pos, "noexist.var %s", S_name(v->u.simple));
            return expTy(NULL, Ty_Int());
        }
        break;
    }
    case A_fieldVar:
    {
        struct expty varExpty = transVar(venv, tenv, v -> u.field.var, level, isLoop, reverse, FALSE, classs);
        E_enventry classEntry = NULL;
        if(varExpty.ty -> kind == Ty_name || varExpty.ty -> kind == Ty_poly){
            if(varExpty.ty -> kind == Ty_name){
                classEntry = (E_enventry)S_look(tenv, varExpty.ty -> u.name.sym);
                if(classEntry -> kind == E_polyentry)
                {
                    if(classEntry -> u.poly -> kind != Ty_name)
                        EM_error(v -> pos, "type.notclass %s", S_name(getName(v)));
                    else
                        classEntry = (E_enventry)S_look(tenv, varExpty.ty -> u.name.sym);
                }
            }
            else if(varExpty.ty -> kind == Ty_poly){
                E_enventry templateEntry = (E_enventry)S_look(tenv, varExpty.ty -> u.poly.name);
                if(templateEntry && templateEntry -> kind == E_templateentry)
                {
                    classEntry = (E_enventry)TAB_search(templateEntry -> u.templatee.specific, varExpty.ty -> u.poly.typeParam);
                }
                else
                {
                    EM_error(v -> pos, "noexist.template %s", S_name(varExpty.ty -> u.poly.name));
                }
            }
        }
        else
            EM_error(v -> pos, "type.notclass %s", S_name(getName(v)));
        Ty_member varEntry = (Ty_member)S_look(classEntry -> u.classs.varTypes, v -> u.field.sym);
        if(varEntry){
            if(varEntry -> accessSpecifier == Ty_private){
                EM_error(v -> pos, "class.private %s", S_name(v -> u.field.sym));
            }
            else if(varEntry -> accessSpecifier == Ty_protected){
                EM_error(v -> pos, "class.protected %s", S_name(v -> u.field.sym));
            }
            // printf("8633333333333333333333333333333333333 %d\n", varExpty.exp -> u.exp -> kind);
            // if(varExpty.ty ->)
            if(varEntry -> ty -> kind == Ty_array || varEntry -> ty -> kind == Ty_name || varEntry -> ty -> kind == Ty_poly)
                return expTy(Tr_OpExp(v -> pos, T_i32, T_add, varExpty.exp -> u.exp, Tr_AddrExp(v -> pos, varEntry -> offset)->u.exp), varEntry -> ty);
            return expTy(Tr_DerefExp(v -> pos, Tr_OpExp(v -> pos, T_i32, T_add, varExpty.exp -> u.exp, Tr_AddrExp(v -> pos, varEntry -> offset)->u.exp)->u.exp, convertType(varEntry -> ty)), varEntry -> ty);
        }
        else{
            EM_error(v -> pos, "class.nomember %s %s", S_name(varExpty.ty -> u.name.sym), S_name(v -> u.field.sym));
        }
    }
    case A_arrowFieldVar:
    {
        struct expty varExpty = transVar(venv, tenv, v -> u.arrowfield.pointer, level, isLoop, reverse, FALSE, classs);
        E_enventry classEntry = NULL;
        if(varExpty.ty -> kind == Ty_pointer){
            if(varExpty.ty -> u.pointer -> kind == Ty_name || varExpty.ty -> u.pointer -> kind == Ty_poly){
                if(varExpty.ty -> u.pointer -> kind == Ty_name){
                    classEntry = (E_enventry)S_look(tenv, varExpty.ty -> u.pointer -> u.name.sym);
                    if(classEntry -> kind == E_polyentry)
                    {
                        if(classEntry -> u.poly -> kind != Ty_name)
                            EM_error(v -> pos, "type.notclass %s", S_name(getName(v -> u.arrowfield.pointer)));
                        else
                            classEntry = (E_enventry)S_look(tenv, varExpty.ty -> u.pointer -> u.name.sym);
                    }
                }
                else if(varExpty.ty -> u.pointer -> kind == Ty_poly){
                    E_enventry templateEntry = (E_enventry)S_look(tenv, varExpty.ty -> u.pointer -> u.poly.name);
                    if(templateEntry && templateEntry -> kind == E_templateentry)
                    {
                        classEntry = (E_enventry)TAB_search(templateEntry -> u.templatee.specific, varExpty.ty -> u.pointer -> u.poly.typeParam);
                    }
                    else
                    {
                        EM_error(v -> pos, "noexist.template %s", S_name(varExpty.ty -> u.pointer -> u.poly.name));
                    }
                }
            }
            else{
                EM_error(v -> pos, "type.notclass %s", S_name(getName(v -> u.arrowfield.pointer)));
            }
            Ty_member varEntry = (Ty_member)S_look(classEntry -> u.classs.varTypes, v -> u.field.sym);
            if(varEntry){
                if(varEntry -> accessSpecifier == Ty_private){
                    EM_error(v -> pos, "class.private %s", S_name(v -> u.field.sym));
                }
                else if(varEntry -> accessSpecifier == Ty_protected){
                    EM_error(v -> pos, "class.protected %s", S_name(v -> u.field.sym));
                }
                // printf("8633333333333333333333333333333333333 %d %s\n", varEntry -> ty -> kind, S_name(v -> u.field.sym));
                if(varEntry -> ty -> kind == Ty_array || varEntry -> ty -> kind == Ty_name || varEntry -> ty -> kind == Ty_poly)
                    return expTy(Tr_OpExp(v -> pos, T_i32, T_add, varExpty.exp -> u.exp, Tr_AddrExp(v -> pos, varEntry -> offset)->u.exp), varEntry -> ty);
                return expTy(Tr_DerefExp(v -> pos, Tr_OpExp(v -> pos, T_i32, T_add, varExpty.exp -> u.exp, Tr_AddrExp(v -> pos, varEntry -> offset)->u.exp)->u.exp, convertType(varEntry -> ty)), varEntry -> ty);
            }
            else{
                EM_error(v -> pos, "class.nomember %s %s", S_name(varExpty.ty -> u.pointer -> u.name.sym), S_name(v -> u.field.sym));
            }
        }
        else
        {
            EM_error(v -> pos, "operator.arrow");
        }
    }
    case A_subscriptVar:
    {
        if(depth == 0)
            v = reverseSubVar(v);
        // printf("%s name\n", S_name(v -> u.subscript.name));
        struct expty varExpty;
        if(v -> u.subscript.var -> kind== A_simpleVar){
            varExpty = transVar(venv, tenv, v -> u.subscript.var, level, isLoop, reverse, isDec, classs);
        }
        else if(v -> u.subscript.var -> kind == A_subscriptVar){
            // printf("hello\n");
            varExpty = transVar(venv, tenv, A_SimpleVar(v -> pos, v -> u.subscript.name), level, isLoop, reverse, isDec, classs);
        }
        else if(v -> u.subscript.var -> kind == A_fieldVar){
            S_symbol arrayName = getName(v -> u.subscript.var);
            varExpty = transVar(venv, tenv, v -> u.subscript.var -> u.field.var, level, isLoop, reverse, isDec, classs);
            // printf("%s array name\n", S_name(arrayName));
            varExpty = transVar(venv, tenv, A_SimpleVar(v -> pos, arrayName), level, isLoop, reverse, isDec, varExpty.ty);
        }
        if(isDec)
        {
            E_enventry entry = (E_enventry)S_look(venv, v->u.subscript.name);
            int addr = entry -> u.var.access -> level -> frame -> offset + entry -> u.var.access -> access -> u.offset;
            return expTy(Tr_AddrExp(v -> pos, addr), entry -> u.var.ty);
        }
        struct expty exp = transExp(venv, tenv, v->u.subscript.exp, level, isLoop, classs);
        // printf("%d index 600\n", v->u.subscript.exp -> u.intt);
        if (varExpty.exp)
        {
            // printf("632 %d\n", entry -> u.var.ty -> kind);
            if (varExpty.ty->kind == Ty_array)
            {
                // printf("test test tes %d %d\n", v -> u.subscript.depth, v -> u.subscript.exp -> u.intt);
                // printf("%d subscript type\n", exp.exp -> u.exp -> kind);
                Ty_ty type = varExpty.ty;
                for (int i = 0; i < depth + 1; i++)
                {
                    type = type->u.array.type;
                }
                // printf("type %d\n", type -> kind);
                depth++;
                struct expty var = transVar(venv, tenv, v->u.subscript.var, level, isLoop, FALSE, FALSE, classs);
                T_exp addr = NULL;
                if(v -> u.subscript.var -> kind == A_fieldVar && var.exp -> u.exp -> kind == T_loadExp){
                    addr = var.exp ->u.exp -> u.load.addr;
                }
                else{
                    addr = var.exp -> u.exp;
                }
                depth = 0;
                // return expTy(Tr_OpExp(v->pos, T_i32, T_add, addr, T_BinOpExp(T_i32, T_mul, T_ConstExp(T_i32, A_IntExp(v->pos, type->size)), T_ConvertExp(T_i32, exp.exp->u.exp))), Ty_Array(var.ty, type->u.array.size));
                return expTy(Tr_OpExp(v->pos, T_i32, T_add, addr, T_BinOpExp(T_i32, T_mul, T_ConstExp(T_i32, A_IntExp(v->pos, type->size)), T_ConvertExp(T_i32, exp.exp->u.exp))), var.ty -> u.array.type);
            }
            else if(varExpty.ty -> kind== Ty_pointer)
            {
                Ty_ty type = varExpty.ty;
                for(int i = 0; i < depth + 1; i++)
                {
                    type = type -> u.pointer;
                }
                depth++;
                struct expty var = transVar(venv, tenv, v -> u.subscript.var, level, isLoop, FALSE, FALSE, classs);
                depth = 0;
                return expTy(Tr_OpExp(v->pos, T_i32, T_add, var.exp->u.exp, T_BinOpExp(T_i32, T_mul, T_ConstExp(T_i32, A_IntExp(v->pos, type->size)), T_ConvertExp(T_i32, exp.exp->u.exp))), varExpty.ty -> u.pointer);

            }
            else
            {
                // printf("632 %d\n", entry -> u.var.ty -> kind);
                EM_error(v->pos, "type.notarray %s", S_name(getName(v)));
            }
        }
        else
        {
            EM_error(v->pos, "noexist.var %s", S_name(v->u.subscript.name));
        }
    }
    case A_derefVar:
    {
        E_enventry entry = (E_enventry)S_look(venv, v->u.deref->u.simple);
        if(entry){
            // printf("aaaaaaaaaaaa %s\n", S_name(v -> u.deref -> u.simple));
            return expTy(Tr_VarExp(v -> pos, T_i32, entry->u.var.access,
                (entry->u.var.access->access->kind == inGlobal), FALSE),
                entry->u.var.access->access->type -> u.pointer);
        }
    }
    }
}

struct expty transExp(S_table venv, S_table tenv, A_exp e, Tr_level level, bool isLoop, Ty_ty classs)
{
    // assert(e);
    switch (e->kind)
    {
    case A_typeEqExp:
    {
        Ty_ty type1 = transTy(venv, tenv, e -> u.typeeq.type1, level);
        Ty_ty type2 = transTy(venv, tenv, e -> u.typeeq.type2, level);
        return expTy(Tr_BoolExp(e -> pos, compType(type1, type2)), Ty_Bool());
    }
    case A_addrExp:
    {
        return expTy(Tr_AddrExp(e -> pos, e -> u.addr), Ty_Pointer(Ty_Void()));
        break;
    }
    case A_intExp:
    {
        return expTy(Tr_IntExp(e->pos, e->u.intt), Ty_Int());
        break;
    }
    case A_stringExp:
    {
        // printf("%s %d\n", e -> u.stringg, strlen(e -> u.stringg));
        T_expList list = T_ExpList(NULL, NULL);
        T_expList result = list;
        int size = 0;
        for(int i = 0; i < strlen(e -> u.stringg);)
        {
            char moji[4];
            if(e -> u.stringg[i] < 0)
            {
                strncpy(moji, e -> u.stringg + i, 3);
                i+=3;
                moji[3]='\0';
            }
            else
            {
                strncpy(moji, e -> u.stringg + i, 1);
                i++;
                moji[1]='\0';
                // printf("1size");
            }
            // printf("jjjj %d\n", strlen(moji));
            list -> head = Tr_CharExp(e -> pos, moji) -> u.exp;
            list -> tail = T_ExpList(NULL, NULL);
            list = list -> tail;
            list -> head = NULL;
            list -> tail = NULL;
            size++;
        }
        // printf("%d size string\n", size);
        return expTy(Tr_ArrayExp(e -> pos, result), Ty_Pointer(Ty_Array(Ty_Char(), size)));
        break;
    }
    case A_realExp:
    {
        return expTy(Tr_RealExp(e->pos, e->u.real), Ty_Real());
        break;
    }
    case A_boolExp:
    {
        return expTy(Tr_BoolExp(e->pos, e->u.booll), Ty_Bool());
        break;
    }
    case A_nilExp:
    {
        return expTy(Tr_NilExp(e->pos), Ty_Nil());
        break;
    }
    case A_charExp:
    {
        return expTy(Tr_CharExp(e->pos, e->u.charr), Ty_Char());
    }
    case A_varExp:
    {
        if (e->u.var->kind == A_simpleVar)
            return transVar(venv, tenv, e->u.var, level, isLoop, FALSE, FALSE, classs);
        else if (e->u.var->kind == A_derefVar)
        {
            struct expty var = transVar(venv, tenv, e->u.var, level, isLoop, FALSE, FALSE, classs);
            if(var.ty -> kind == Ty_name || var.ty -> kind == Ty_array || var.ty -> kind == Ty_poly)
                return var;
            return expTy(Tr_DerefExp(e->pos, var.exp->u.exp, convertType(actual_ty(var.ty))), var.ty);
        }
        else if(e -> u.var -> kind == A_subscriptVar)
        {
            struct expty var = transVar(venv, tenv, e->u.var, level, isLoop, FALSE, FALSE, classs);
            return expTy(Tr_DerefExp(e->pos, var.exp->u.exp, convertType(actual_ty(var.ty))), var.ty);
        }
        else if(e -> u.var -> kind == A_fieldVar || e -> u.var -> kind == A_arrowFieldVar)
        {
            return transVar(venv, tenv, e -> u.var, level, isLoop, FALSE, FALSE, classs);
        }
        else
        {
            EM_error(e -> pos, "Unknown variable type.");
        }
        break;
    }
    case A_derefExp:
    {
        E_enventry entry = (E_enventry)S_look(venv, e->u.deref->u.simple);
        return expTy(Tr_DerefExp(e->pos, Tr_VarExp(e->pos, T_i32, entry->u.var.access, !(entry->u.var.access->level->parent), FALSE)->u.exp, convertType(actual_ty(entry->u.var.ty))), transVar(venv, tenv, e->u.deref, level, isLoop, FALSE, FALSE, classs).ty);
        break;
    }
    case A_addressExp:
    {
        E_enventry varEntry = (E_enventry)S_look(venv, e->u.address->u.simple);
        int address = 0;
        if (!varEntry)
        {
            struct expty varExpty = transVar(venv, tenv, e -> u.address, level, isLoop, FALSE, FALSE, classs);
            if((e -> u.address -> kind == A_fieldVar || e -> u.address -> kind == A_simpleVar) && varExpty.exp -> u.exp -> kind == T_loadExp){
                return expTy(Tr_OpExp(e -> pos, T_i32, T_add, varExpty.exp -> u.exp -> u.load.addr, Tr_AddrExp(e -> pos, 0)->u.exp), Ty_Pointer(varExpty.ty));
            }
            else if((e -> u.address -> kind == A_fieldVar || e -> u.address -> kind == A_simpleVar) && varExpty.exp -> u.exp -> kind != T_loadExp){
                return expTy(varExpty.exp, Ty_Pointer(varExpty.ty));
            }
            EM_error(e->pos, "noexist.var %s", S_name(getName(e -> u.address)));
        }
        else
        {
            toInFrame(varEntry->u.var.access->access, varEntry->u.var.access->level->frame);
        }
        address = varEntry->u.var.access->level->frame->offset +
            varEntry->u.var.access->access->u.offset;
      //printf("Address of %s: %d\n", S_name(e->u.address->u.simple), address);
        return expTy(Tr_AddrExp(e->pos, address), Ty_Pointer(varEntry->u.var.ty));
        break;
    }
    case A_funcExp:
    {
        char nameBuffer[8] = "";
        sprintf(nameBuffer, "%d", funcs);
        A_dec funcDec = A_FunctionDec(e -> pos, A_FundecList(A_Fundec(e -> pos, S_Symbol(nameBuffer), e -> u.func.params, e -> u.func.result, e -> u.func.body), A_FundecList(NULL, NULL)));
        Ty_tyList Ty_tyParams = makeParamTypeList(venv, tenv, e -> u.func.params, level);
        Ty_ty Ty_tyResult = Ty_Void();
        if(e -> u.func.result -> head)
            Ty_tyResult = transTy(venv, tenv, e -> u.func.result -> head -> typ, level);
        T_module dec = transDec(venv, tenv, funcDec, level, isLoop, venv, classs);
        tableList -> head = dec;
        list -> head = dec;
        tableList -> tail = T_ModuleList(NULL, NULL);
        list -> tail = T_ModuleList(NULL, NULL);
        tableList = tableList -> tail;
        list = list -> tail;
        tableList -> head = NULL;
        list -> head = NULL;
        tableList -> tail = NULL;
        list -> tail = NULL;
        tableIndex++;
        typeList -> head = T_TypeMod(convertAllType(Ty_tyParams), convertType(Ty_tyResult));
        typeList -> tail = T_ModuleList(NULL, NULL);
        typeList = typeList -> tail;
        typeList -> head = NULL;
        typeList -> tail = NULL;
        typeIndex++;
        return expTy(Tr_AddrExp(e -> pos, tableIndex - 1), Ty_Func(Ty_tyParams, Ty_tyResult, typeIndex - 1));
    }
    case A_parenExp:
    {
        return transExp(venv, tenv, e -> u.paren.paren, level, isLoop, classs);
    }
    case A_callExp:
    {
        S_symbol name = NULL;
        bool isCalledInMethod = FALSE;
        if(e -> u.call.func -> kind == A_varExp || e -> u.call.func -> kind == A_fieldExp || e -> u.call.func -> kind == A_arrowFieldExp){
            E_enventry entry = NULL;
            if(e -> u.call.func -> kind == A_varExp)
            {
                A_var var = e -> u.call.func -> u.var;
                if(var -> kind == A_simpleVar){
                    name = var -> u.simple;
                    entry = (E_enventry)S_look(venv, name);
                    if(entry == NULL){
                        E_enventry classEntry;
                        if(classs -> kind == Ty_name){
                            classEntry = (E_enventry)S_look(tenv, classs -> u.name.sym);
                        }
                        if(classEntry){
                            entry = (E_enventry)S_look(classEntry -> u.classs.methods, var -> u.simple);
                            isCalledInMethod = TRUE;
                        }
                    }
                }
                else if(var -> kind == A_fieldVar || var -> kind == A_arrowFieldVar){
                    struct expty field;
                    Ty_ty type = NULL;
                    if(var -> kind == A_fieldVar){
                        field = transVar(venv, tenv, var->u.field.var, level, isLoop, FALSE, FALSE, classs);
                        type = field.ty;
                        // printf("%d typeeee field\n", type -> kind);

                        name = var -> u.field.sym;
                    }
                    else if(var -> kind == A_arrowFieldVar){
                        field = transVar(venv, tenv, var->u.arrowfield.pointer, level, isLoop, FALSE, FALSE, classs);
                        if(field.ty -> kind != Ty_pointer){
                            EM_error(var -> pos, "type.notpointer %s", S_name(getName(var -> u.arrowfield.pointer)));
                        }
                        type = field.ty -> u.pointer;
                        // printf("%d typeeee arrowField\n", field.ty -> kind);
                        name = var -> u.arrowfield.member;
                    }
                    else
                    {
                        // printf("y tho\n");
                    }
                    E_enventry classEntry = NULL;
                    if(type -> kind == Ty_name)
                        classEntry = (E_enventry)S_look(tenv, type -> u.name.sym);
                    else if(type -> kind == Ty_poly)
                    {
                        E_enventry templateEntry = (E_enventry)S_look(tenv, type -> u.poly.name);
                        if(templateEntry && templateEntry -> kind == E_templateentry)
                        {
                            classEntry = (E_enventry)TAB_search(templateEntry -> u.templatee.specific, type -> u.poly.typeParam);
                        }
                        else
                        {
                            EM_error(e -> pos, "noexist.template %s", S_name(type -> u.poly.name));
                        }
                    }
                    else
                    {
                        EM_error(e -> pos, "type.notobject %s", S_name(getName(var)));
                    }
                    if(classEntry){
                        entry = (E_enventry)S_look(classEntry -> u.classs.methods, name);
                    }
                    else{
                        EM_error(e -> pos, "type.noclass %s", S_name(type -> u.name.sym));
                    }
                }
            }
            else if(e -> u.call.func -> kind == A_fieldExp || e -> u.call.func -> kind == A_arrowFieldExp){
                struct expty fieldExpty;
                Ty_ty type = NULL;
                if(e -> u.call.func -> kind == A_fieldExp)
                {
                    fieldExpty = transExp(venv, tenv, e -> u.call.func -> u.field.field, level, isLoop, classs);
                    type = fieldExpty.ty;
                    name = e -> u.call.func -> u.field.member;
                }
                else if(e -> u.call.func -> kind == A_arrowFieldExp)
                {
                    fieldExpty = transExp(venv, tenv, e -> u.call.func -> u.arrowfield.pointer, level, isLoop, classs);
                    type = fieldExpty.ty -> u.pointer;
                    name = e -> u.call.func -> u.arrowfield.member;
                }
                if(type -> kind == Ty_name || type -> kind == Ty_poly)
                {
                    E_enventry classEntry = NULL;
                    if(type -> kind == Ty_name)
                    {
                        classEntry = (E_enventry)S_look(tenv, type -> u.name.sym);
                    }
                    else if(type -> kind == Ty_poly)
                    {
                        E_enventry templateEntry = (E_enventry)S_look(tenv, type -> u.poly.name);
                        if(templateEntry && templateEntry -> kind == E_templateentry)
                        {
                            classEntry = (E_enventry)TAB_search(templateEntry -> u.templatee.specific, type -> u.poly.typeParam);
                        }
                        else
                        {
                            EM_error(e -> pos, "noexist.template %s", S_name(type -> u.poly.name));
                        }
                    }
                    if(classEntry && classEntry -> kind == E_classentry){
                        entry = (E_enventry)S_look(classEntry -> u.classs.methods, name);
                    }
                    else{
                        EM_error(e -> pos, "noexist.class %s", S_name(type -> u.name.sym));
                    }
                }
                else
                {
                    EM_error(e -> pos, "noexist.member %s", S_name(name));
                }
            }
            T_expList expList = T_ExpList(NULL, NULL);
            T_expList result = expList;
            if(entry != NULL && entry -> kind == E_listEntry && entry -> u.list -> size() > 0){
                for(const auto &entries: *(entry -> u.list)){
                    if(entries -> kind == E_funcentry){
                        Ty_tyList paramType = entries -> u.func.formals;
                        A_expList args = e -> u.call.args;
                        bool functionOk = true;
                        for(;args && paramType; paramType = paramType -> tail, args = args -> tail){
                            if (args -> head == NULL && paramType -> head != NULL)
                            {
                                EM_error(e->pos, "func.few");
                                break;
                            }
                            else if (args -> head != NULL && paramType -> head == NULL)
                            {
                                EM_error(e->pos, "func.many");
                                break;
                            }
                            Ty_ty argType = transExp(venv, tenv, args -> head, level, isLoop, classs).ty;
                            if(compType(paramType -> head, argType)){
                                continue;
                            }
                            else if(args -> head -> kind == A_stringExp && paramType -> head -> kind == Ty_name){
                                if(strcmp(_stringClassName.c_str(), S_name(paramType -> head -> u.name.sym)) == 0){
                                    continue;
                                }
                            }
                            else{
                                functionOk = false;
                                break;
                            }
                        }
                        if(functionOk){
                            entry = entries;
                            break;
                        }
                        else{
                            entry = NULL;
                        }
                    }
                }
            }
            if (entry != NULL && (entry->kind == E_funcentry || (entry->kind == E_templateentry && entry -> u.templatee.dec -> kind == A_functionDec)))
            {
                E_enventry templateTemp = (E_enventry)checked_malloc(sizeof(*templateTemp));
                templateTemp = NULL;
                A_expList expArgs = NULL;
                Ty_tyList entryArgsTy = NULL;
                if(entry -> kind == E_templateentry)
                {
                    templateTemp = entry;
                    A_fundec fundec = templateTemp -> u.templatee.dec -> u.function -> head;
                    Ty_ty type = Ty_Void();
                    entryArgsTy = makeParamTypeList(venv, tenv, fundec -> params, level);
                    for(A_expList argList = e -> u.call.args; entryArgsTy && argList; entryArgsTy = entryArgsTy -> tail, argList = argList -> tail)
                    {
                        if (argList -> head == NULL && entryArgsTy -> head != NULL)
                        {
                            EM_error(e->pos, "func.few");
                            break;
                        }
                        else if (argList -> head != NULL && entryArgsTy -> head == NULL)
                        {
                            EM_error(e->pos, "func.many");
                            break;
                        }
                        if(entryArgsTy -> head -> kind == Ty_name)
                        {
                          //printf("%s templatee.name\n", S_name(entry -> u.templatee.name));
                            if(entryArgsTy -> head -> u.name.sym == entry -> u.templatee.name)
                            {
                                struct expty arg = transExp(venv, tenv, argList -> head, level , isLoop, classs);
                              //printf("%d type kind\n", arg.ty -> kind);
                                entry = (E_enventry)TAB_search(entry -> u.templatee.specific, arg.ty);
                                if(entry){
                                    break;
                                }
                                S_enter(tenv, templateTemp -> u.templatee.name, E_PolyEntry(arg.ty));
                                type = arg.ty;
                                break;
                            }
                        }
                    }
                    T_module dec = transDec(venv, tenv, templateTemp -> u.templatee.dec, level, isLoop, venv, classs);
                    list -> head = dec;
                    list -> tail = T_ModuleList(NULL, NULL);
                    list = list -> tail;
                    list -> head = NULL;
                    list -> tail = NULL;
                    entry = (E_enventry)S_look(venv, name);
                    TAB_insert(templateTemp -> u.templatee.specific, type, entry);
                }
                int i = 1;
                for (i = 1, expArgs = e->u.call.args, entryArgsTy = entry->u.func.formals; expArgs != NULL && entryArgsTy != NULL;
                    expArgs = expArgs->tail, entryArgsTy = entryArgsTy->tail, i++)
                {
                    A_exp exp = expArgs->head;
                    Ty_ty type = entryArgsTy->head;

                    if (exp == NULL && type != NULL)
                    {
                        EM_error(e->pos, "func.few");
                        break;
                    }
                    else if (exp != NULL && type == NULL)
                    {
                        EM_error(e->pos, "func.many");
                        break;
                    }
                    struct expty expp = transExp(venv, tenv, exp, level, isLoop, classs);
                    if(exp -> kind == A_stringExp && type -> kind == Ty_name ){
                        if(type -> kind == Ty_name){
                            stringLiterals -> head = transStm(venv, tenv, A_DeclarationStm(exp -> pos, A_VarDec(exp -> pos, A_AssignStm(exp -> pos, A_SimpleVar(exp -> pos, S_Symbol("__string_literal")), exp, TRUE), A_NameTy(exp -> pos, type -> u.name.sym))), level, isLoop, classs).exp -> u.stm;
                            stringLiterals -> tail = T_StmList(NULL, NULL);
                            stringLiterals = stringLiterals -> tail;
                            stringLiterals -> head = NULL;
                            stringLiterals -> tail = NULL;
                        }
                        exp = A_VarExp(exp -> pos, A_SimpleVar(exp -> pos, S_Symbol("__string_literal")));
                        expp = transExp(venv, tenv, exp, level, isLoop, classs);
                    }
                    if(expp.ty -> kind == Ty_array && expp.exp -> u.exp -> kind == T_loadExp)
                        expList->head = expp.exp->u.exp -> u.load.addr;
                    else
                        expList->head = expp.exp->u.exp;
                    expList->tail = T_ExpList(NULL, NULL);
                    if (exp != NULL && type != NULL)
                    {
                        if (!compType(type, expp.ty))
                        {
                            // printf("%d %d flaj\n", type -> kind, argType.ty -> kind);
                            expList -> head = convertAssign(type, expp, e -> pos);
                        }
                    }
                    expList = expList->tail;
                    expList->tail = NULL;
                    expList->head = NULL;
                }
                if(e -> u.call.func -> kind == A_varExp && e -> u.call.func-> u.var -> kind == A_fieldVar){
                    struct expty field = transVar(venv, tenv, e -> u.call.func -> u.var -> u.field.var, level, isLoop, FALSE, FALSE, classs);
                    result = T_ExpList(field.exp -> u.exp, result);
                }
                else if(e -> u.call.func -> kind == A_varExp && e -> u.call.func-> u.var -> kind == A_arrowFieldVar){
                    struct expty field = transVar(venv, tenv, e -> u.call.func -> u.var -> u.arrowfield.pointer, level, isLoop, FALSE, FALSE, classs);
                    result = T_ExpList(field.exp -> u.exp, result);
                }
                else if(isCalledInMethod){
                    result = T_ExpList(T_GetLocalExp(T_i32, 0), result);
                }
                else if(e -> u.call.func -> kind == A_fieldExp)
                {
                    struct expty field = transExp(venv, tenv, e -> u.call.func -> u.field.field, level, isLoop, classs);
                    result = T_ExpList(field.exp -> u.exp, result);
                }
                else if(e -> u.call.func -> kind == A_arrowFieldExp)
                {
                    struct expty field = transExp(venv, tenv, e -> u.call.func -> u.arrowfield.pointer, level, isLoop, classs);
                    result = T_ExpList(field.exp -> u.exp, result);
                }
                if (expArgs == NULL && entryArgsTy != NULL)
                {
                    if (entryArgsTy->head != NULL)
                    {
                        EM_error(e->pos, "func.few");
                    }
                }
                else if (expArgs != NULL && entryArgsTy == NULL)
                {
                    if (expArgs->head != NULL)
                    {
                        EM_error(e->pos, "func.many");
                    }
                }
                if(templateTemp)
                {
                    S_enter(venv, name, templateTemp);
                }
                // printf("%s %d fdoaijfoasdjfoijsijo\n", S_name(e ->u.call.func), entry -> kind);
                return expTy(Tr_CallExp(e->pos, convertType(entry->u.func.returnType),
                    entry->u.func.index, S_name(entry->u.func.label), result),
                    entry->u.func.returnType);
            }
            else
            {
                EM_error(e->pos, "noexist.func %s", S_name(name));
            }
        }
        else
            EM_error(e -> pos, "func.cannotcall");
    }
    case A_recordExp:
    {
        return expTy(NULL, (Ty_ty)S_look(tenv, e->u.record.typ));
    }
    case A_sizeofExp:
    {
        E_enventry entry = NULL;
        if(e -> u.sizeOf -> kind == A_simpleVar)
        {
            entry = (E_enventry)S_look(venv, e -> u.sizeOf -> u.simple);
            // printf("entry -> kind %d\n", entry -> kind);
            // return expTy(Tr_AddrExp(e -> pos, entry -> u.var.ty -> size), Ty_Short());
            // printf("%d sizeOf kinddddddddddddddddddddddddd\n", transVar(venv, tenv, e -> u.sizeOf, level, isLoop, FALSE, FALSE, classs).ty -> kind);
            return expTy(Tr_AddrExp(e -> pos, transVar(venv, tenv, e -> u.sizeOf, level, isLoop, FALSE, FALSE, classs).ty -> size), Ty_Short());
        }
        else if(e -> u.sizeOf -> kind == A_subscriptVar)
        {
            entry = (E_enventry)S_look(venv, e -> u.sizeOf -> u.subscript.name);
            return expTy(Tr_AddrExp(e -> pos, actual_ty(entry -> u.var.ty) -> size), Ty_Short());
        }
        else if(e -> u.sizeOf -> kind == A_derefVar)
        {
            return expTy(Tr_AddrExp(e -> pos, transVar(venv, tenv, e -> u.sizeOf -> u.deref, level, isLoop, FALSE, FALSE, classs).ty -> u.pointer -> size), Ty_Short());
        }
        else
            EM_error(e -> pos, "sizeof.var");
        
    }
    case A_arrayExp:
    {
        T_expList list = T_ExpList(NULL, NULL);
        T_expList result = list;
        A_expList explist = e -> u.array.list;
        if(explist == NULL)
            return expTy(Tr_NoExp(e -> pos), Ty_Void());
        int i = 0;
        A_exp exp = NULL;
        struct expty expp;
        Ty_ty arrayType = NULL;
        for(; explist; explist = explist -> tail, i++)
        {
            exp = explist -> head;
            expp = transExp(venv, tenv, exp, level, isLoop, classs);
            // printf("%d kind\n", expp.ty -> kind);
            list -> head = expp.exp -> u.exp;
            if(arrayType && compType(arrayType, expp.ty))
                arrayType = expp.ty;
            else if(!arrayType)
                arrayType = expp.ty;
            else if(arrayType && arrayType != expp.ty){
                // printf("%d %d types\n", arrayType -> kind, expp.ty -> kind);
                EM_error(e -> pos, "type.element %d", i);
            }
            list -> tail = T_ExpList(NULL, NULL);
            list = list -> tail;
            list -> head = NULL;
            list -> tail = NULL;
        }
        // printf("testettsedtestest\n");
        return expTy(Tr_ArrayExp(e -> pos, result), Ty_Array(arrayType, i));
    }
    case A_subscriptExp:
    {
        struct expty arrayExpty = transExp(venv, tenv, e -> u.subscript.array, level, isLoop, classs);
        struct expty indexExpty = transExp(venv, tenv, e -> u.subscript.index, level, isLoop, classs);
        T_exp offset = NULL;
        if(arrayExpty.ty -> kind == Ty_array)
        {
            offset = Tr_OpExp(e -> pos, T_i32, T_mul, Tr_AddrExp(e -> pos, arrayExpty.ty -> u.array.type -> size)->u.exp, T_ConvertExp(T_i32, indexExpty.exp -> u.exp))->u.exp;
        }
        else if(arrayExpty.ty -> kind == Ty_pointer)
        {
            offset = Tr_OpExp(e -> pos, T_i32, T_mul, Tr_AddrExp(e -> pos, arrayExpty.ty -> u.pointer -> size)->u.exp, T_ConvertExp(T_i32, indexExpty.exp -> u.exp))->u.exp;
        }
        else
        {
            EM_error(e -> pos, "operator.subscript");
        }
        return expTy(Tr_DerefExp(e -> pos, Tr_OpExp(e -> pos, T_i32, T_add, arrayExpty.exp -> u.exp, offset)->u.exp, convertType(arrayExpty.ty -> u.array.type)), arrayExpty.ty -> u.array.type);
    }
    case A_fieldExp:
    {
        //debug(e -> pos, "A_FieldExp");
        struct expty fieldExpty = transExp(venv, tenv, e -> u.field.field, level, isLoop, classs);
        if(fieldExpty.ty -> kind == Ty_name || fieldExpty.ty -> kind == Ty_poly){
            E_enventry classEntry;
            if(fieldExpty.ty -> kind == Ty_name)
            {
                classEntry = (E_enventry)S_look(tenv, fieldExpty.ty -> u.name.sym);
            }
            else if(fieldExpty.ty -> kind == Ty_poly)
            {
                E_enventry templateEntry = (E_enventry)S_look(tenv, fieldExpty.ty -> u.poly.name);
                if(templateEntry && templateEntry -> kind == E_templateentry)
                {
                    classEntry = (E_enventry)TAB_search(templateEntry -> u.templatee.specific, fieldExpty.ty -> u.poly.typeParam);
                }
                else
                {
                    EM_error(e -> pos, "noexist.template %s", S_name(fieldExpty.ty -> u.poly.name));
                }
            }
            if(classEntry -> kind != E_classentry)
                EM_error(e -> pos, "noexist.class %s", S_name(fieldExpty.ty -> u.name.sym));
            Ty_member member = (Ty_member)S_look(classEntry -> u.classs.varTypes, e -> u.field.member);
            if(member)
            {
                T_exp offset = Tr_AddrExp(e -> pos, member -> offset)->u.exp;
                return expTy(Tr_DerefExp(e -> pos, Tr_OpExp(e -> pos, T_i32, T_add, fieldExpty.exp -> u.exp, offset)->u.exp, convertType(member->ty)), member->ty);
            }
            else
            {
                EM_error(e -> pos, "class.nomember %s %s", S_name(e -> u.field.member), S_name(fieldExpty.ty -> u.name.sym));
            }
        }
        else
        {
            EM_error(e -> pos, "operator.field");
        }
    }
    case A_arrowFieldExp:
    {
        //debug(e -> pos, "A_ArrowFieldExp");
        struct expty fieldExpty = transExp(venv, tenv, e -> u.arrowfield.pointer, level, isLoop, classs);
        if(fieldExpty.ty -> kind == Ty_pointer){
            if(fieldExpty.ty -> u.pointer -> kind == Ty_name || fieldExpty.ty -> u.pointer -> kind == Ty_poly){
                E_enventry classEntry;
                if(fieldExpty.ty -> u.pointer -> kind == Ty_name)
                {
                    classEntry = (E_enventry)S_look(tenv, fieldExpty.ty -> u.pointer -> u.name.sym);
                }
                else if(fieldExpty.ty -> u.pointer -> kind == Ty_poly)
                {
                    E_enventry templateEntry = (E_enventry)S_look(tenv, fieldExpty.ty -> u.pointer -> u.poly.name);
                    if(templateEntry && templateEntry -> kind == E_templateentry)
                    {
                        classEntry = (E_enventry)TAB_search(templateEntry -> u.templatee.specific, fieldExpty.ty -> u.pointer -> u.poly.typeParam);
                    }
                    else
                    {
                        EM_error(e -> pos, "noexist.template %s", S_name(fieldExpty.ty -> u.pointer -> u.poly.name));
                    }
                }
                if(classEntry -> kind != E_classentry)
                    EM_error(e -> pos, "noexist.class %s", S_name(fieldExpty.ty -> u.pointer -> u.name.sym));
                Ty_member member = (Ty_member)S_look(classEntry -> u.classs.varTypes, e -> u.field.member);
                if(member)
                {
                    T_exp offset = Tr_AddrExp(e -> pos, member -> offset)->u.exp;
                    if(member -> ty -> kind == Ty_array || member -> ty -> kind == Ty_name || member -> ty -> kind == Ty_poly){
                        return expTy(Tr_OpExp(e -> pos, T_i32, T_add, fieldExpty.exp -> u.exp, offset), member -> ty);
                    }
                    return expTy(Tr_DerefExp(e -> pos, Tr_OpExp(e -> pos, T_i32, T_add, fieldExpty.exp -> u.exp, offset)->u.exp, convertType(member->ty)), member->ty);
                }
                else
                {
                    EM_error(e -> pos, "class.nomember %s %s", S_name(e -> u.field.member), S_name(fieldExpty.ty -> u.pointer -> u.name.sym));
                }
            }
            else
            {
                EM_error(e -> pos, "operator.field");
            }
        }
        else
        {
            EM_error(e -> pos, "operator.arrow");
        }
    }
    case A_opExp:
    {
        A_oper oper = e->u.op.oper;
        struct expty left = transExp(venv, tenv, e->u.op.left, level, isLoop, classs);
        struct expty right = transExp(venv, tenv, e->u.op.right, level, isLoop, classs);
        if(left.ty -> kind == Ty_name){
            S_symbol opSym = operToSym(oper);
            E_enventry classEntry = (E_enventry)S_look(tenv, left.ty -> u.name.sym);
            if(classEntry){
                E_enventry funcEntry = (E_enventry)S_look(classEntry -> u.classs.methods, opSym);
                if(funcEntry){
                    return transExp(venv, tenv, A_CallExp(e -> pos, A_FieldExp(e -> pos, e -> u.op.left, opSym), A_ExpList(e -> u.op.right, NULL)), level, isLoop, classs);
                }
                else{
                    EM_error(e -> pos, "noexist.func %s", S_name(opSym));
                }
            }
            else{
                EM_error(e -> pos, "noexist.class %s", S_name(left.ty -> u.name.sym));
            }
        }
        if (oper == A_plusOp)
        {
            // printf("%d size\n", left.ty->kind);
            if (left.ty != NULL && right.ty != NULL)
            {
                if (left.ty->kind == Ty_short && right.ty->kind == Ty_int)
                    return expTy(Tr_OpExp(e->pos, convertType(right.ty), T_add, T_ConvertExp(T_i64, left.exp->u.exp), right.exp->u.exp), Ty_Int());
                if (left.ty->kind == Ty_int && right.ty->kind == Ty_short)
                    return expTy(Tr_OpExp(e->pos, convertType(right.ty), T_add, left.exp->u.exp, T_ConvertExp(T_i64, right.exp->u.exp)), Ty_Int());
                if (left.ty->kind == Ty_short && right.ty->kind == Ty_short)
                    return expTy(Tr_OpExp(e->pos, convertType(left.ty), T_add, left.exp->u.exp, right.exp->u.exp), Ty_Short());
                if (left.ty->kind == Ty_int && right.ty->kind == Ty_int)
                    return expTy(Tr_OpExp(e->pos, convertType(left.ty), T_add, left.exp->u.exp, right.exp->u.exp), Ty_Int());
                if (left.ty->kind == Ty_real && (right.ty->kind == Ty_int || right.ty->kind == Ty_short))
                    return expTy(Tr_OpExp(e->pos, convertType(left.ty), T_add, left.exp->u.exp, T_ConvertExp(T_f64, right.exp->u.exp)), Ty_Real());
                if ((left.ty->kind == Ty_int || left.ty->kind == Ty_short) && right.ty->kind == Ty_real)
                    return expTy(Tr_OpExp(e->pos, convertType(right.ty), T_add, T_ConvertExp(T_f64, left.exp->u.exp), right.exp->u.exp), Ty_Real());
                if (left.ty->kind == Ty_real && right.ty->kind == Ty_real)
                    return expTy(Tr_OpExp(e->pos, convertType(left.ty), T_add, left.exp->u.exp, right.exp->u.exp), Ty_Real());
                if (left.ty->kind == Ty_string && right.ty->kind == Ty_string)
                    return expTy(Tr_OpExp(e->pos, convertType(left.ty), T_add, left.exp->u.exp, right.exp->u.exp), Ty_String());
                else
                    EM_error(e->u.op.left->pos, "type.add");
            }
            else
            {
                if (left.ty == NULL && right.ty != NULL)
                    EM_error(e->pos, "type.lhs");
                else if (left.ty != NULL && right.ty == NULL)
                    EM_error(e->pos, "type.rhs");
                if (left.ty == NULL && right.ty == NULL)
                    EM_error(e->pos, "type.bhs");
            }
        }
        if (oper == A_minusOp)
        {
            if (left.ty != NULL && right.ty != NULL)
            {
                if (left.ty->kind == Ty_short && right.ty->kind == Ty_int)
                    return expTy(Tr_OpExp(e->pos, convertType(right.ty), T_sub, T_ConvertExp(T_i64, left.exp->u.exp), right.exp->u.exp), Ty_Int());
                if (left.ty->kind == Ty_int && right.ty->kind == Ty_short)
                    return expTy(Tr_OpExp(e->pos, convertType(right.ty), T_sub, left.exp->u.exp, T_ConvertExp(T_i64, right.exp->u.exp)), Ty_Int());
                if (left.ty->kind == Ty_short && right.ty->kind == Ty_short)
                    return expTy(Tr_OpExp(e->pos, convertType(left.ty), T_sub, left.exp->u.exp, right.exp->u.exp), Ty_Short());
                if (left.ty->kind == Ty_int && right.ty->kind == Ty_int)
                    return expTy(Tr_OpExp(e->pos, convertType(left.ty), T_sub, left.exp->u.exp, right.exp->u.exp), Ty_Int());
                if (left.ty->kind == Ty_real && (right.ty->kind == Ty_int || right.ty->kind == Ty_short))
                    return expTy(Tr_OpExp(e->pos, convertType(left.ty), T_sub, left.exp->u.exp, T_ConvertExp(T_f64, right.exp->u.exp)), Ty_Real());
                if ((left.ty->kind == Ty_int || left.ty->kind == Ty_short) && right.ty->kind == Ty_real)
                    return expTy(Tr_OpExp(e->pos, convertType(right.ty), T_sub, T_ConvertExp(T_f64, left.exp->u.exp), right.exp->u.exp), Ty_Real());
                if (left.ty->kind == Ty_real && right.ty->kind == Ty_real)
                    return expTy(Tr_OpExp(e->pos, convertType(left.ty), T_sub, left.exp->u.exp, right.exp->u.exp), Ty_Real());
                if (left.ty->kind == Ty_string && right.ty->kind == Ty_string)
                    return expTy(Tr_OpExp(e->pos, convertType(left.ty), T_sub, left.exp->u.exp, right.exp->u.exp), Ty_String());
                else
                    EM_error(e->u.op.left->pos, "type.sub");
            }
            else
            {
                if (left.ty == NULL && right.ty != NULL)
                    EM_error(e->pos, "type.lhs");
                else if (left.ty != NULL && right.ty == NULL)
                    EM_error(e->pos, "type.rhs");
                if (left.ty == NULL && right.ty == NULL)
                    EM_error(e->pos, "type.bhs");
            }
        }
        if (oper == A_timesOp)
        {
            if (left.ty != NULL && right.ty != NULL)
            {
                if (left.ty->kind == Ty_short && right.ty->kind == Ty_int)
                    return expTy(Tr_OpExp(e->pos, convertType(right.ty), T_mul, T_ConvertExp(T_i64, left.exp->u.exp), right.exp->u.exp), Ty_Int());
                if (left.ty->kind == Ty_int && right.ty->kind == Ty_short)
                    return expTy(Tr_OpExp(e->pos, convertType(right.ty), T_mul, left.exp->u.exp, T_ConvertExp(T_i64, right.exp->u.exp)), Ty_Int());
                if (left.ty->kind == Ty_short && right.ty->kind == Ty_short)
                    return expTy(Tr_OpExp(e->pos, convertType(left.ty), T_mul, left.exp->u.exp, right.exp->u.exp), Ty_Short());
                if (left.ty->kind == Ty_int && right.ty->kind == Ty_int)
                    return expTy(Tr_OpExp(e->pos, convertType(left.ty), T_mul, left.exp->u.exp, right.exp->u.exp), Ty_Int());
                if (left.ty->kind == Ty_real && (right.ty->kind == Ty_int || right.ty->kind == Ty_short))
                    return expTy(Tr_OpExp(e->pos, convertType(left.ty), T_mul, left.exp->u.exp, T_ConvertExp(T_f64, right.exp->u.exp)), Ty_Real());
                if ((left.ty->kind == Ty_int || left.ty->kind == Ty_short) && right.ty->kind == Ty_real)
                    return expTy(Tr_OpExp(e->pos, convertType(right.ty), T_mul, T_ConvertExp(T_f64, left.exp->u.exp), right.exp->u.exp), Ty_Real());
                if (left.ty->kind == Ty_real && right.ty->kind == Ty_real)
                    return expTy(Tr_OpExp(e->pos, convertType(left.ty), T_mul, left.exp->u.exp, right.exp->u.exp), Ty_Real());
                if (left.ty->kind == Ty_string && right.ty->kind == Ty_string)
                    return expTy(Tr_OpExp(e->pos, convertType(left.ty), T_mul, left.exp->u.exp, right.exp->u.exp), Ty_String());
                else
                    EM_error(e->u.op.left->pos, "type.mul");
            }
            else
            {
                if (left.ty == NULL && right.ty != NULL)
                    EM_error(e->pos, "type.lhs");
                else if (left.ty != NULL && right.ty == NULL)
                    EM_error(e->pos, "type.rhs");
                if (left.ty == NULL && right.ty == NULL)
                    EM_error(e->pos, "type.bhs");
            }
        }
        if (oper == A_divideOp)
        {
            if (left.ty != NULL && right.ty != NULL)
            {
                if (left.ty->kind == Ty_short && right.ty->kind == Ty_int)
                    return expTy(Tr_OpExp(e->pos, convertType(right.ty), T_div_s, T_ConvertExp(T_i64, left.exp->u.exp), right.exp->u.exp), Ty_Int());
                if (left.ty->kind == Ty_int && right.ty->kind == Ty_short)
                    return expTy(Tr_OpExp(e->pos, convertType(right.ty), T_div_s, left.exp->u.exp, T_ConvertExp(T_i64, right.exp->u.exp)), Ty_Int());
                if (left.ty->kind == Ty_short && right.ty->kind == Ty_short)
                    return expTy(Tr_OpExp(e->pos, convertType(left.ty), T_div_s, left.exp->u.exp, right.exp->u.exp), Ty_Short());
                if (left.ty->kind == Ty_int && right.ty->kind == Ty_int)
                    return expTy(Tr_OpExp(e->pos, convertType(left.ty), T_div_s, left.exp->u.exp, right.exp->u.exp), Ty_Int());
                if (left.ty->kind == Ty_real && (right.ty->kind == Ty_int || right.ty->kind == Ty_short))
                    return expTy(Tr_OpExp(e->pos, convertType(left.ty), T_div_s, left.exp->u.exp, T_ConvertExp(T_f64, right.exp->u.exp)), Ty_Real());
                if ((left.ty->kind == Ty_int || left.ty->kind == Ty_short) && right.ty->kind == Ty_real)
                    return expTy(Tr_OpExp(e->pos, convertType(right.ty), T_div_s, T_ConvertExp(T_f64, left.exp->u.exp), right.exp->u.exp), Ty_Real());
                if (left.ty->kind == Ty_real && right.ty->kind == Ty_real)
                    return expTy(Tr_OpExp(e->pos, convertType(left.ty), T_div_s, left.exp->u.exp, right.exp->u.exp), Ty_Real());
                if (left.ty->kind == Ty_string && right.ty->kind == Ty_string)
                    return expTy(Tr_OpExp(e->pos, convertType(left.ty), T_div_s, left.exp->u.exp, right.exp->u.exp), Ty_String());
                EM_error(e->u.op.left->pos, "type.div");
                return expTy(NULL, NULL);
            }
            else
            {
                if (left.ty == NULL && right.ty != NULL)
                    EM_error(e->pos, "type.lhs");
                else if (left.ty != NULL && right.ty == NULL)
                    EM_error(e->pos, "type.rhs");
                if (left.ty == NULL && right.ty == NULL)
                    EM_error(e->pos, "type.bhs");
            }
        }
        if (oper == A_eqOp)
        {
            if (left.ty != NULL && right.ty != NULL)
            {
                if (left.ty->kind == Ty_short && right.ty->kind == Ty_int)
                    return expTy(Tr_OpExp(e->pos, convertType(right.ty), T_eq, T_ConvertExp(T_i64, left.exp->u.exp), right.exp->u.exp), Ty_Bool());
                if (left.ty->kind == Ty_int && right.ty->kind == Ty_short)
                    return expTy(Tr_OpExp(e->pos, convertType(right.ty), T_eq, left.exp->u.exp, T_ConvertExp(T_i64, right.exp->u.exp)), Ty_Bool());
                if (left.ty->kind == Ty_short && right.ty->kind == Ty_short)
                    return expTy(Tr_OpExp(e->pos, convertType(left.ty), T_eq, left.exp->u.exp, right.exp->u.exp), Ty_Bool());
                if (left.ty->kind == Ty_int && right.ty->kind == Ty_int)
                    return expTy(Tr_OpExp(e->pos, convertType(left.ty), T_eq, left.exp->u.exp, right.exp->u.exp), Ty_Bool());
                if (left.ty->kind == Ty_real && (right.ty->kind == Ty_int || right.ty->kind == Ty_short))
                    return expTy(Tr_OpExp(e->pos, convertType(left.ty), T_eq, left.exp->u.exp, T_ConvertExp(T_f64, right.exp->u.exp)), Ty_Bool());
                if ((left.ty->kind == Ty_int || left.ty->kind == Ty_short) && right.ty->kind == Ty_real)
                    return expTy(Tr_OpExp(e->pos, convertType(right.ty), T_eq, T_ConvertExp(T_f64, left.exp->u.exp), right.exp->u.exp), Ty_Bool());
                if (left.ty->kind == Ty_real && right.ty->kind == Ty_real)
                    return expTy(Tr_OpExp(e->pos, convertType(left.ty), T_eq, left.exp->u.exp, right.exp->u.exp), Ty_Bool());
                if(left.ty -> kind == right.ty -> kind){
                    return expTy(Tr_OpExp(e->pos, convertType(left.ty), T_eq, left.exp->u.exp, right.exp->u.exp), Ty_Bool());
                }
                EM_error(e->u.op.left->pos, "type.comp");
                return expTy(NULL, NULL);
            }
            else
            {
                if (left.ty == NULL && right.ty != NULL)
                    EM_error(e->pos, "type.lhs");
                else if (left.ty != NULL && right.ty == NULL)
                    EM_error(e->pos, "type.rhs");
                if (left.ty == NULL && right.ty == NULL)
                    EM_error(e->pos, "type.bhs");
            }
        }
        if (oper == A_neqOp)
        {
            if (left.ty != NULL && right.ty != NULL)
            {
                if (left.ty->kind == Ty_short && right.ty->kind == Ty_int)
                    return expTy(Tr_OpExp(e->pos, convertType(right.ty), T_ne, T_ConvertExp(T_i64, left.exp->u.exp), right.exp->u.exp), Ty_Bool());
                if (left.ty->kind == Ty_int && right.ty->kind == Ty_short)
                    return expTy(Tr_OpExp(e->pos, convertType(right.ty), T_ne, left.exp->u.exp, T_ConvertExp(T_i64, right.exp->u.exp)), Ty_Bool());
                if (left.ty->kind == Ty_short && right.ty->kind == Ty_short)
                    return expTy(Tr_OpExp(e->pos, convertType(left.ty), T_ne, left.exp->u.exp, right.exp->u.exp), Ty_Bool());
                if (left.ty->kind == Ty_int && right.ty->kind == Ty_int)
                    return expTy(Tr_OpExp(e->pos, convertType(left.ty), T_ne, left.exp->u.exp, right.exp->u.exp), Ty_Bool());
                if (left.ty->kind == Ty_real && (right.ty->kind == Ty_int || right.ty->kind == Ty_short))
                    return expTy(Tr_OpExp(e->pos, convertType(left.ty), T_ne, left.exp->u.exp, T_ConvertExp(T_f64, right.exp->u.exp)), Ty_Bool());
                if ((left.ty->kind == Ty_int || left.ty->kind == Ty_short) && right.ty->kind == Ty_real)
                    return expTy(Tr_OpExp(e->pos, convertType(right.ty), T_ne, T_ConvertExp(T_f64, left.exp->u.exp), right.exp->u.exp), Ty_Bool());
                if (left.ty->kind == Ty_real && right.ty->kind == Ty_real)
                    return expTy(Tr_OpExp(e->pos, convertType(left.ty), T_ne, left.exp->u.exp, right.exp->u.exp), Ty_Bool());
                if(left.ty -> kind == right.ty -> kind){
                    return expTy(Tr_OpExp(e->pos, convertType(left.ty), T_ne, left.exp->u.exp, right.exp->u.exp), Ty_Bool());
                }
                EM_error(e->u.op.left->pos, "type.comp");
                return expTy(NULL, NULL);
            }
            else
            {
                if (left.ty == NULL && right.ty != NULL)
                    EM_error(e->pos, "type.lhs");
                else if (left.ty != NULL && right.ty == NULL)
                    EM_error(e->pos, "type.rhs");
                if (left.ty == NULL && right.ty == NULL)
                    EM_error(e->pos, "type.bhs");
            }
        }
        if (oper == A_leOp)
        {
            if (left.ty != NULL && right.ty != NULL)
            {
                if (left.ty->kind == Ty_short && right.ty->kind == Ty_int)
                    return expTy(Tr_OpExp(e->pos, convertType(right.ty), T_le_s, T_ConvertExp(T_i64, left.exp->u.exp), right.exp->u.exp), Ty_Bool());
                if (left.ty->kind == Ty_int && right.ty->kind == Ty_short)
                    return expTy(Tr_OpExp(e->pos, convertType(right.ty), T_le_s, left.exp->u.exp, T_ConvertExp(T_i64, right.exp->u.exp)), Ty_Bool());
                if (left.ty->kind == Ty_short && right.ty->kind == Ty_short)
                    return expTy(Tr_OpExp(e->pos, convertType(left.ty), T_le_s, left.exp->u.exp, right.exp->u.exp), Ty_Bool());
                if (left.ty->kind == Ty_int && right.ty->kind == Ty_int)
                    return expTy(Tr_OpExp(e->pos, convertType(left.ty), T_le_s, left.exp->u.exp, right.exp->u.exp), Ty_Bool());
                if (left.ty->kind == Ty_real && (right.ty->kind == Ty_int || right.ty->kind == Ty_short))
                    return expTy(Tr_OpExp(e->pos, convertType(left.ty), T_le_s, left.exp->u.exp, T_ConvertExp(T_f64, right.exp->u.exp)), Ty_Bool());
                if ((left.ty->kind == Ty_int || left.ty->kind == Ty_short) && right.ty->kind == Ty_real)
                    return expTy(Tr_OpExp(e->pos, convertType(right.ty), T_le_s, T_ConvertExp(T_f64, left.exp->u.exp), right.exp->u.exp), Ty_Bool());
                if (left.ty->kind == Ty_real && right.ty->kind == Ty_real)
                    return expTy(Tr_OpExp(e->pos, convertType(left.ty), T_le_s, left.exp->u.exp, right.exp->u.exp), Ty_Bool());
                EM_error(e->u.op.left->pos, "type.comp");
                return expTy(NULL, NULL);
            }
            else
            {
                if (left.ty == NULL && right.ty != NULL)
                    EM_error(e->pos, "type.lhs");
                else if (left.ty != NULL && right.ty == NULL)
                    EM_error(e->pos, "type.rhs");
                if (left.ty == NULL && right.ty == NULL)
                    EM_error(e->pos, "type.bhs");
            }
        }
        if (oper == A_ltOp)
        {
            if (left.ty != NULL && right.ty != NULL)
            {
                if (left.ty->kind == Ty_short && right.ty->kind == Ty_int)
                    return expTy(Tr_OpExp(e->pos, convertType(right.ty), T_lt_s, T_ConvertExp(T_i64, left.exp->u.exp), right.exp->u.exp), Ty_Bool());
                if (left.ty->kind == Ty_int && right.ty->kind == Ty_short)
                    return expTy(Tr_OpExp(e->pos, convertType(right.ty), T_lt_s, left.exp->u.exp, T_ConvertExp(T_i64, right.exp->u.exp)), Ty_Bool());
                if (left.ty->kind == Ty_short && right.ty->kind == Ty_short)
                    return expTy(Tr_OpExp(e->pos, convertType(left.ty), T_lt_s, left.exp->u.exp, right.exp->u.exp), Ty_Bool());
                if (left.ty->kind == Ty_int && right.ty->kind == Ty_int)
                    return expTy(Tr_OpExp(e->pos, convertType(left.ty), T_lt_s, left.exp->u.exp, right.exp->u.exp), Ty_Bool());
                if (left.ty->kind == Ty_real && (right.ty->kind == Ty_int || right.ty->kind == Ty_short))
                    return expTy(Tr_OpExp(e->pos, convertType(left.ty), T_lt_s, left.exp->u.exp, T_ConvertExp(T_f64, right.exp->u.exp)), Ty_Bool());
                if ((left.ty->kind == Ty_int || left.ty->kind == Ty_short) && right.ty->kind == Ty_real)
                    return expTy(Tr_OpExp(e->pos, convertType(right.ty), T_lt_s, T_ConvertExp(T_f64, left.exp->u.exp), right.exp->u.exp), Ty_Bool());
                if (left.ty->kind == Ty_real && right.ty->kind == Ty_real)
                    return expTy(Tr_OpExp(e->pos, convertType(left.ty), T_lt_s, left.exp->u.exp, right.exp->u.exp), Ty_Bool());
                EM_error(e->u.op.left->pos, "type.comp");
                return expTy(NULL, NULL);
            }
            else
            {
                if (left.ty == NULL && right.ty != NULL)
                    EM_error(e->pos, "type.lhs");
                else if (left.ty != NULL && right.ty == NULL)
                    EM_error(e->pos, "type.rhs");
                if (left.ty == NULL && right.ty == NULL)
                    EM_error(e->pos, "type.bhs");
            }
        }
        if (oper == A_geOp)
        {
            if (left.ty != NULL && right.ty != NULL)
            {
                if (left.ty->kind == Ty_short && right.ty->kind == Ty_int)
                    return expTy(Tr_OpExp(e->pos, convertType(right.ty), T_ge_s, T_ConvertExp(T_i64, left.exp->u.exp), right.exp->u.exp), Ty_Bool());
                if (left.ty->kind == Ty_int && right.ty->kind == Ty_short)
                    return expTy(Tr_OpExp(e->pos, convertType(right.ty), T_ge_s, left.exp->u.exp, T_ConvertExp(T_i64, right.exp->u.exp)), Ty_Bool());
                if (left.ty->kind == Ty_short && right.ty->kind == Ty_short)
                    return expTy(Tr_OpExp(e->pos, convertType(left.ty), T_ge_s, left.exp->u.exp, right.exp->u.exp), Ty_Bool());
                if (left.ty->kind == Ty_int && right.ty->kind == Ty_int)
                    return expTy(Tr_OpExp(e->pos, convertType(left.ty), T_ge_s, left.exp->u.exp, right.exp->u.exp), Ty_Bool());
                if (left.ty->kind == Ty_real && (right.ty->kind == Ty_int || right.ty->kind == Ty_short))
                    return expTy(Tr_OpExp(e->pos, convertType(left.ty), T_ge_s, left.exp->u.exp, T_ConvertExp(T_f64, right.exp->u.exp)), Ty_Bool());
                if ((left.ty->kind == Ty_int || left.ty->kind == Ty_short) && right.ty->kind == Ty_real)
                    return expTy(Tr_OpExp(e->pos, convertType(right.ty), T_ge_s, T_ConvertExp(T_f64, left.exp->u.exp), right.exp->u.exp), Ty_Bool());
                if (left.ty->kind == Ty_real && right.ty->kind == Ty_real)
                    return expTy(Tr_OpExp(e->pos, convertType(left.ty), T_ge_s, left.exp->u.exp, right.exp->u.exp), Ty_Bool());
                EM_error(e->u.op.left->pos, "type.comp");
                return expTy(NULL, NULL);
            }
            else
            {
                if (left.ty == NULL && right.ty != NULL)
                    EM_error(e->pos, "type.lhs");
                else if (left.ty != NULL && right.ty == NULL)
                    EM_error(e->pos, "type.rhs");
                if (left.ty == NULL && right.ty == NULL)
                    EM_error(e->pos, "type.bhs");
            }
        }
        if (oper == A_gtOp)
        {
            if (left.ty != NULL && right.ty != NULL)
            {
                if (left.ty->kind == Ty_short && right.ty->kind == Ty_int)
                    return expTy(Tr_OpExp(e->pos, convertType(right.ty), T_gt_s, T_ConvertExp(T_i64, left.exp->u.exp), right.exp->u.exp), Ty_Bool());
                if (left.ty->kind == Ty_int && right.ty->kind == Ty_short)
                    return expTy(Tr_OpExp(e->pos, convertType(right.ty), T_gt_s, left.exp->u.exp, T_ConvertExp(T_i64, right.exp->u.exp)), Ty_Bool());
                if (left.ty->kind == Ty_short && right.ty->kind == Ty_short)
                    return expTy(Tr_OpExp(e->pos, convertType(left.ty), T_gt_s, left.exp->u.exp, right.exp->u.exp), Ty_Bool());
                if (left.ty->kind == Ty_int && right.ty->kind == Ty_int)
                    return expTy(Tr_OpExp(e->pos, convertType(left.ty), T_gt_s, left.exp->u.exp, right.exp->u.exp), Ty_Bool());
                if (left.ty->kind == Ty_real && (right.ty->kind == Ty_int || right.ty->kind == Ty_short))
                    return expTy(Tr_OpExp(e->pos, convertType(left.ty), T_gt_s, left.exp->u.exp, T_ConvertExp(T_f64, right.exp->u.exp)), Ty_Bool());
                if ((left.ty->kind == Ty_int || left.ty->kind == Ty_short) && right.ty->kind == Ty_real)
                    return expTy(Tr_OpExp(e->pos, convertType(right.ty), T_gt_s, T_ConvertExp(T_f64, left.exp->u.exp), right.exp->u.exp), Ty_Bool());
                if (left.ty->kind == Ty_real && right.ty->kind == Ty_real)
                    return expTy(Tr_OpExp(e->pos, convertType(left.ty), T_gt_s, left.exp->u.exp, right.exp->u.exp), Ty_Bool());
                EM_error(e->u.op.left->pos, "type.comp");
                return expTy(NULL, NULL);
            }
            else
            {
                if (left.ty == NULL && right.ty != NULL)
                    EM_error(e->pos, "type.lhs");
                else if (left.ty != NULL && right.ty == NULL)
                    EM_error(e->pos, "type.rhs");
                if (left.ty == NULL && right.ty == NULL)
                    EM_error(e->pos, "type.bhs");
            }
        }
        if (oper == A_andOp)
        {
            if (left.ty->kind != Ty_bool)
            {
                EM_error(e->pos, "type.lhsnotbool");
            }
            if (right.ty->kind != Ty_bool)
            {
                EM_error(e->pos, "type.rhsnotbool");
            }
            return expTy(Tr_OpExp(e->pos, convertType(left.ty), T_and, left.exp->u.exp, right.exp->u.exp), Ty_Bool());
        }
        if (oper == A_orOp)
        {
            if (left.ty->kind != Ty_bool)
            {
                EM_error(e->pos, "type.lhsnotbool");
            }
            if (right.ty->kind != Ty_bool)
            {
                EM_error(e->pos, "type.rhsnotbool");
            }
            return expTy(Tr_OpExp(e->pos, convertType(left.ty), T_or, left.exp->u.exp, right.exp->u.exp), Ty_Bool());
        }
        if (oper == A_modOp)
        {
            if (left.ty != NULL && right.ty != NULL)
            {
                if (left.ty->kind == Ty_int && right.ty->kind == Ty_int)
                    return expTy(Tr_OpExp(e->pos, convertType(left.ty), T_rem_s, left.exp->u.exp, right.exp->u.exp), Ty_Int());
                if (left.ty->kind == Ty_int && right.ty->kind == Ty_short)
                    return expTy(Tr_OpExp(e->pos, convertType(left.ty), T_rem_s, left.exp->u.exp, T_ConvertExp(T_i64, right.exp->u.exp)), Ty_Int());
                if (left.ty->kind == Ty_short && right.ty->kind == Ty_int)
                    return expTy(Tr_OpExp(e->pos, convertType(right.ty), T_rem_s, T_ConvertExp(T_i64, left.exp->u.exp), right.exp->u.exp), Ty_Int());
                else
                {
                    EM_error(e->u.op.left->pos, "type.mod");
                    return expTy(NULL, NULL);
                }
            }
            else
            {
                if (left.ty == NULL && right.ty != NULL)
                    EM_error(e->pos, "type.lhs");
                else if (left.ty != NULL && right.ty == NULL)
                    EM_error(e->pos, "type.rhs");
                if (left.ty == NULL && right.ty == NULL)
                    EM_error(e->pos, "type.bhs");
            }
        }
        break;
    }
    }
}

T_module transDec(S_table venv, S_table tenv, A_dec d, Tr_level level, bool isLoop, S_table table, Ty_ty classs)
{
    assert(d);
    switch (d->kind)
    {
    case A_operatorDec:{
        string opId = "";
        switch(d -> u.op.oper){
            case A_plusOp:
                opId = "+";
                break;
            case A_minusOp:
                opId = "-";
                break;
            case A_timesOp:
                opId = "*";
                break;
            case A_divideOp:
                opId = "/";
                break;
            case A_modOp:
                opId = "%";
                break;
            case A_eqOp:
                opId = "==";
                break;
            case A_neqOp:
                opId = "!=";
                break;
            case A_ltOp:
                opId = "<";
                break;
            case A_leOp:
                opId = "<=";
                break;
            case A_gtOp:
                opId = ">";
                break;
            case A_geOp:
                opId = ">=";
                break;
            case A_andOp:
                opId = "&&";
                break;
            case A_orOp:
                opId = "||";
                break;
            case A_assignOp:
                opId = "=";
                break;
        }
        return transDec(venv, tenv, A_FunctionDec(d -> pos, A_FundecList(A_Fundec(d -> pos, S_Symbol(opId), d -> u.op.params, d -> u.op.result, d -> u.op.body), NULL)), level, isLoop, table, classs);
    }
    case A_varDec:
    {
        struct expty exp;
        if(d -> u.var.var -> kind == A_subscriptVar)
        {
            for(A_var var = d -> u.var.var; var -> kind == A_subscriptVar; var = var -> u.subscript.var)
            {
                d -> u.var.typ = A_ArrayTy(d -> pos, d -> u.var.typ, var -> u.subscript.exp -> u.intt);
            }
            // printf("dasfdas\n");
        }
        E_enventry varEntry = (E_enventry)S_look(venv, d->u.var.var->u.simple);
        E_enventry typeEntry = (E_enventry)S_look(tenv, d->u.var.var->u.simple);
        S_symbol sym = NULL;
        if (varEntry != NULL)
        {
            //debug(d->pos, "Variable or function of name %s is already declared.", S_name(d->u.var.var->u.simple));
        }
        if (typeEntry != NULL)
        {
            EM_error(d->pos, "type.alreadyexists %s", S_name(d->u.var.var->u.simple));
        }
        if (d->u.var.init != NULL)
        {
            Ty_ty type = transTy(venv, tenv, d->u.var.typ, level);
            if (d->u.var.var->kind == A_simpleVar)
            {
                //debug(d->pos, "Variable declared with value.");
                S_enter(venv, d->u.var.var->u.simple, E_VarEntry(Tr_allocLocal(level, ESC_checkEscapeFromType(type), type), type));
            }
            else if (d->u.var.var->kind == A_subscriptVar)
            {
                S_enter(venv, d->u.var.var->u.subscript.name, E_VarEntry(Tr_allocLocal(level, TRUE, type), type));
            }
        }
        else if (d->u.var.init == NULL)
        {
            if (d->u.var.var->kind == A_simpleVar){
                Ty_ty type = transTy(venv, tenv, d->u.var.typ, level);
                // printf("%d memorySize\n", memorySize);
                S_enter(venv, d->u.var.var->u.simple, E_VarEntry(Tr_allocLocal(level, ESC_checkEscapeFromType(type), type), type));
            }
            else if (d->u.var.var->kind == A_subscriptVar)
            {
                assert(d->u.var.var->u.subscript.name);
                Ty_ty type = transTy(venv, tenv, d -> u.var.typ, level);
                S_enter(venv, d->u.var.var->u.subscript.name, E_VarEntry(Tr_allocLocal(level, TRUE, type), type));
            }
        }
        if (!(level->parent))
        {
            return T_GlobalMod(convertType(transTy(venv, tenv, d->u.var.typ, level)), transExp(venv, tenv, d->u.var.init, level, isLoop, classs).exp->u.exp);
        }
        return NULL;
        break;
    }
    case A_functionDec:
    {
        A_fundec func = d->u.function->head;
        Ty_tyList paramTypes = makeParamTypeList(venv, tenv, func->params, level);
        Tr_level newLevel = NULL;
        if(venv == table){
            newLevel = Tr_newLevel(level, func->name, makeEscapeList(func->params), paramTypes, FALSE, Ty_Void());
        }
        else{
            newLevel = Tr_newLevel(level, func->name, makeEscapeList(func->params), paramTypes, TRUE, classs);
        }
        A_fieldList returnList = func->result;
        if (returnList->tail)
            EM_error(d->pos, "notsupport.multiplereturn");
        T_typeList newTypeList = T_TypeList(T_none, NULL);
        T_typeList result = newTypeList;
        E_enventry entry = NULL;
        A_field resultVar = NULL;
        if (returnList->head)
        {
            resultVar = returnList->head;
            Ty_ty type = transTy(venv, tenv, resultVar -> typ, level);
            entry = E_FuncEntry(level, func->name, paramTypes, Tr_allocLocal(newLevel, resultVar->escape, type), type);
        }
        else
            entry = E_FuncEntry(level, func->name, paramTypes, Tr_allocLocal(newLevel, FALSE, Ty_Void()), Ty_Void());
        funcs--;
        E_enventry funcEntry = NULL;
        if(!(funcEntry = (E_enventry)S_look(venv, func -> name))){
            S_enter(table, func->name, entry);
        }
        else if(funcEntry -> kind == E_funcentry){
            E_enventry newEntry = E_ListEntry(funcEntry);
            newEntry -> u.list -> push_back(entry);
            S_enter(table, func -> name, newEntry);
        }
        else if(funcEntry -> kind == E_listEntry){
            funcEntry -> u.list -> push_back(entry);
        }

        S_beginScope(venv);
        {
            if (resultVar){
                S_enter(venv, getName(resultVar->var), E_VarEntry(entry->u.func.result, entry->u.func.returnType));
            }

            A_fieldList list = NULL;
            Ty_tyList typeList = NULL;
            Tr_accessList accessList = NULL;
            T_stmList assignList = T_StmList(NULL, NULL);
            T_stmList resultAssignList = assignList;
            for (accessList = newLevel->formals->tail, list = func->params, typeList = paramTypes; list != NULL && typeList != NULL &&
                accessList != NULL;
                list = list->tail, typeList = typeList->tail, accessList = accessList->tail)
            {
                newTypeList->head = convertType(typeList->head);
                newTypeList->tail = T_TypeList(T_none, NULL);
                newTypeList = newTypeList->tail;
                newTypeList->head = T_none;
                newTypeList->tail = NULL;
                list->head->escape = ESC_checkEscapeFromType(typeList->head);
                if (typeList->head != NULL)
                    S_enter(venv, getName(list->head->var), E_VarEntry(accessList->head, typeList->head));
                if(typeList -> head -> kind == Ty_name){
                    int addr = accessList -> head -> access -> u.offset + accessList -> head -> level -> frame -> offset;
                    T_stm assign = T_CopyStm(Tr_AddrExp(d -> pos, addr)->u.exp, T_GetLocalExp(T_i32, accessList -> head -> access -> paramIndex), Tr_AddrExp(d -> pos, accessList -> head -> access -> type -> size)->u.exp);
                    assignList -> head = assign;
                    assignList -> tail = T_StmList(NULL, NULL);
                    assignList = assignList -> tail;
                    assignList -> head = NULL;
                    assignList -> tail = NULL;
                }
            }
            T_stm assignListStm = Tr_CompoundStm(d -> pos, resultAssignList)-> u.stm;
            struct expty body;
            if (func->body != NULL)
            {
                body = transStm(venv, tenv, func->body, newLevel, isLoop, classs);
            }
            entry -> u.func.index = funcs;
            funcs++;
            S_endScope(venv);
            //debug(d->pos, "Function declared. %s", S_name(func->name));

            T_typeList typeListt = convertAllType(newLevel->frame->localsType);
            string name = "";
            if (strcmp(S_name(d->u.function->head->name), _mainFuncName.c_str()) == 0)
            {
                name = "main";
            }
            else if (strcmp(S_name(d->u.function->head->name), "loop") == 0)
            {
                name = "loop";
            }
            T_stm resultBody = NULL;
            if(resultAssignList -> head){
              //printf("19922222222222222222222222222222222222222\n");
                if(body.exp -> u.stm -> kind == T_seqStm)
                    resultBody = Tr_CompoundStm(d -> pos, T_StmList(assignListStm, body.exp -> u.stm -> u.seq))->u.stm;
                else
                    resultBody = Tr_CompoundStm(d -> pos, T_StmList(assignListStm, T_StmList(body.exp -> u.stm, NULL)))->u.stm;
            }
            else{
                resultBody = body.exp -> u.stm;
            }
            if (entry->u.func.result){
                if(entry -> u.func.returnType -> kind == Ty_name){
                    return T_FuncMod(T_Fundec(result, typeListt, convertType(transTy(venv, tenv, resultVar->typ, level)),
                    resultBody, name, entry->u.func.index,
                    Tr_VarExp(d->pos, convertType(entry->u.func.returnType), entry->u.func.result, FALSE, FALSE)->u.exp -> u.load.addr));
                }
                return T_FuncMod(T_Fundec(result, typeListt, convertType(transTy(venv, tenv, resultVar->typ, level)),
                    resultBody, name, entry->u.func.index,
                    Tr_VarExp(d->pos, convertType(entry->u.func.returnType), entry->u.func.result, FALSE, FALSE)->u.exp));
            }
            else
                return T_FuncMod(T_Fundec(result, typeListt, T_none,
                    resultBody, name, entry->u.func.index,
                    NULL));
            break;
        }
    }
    case A_typeDec:
    {
        S_enter(tenv, d->u.type->head->name, transTy(venv, tenv, d->u.type->head->ty, level));
        break;
    }
    case A_funcImportDec:
    {
        Ty_tyList paramTypes = makeParamTypeList(venv, tenv, d -> u.funcImport.params, level);
        if(venv != table){
            EM_error(d -> pos, "notsupport.importinclass");
        }
        Tr_level newLevel = Tr_newLevel(level, d -> u.funcImport.name, makeEscapeList(d -> u.funcImport.params), paramTypes, FALSE, Ty_Void());
        A_fieldList returnList = d -> u.funcImport.result;
        if (returnList->tail)
            EM_error(d->pos, "notsupport.multiplereturn");
        E_enventry entry = NULL;
        A_field resultVar = NULL;
        // printf("%s func->name\n", S_name(d -> u.funcImport.name));
        if (returnList->head)
        {
            // printf("there is head\n");
            resultVar = returnList->head;
            Ty_ty type = transTy(venv, tenv, resultVar->typ, level);
            entry = E_FuncEntry(level, d -> u.funcImport.name, paramTypes, Tr_allocLocal(newLevel, resultVar->escape, type), type);
        }
        else
            entry = E_FuncEntry(level, d -> u.funcImport.name, paramTypes, Tr_allocLocal(newLevel, FALSE, Ty_Void()), Ty_Void());

        S_enter(venv, d -> u.funcImport.name, entry);
        T_module func = NULL;
        if(returnList -> head != NULL)
        {
            func = T_FuncMod(T_Fundec(convertAllType(paramTypes), NULL, convertType(transTy(venv, tenv, resultVar->typ, level)),
                        NULL, S_name(d -> u.funcImport.name), entry->u.func.index, NULL));
        }
        else
        {
            func = T_FuncMod(T_Fundec(convertAllType(paramTypes), NULL, convertType(Ty_Void()),
                        NULL, S_name(d -> u.funcImport.name), entry->u.func.index, NULL));
        }
        return T_ImportMod(d -> u.funcImport.mod, d -> u.funcImport.func, func);
    }
    case A_funcExportDec:
    {
        // printf("%s\n", S_name(d -> u.funcExport.name));
        E_enventry funcEntry = (E_enventry)S_look(venv, d -> u.funcExport.name);
        if(funcEntry){
            return T_ExportMod(d -> u.funcExport.exportName, funcEntry -> u.func.index);
        }
        else{
            EM_error(d -> pos, "noexist.func %s", S_name(d -> u.funcExport.name));
        }
    }
    case A_classDec:
    {
        S_table varTypes = S_empty();
        S_table methods = S_empty();
        int classSize = 0;
        if(d -> u.classs.inheritance){
            //maybe make a S_table copy function using S_dump()

            E_enventry classEntry = (E_enventry)S_look(tenv, d -> u.classs.inheritance -> head);
            if(classEntry){
                tempTable = S_empty();
                S_dump(classEntry -> u.classs.varTypes, &copyTable);
                varTypes = tempTable;
                tempTable = S_empty();
                S_dump(classEntry -> u.classs.methods, &copyTable);
                methods = tempTable;
                classSize = classEntry -> u.classs.size;
            }
        }

        A_classMemberList memberList = d -> u.classs.members;
        // memberList = flipClassMemberList(memberList);
        for(; memberList -> tail; memberList = memberList -> tail)
        {
            // printf("debug %d\n", memberList -> head -> dec -> kind);
            if(memberList -> head -> dec -> kind == A_varDec || memberList -> head -> dec -> kind == A_objectDec)
            {
                Ty_ty varType = NULL;
                
                S_symbol name = NULL;
                if(memberList -> head -> dec -> kind == A_varDec){
                    A_ty type = memberList -> head -> dec -> u.var.typ; 
                    if(memberList -> head -> dec -> u.var.var -> kind == A_simpleVar){
                        name = memberList -> head -> dec -> u.var.var -> u.simple;
                    }
                    else if(memberList -> head -> dec -> u.var.var -> kind == A_subscriptVar){
                        for(A_var var = memberList -> head -> dec -> u.var.var; var -> kind == A_subscriptVar; var = var -> u.subscript.var)
                        {
                            type = A_ArrayTy(memberList -> head -> dec -> pos, type, var -> u.subscript.exp -> u.intt);
                        }
                        // printf("dasfdas\n");
                        name = memberList -> head -> dec -> u.var.var -> u.subscript.name;
                    }
                    else
                        EM_error(memberList -> head -> dec -> pos, "code.declaredoesntwork");
                        
                    varType = transTy(venv, tenv, type, level);
                }
                else if(memberList -> head -> dec -> kind == A_objectDec){
                    name = memberList -> head -> dec -> u.object.name;
                    varType = transTy(venv, tenv, memberList -> head -> dec -> u.object.className, level);
                }
                
                // printf("classSize %d\n", classSize);
                Ty_accessSpecifier specifier = Ty_public;
                if(memberList -> head -> accessSpecifier == A_public) specifier = Ty_public;
                else if(memberList -> head -> accessSpecifier == A_private) specifier = Ty_private;
                else if(memberList -> head -> accessSpecifier == A_protected) specifier = Ty_protected;
                // printf("%d classSize\n", classSize);
                S_enter(varTypes, name, Ty_Member(classSize, varType, specifier));
                // printf("varTYpe -> size %d\n", varType -> size);
                classSize += varType -> size;
            }
        }
        Ty_ty type = NULL;
        if(classs -> kind == Ty_poly)
        {
            type = Ty_Poly(classs -> u.poly.name, classs -> u.poly.typeParam, classSize);
        }
        else
        {
            type = Ty_Name(d -> u.classs.name, classSize);
        }
        if(classs -> kind != Ty_poly){
            S_enter(tenv, d -> u.classs.name, E_ClassEntry(d -> u.classs.name, classSize, varTypes, methods, type));
        }
        else if(classs -> kind == Ty_poly){
            E_enventry templateEntry = (E_enventry)S_look(tenv, d -> u.classs.name);
            TAB_insert(templateEntry -> u.templatee.specific, classs -> u.poly.typeParam, E_ClassEntry(d -> u.classs.name, classSize, varTypes, methods, type));
            S_enter(tenv, d -> u.classs.name, templateEntry);
        }
        T_moduleList modlist = T_ModuleList(NULL, NULL);
        memberList= d -> u.classs.members;
        // memberList= flipClassMemberList(memberList);
        for(; memberList-> tail; memberList= memberList-> tail)
        {
            if(memberList-> head -> dec -> kind == A_functionDec || memberList -> head -> dec -> kind == A_operatorDec)
            {
                // printf("%s funcname\n", S_name(memberList-> head -> dec -> u.function->head->name));

                T_module mod = transDec(venv, tenv, memberList -> head -> dec, level, isLoop, methods, type);
                mod -> u.func -> params = T_TypeList(T_i32, mod -> u.func -> params);
                
                modlist = T_ModuleList(mod, modlist);
                list -> head = mod;
                list -> tail = T_ModuleList(NULL, NULL);
                list = list -> tail;
                list -> head = NULL;
                list -> tail = NULL;
                
                
                // S_enter(tenv, d -> u.classs.name, E_ClassEntry(d -> u.classs.name, classSize, varTypes, methods));
                if(classs -> kind != Ty_poly){
                    S_enter(tenv, d -> u.classs.name, E_ClassEntry(d -> u.classs.name, classSize, varTypes, methods, type));
                }
                else if(classs -> kind == Ty_poly){
                    E_enventry templateEntry = (E_enventry)S_look(tenv, d -> u.classs.name);
                    TAB_insert(templateEntry -> u.templatee.specific, classs -> u.poly.typeParam, E_ClassEntry(d -> u.classs.name, classSize, varTypes, methods, type));
                    S_enter(tenv, d -> u.classs.name, templateEntry);
                }
            }
        }
        // templatee declartaion is before T_SeqMod(modlist)
      //printf("-------------------------------CLASS DECLARE ENDED--------------------------------\n");
        modlist = flipModList(modlist);
        return NULL;
    }
    case A_objectDec:
    {
        // printf("cursor pos %d", d -> pos);
        // printf("line %d column %d", L_getErrorPos(d -> pos).lineNum, L_getErrorPos(d -> pos).columnNum);
        transDec(venv, tenv, A_VarDec(d -> pos, A_AssignStm(d -> pos, A_SimpleVar(d -> pos, d -> u.object.name), NULL, TRUE), d -> u.object.className), level, isLoop, table, classs);
        return NULL;
        break;
    }
    case A_templateDec:
    {
        E_enventry entry = E_TemplateEntry(d -> u.templatee.name, d -> u.templatee.dec);
        S_enter(tenv, d -> u.templatee.name, E_PolyEntry(Ty_Void()));
        if(d -> u.templatee.dec -> kind == A_functionDec)
        {
            S_enter(venv, d -> u.templatee.dec -> u.function -> head -> name, entry);
        }
        else if(d -> u.templatee.dec -> kind == A_classDec)
        {
            tempTable = S_empty();
            S_dump(venv, &copyTable);
            entry -> u.templatee.venv = tempTable;
            tempTable = S_empty();
            S_dump(tenv, &copyTable);
            entry -> u.templatee.tenv = tempTable;
            //update tenv with the templatee name
            S_enter(tenv, d -> u.templatee.dec -> u.classs.name, entry);
            tempTable = S_empty();
            S_dump(tenv, &copyTable);
            //load the tenv with the templatee name
            entry -> u.templatee.tenv = tempTable;
            //update tenv with the templatee that has the templatee name in its tenv
            S_enter(tenv, d -> u.templatee.dec -> u.classs.name, entry);
        }
        else
        {
            EM_error(d -> pos, "code.templatenotsupport");
        }
        return NULL;
    }
    }
}

Ty_ty transTy(S_table venv, S_table tenv, A_ty ty, Tr_level level)
{
    static int arrayDepth = 0; 
    switch (ty->kind)
    {
    case A_nameTy:
    {
        Ty_ty type = NULL;
        E_enventry entry = NULL;
        if (type = checkSymType(ty->u.name))
        {
            return type;
        }
        else
        {
            entry = (E_enventry)S_look(tenv, ty->u.name);
            if (entry)
            {
                if(entry -> kind == E_classentry){
                    return entry -> u.classs.type;
                }
                else if(entry -> kind == E_polyentry){
                    if(entry -> u.poly == Ty_Void())
                        return Ty_Name(ty -> u.name, 0);
                    return entry -> u.poly;
                }
            }
            else{
                EM_error(ty -> pos, "noexist.type %s", S_name(ty -> u.name));
            }
        }
    }
    case A_pointerTy:
    {
        Ty_ty type = NULL;
        E_enventry entry = NULL;
        if (type = transTy(venv, tenv, ty->u.pointer, level))
        {
            type = Ty_Pointer(type);
            return type;
        }
        else
        {
            entry = (E_enventry)S_look(tenv, ty->u.name);
            if (entry)
            {
                return Ty_Name(ty->u.name, entry -> u.classs.size);
            }
            return Ty_Pointer(type);
        }
    }
    case A_arrayTy:
    {
        // printf("%d arraydepth\n", arrayDepth);
        // if(arrayDepth==0)
        //     ty=reverseArrayTy(ty);
        arrayDepth++;
        Ty_ty type = transTy(venv, tenv, ty->u.array.type, level);
        arrayDepth = 0;
        int a = ty -> u.array.size;
        // printf("%d size kind1420\n", ty -> u.array.size);
        if (type)
        {
                    // printf("A_arrayTy\n");

            type = Ty_Array(type, ty->u.array.size);
            // printf("%d 1424\n", type -> u.array.type -> size);
            return type;
        }
        else
        {
            type = (Ty_ty)S_look(tenv, ty->u.name);
            if (!type)
            {
                EM_error(ty->pos, "noexist.type %s", S_name(ty -> u.name));
            }
            return Ty_Array(type, ty->u.array.size);
        }
    }
    case A_polyTy:
    {
        Ty_ty typeParam = transTy(venv, tenv, ty -> u.poly.typeParam, level);
        E_enventry templateEntry = (E_enventry)S_look(tenv, ty -> u.poly.name);
        if(!templateEntry){
            EM_error(ty -> pos, "noexist.template %s", S_name(ty -> u.poly.name));
        }
        if(templateEntry -> kind == E_templateentry)
        {
            E_enventry classEntry = (E_enventry)TAB_search(templateEntry -> u.templatee.specific, typeParam);
            if(!classEntry)
            {
                // if(typeParam -> kind == Ty_name)
                    // printf("%s %s 2074444444444444444444444444444444444444444444444444444444444444444444\n", S_name(ty -> u.poly.name), S_name(typeParam -> u.name.sym));
                //update the polymorphic type with type parameter
                S_enter(templateEntry -> u.templatee.tenv, templateEntry -> u.templatee.name, E_PolyEntry(typeParam));
                //translate the declaration using the type parameter
                int temp = memorySize;
                T_module dec = transDec(templateEntry -> u.templatee.venv, templateEntry -> u.templatee.tenv, templateEntry -> u.templatee.dec, Tr_outermost(), FALSE, templateEntry -> u.templatee.venv, Ty_Poly(ty -> u.poly.name, typeParam, 0));
                level -> frame -> frameSize += (memorySize - temp);
                // E_enventry tempEntry = (E_enventry)S_look(templateEntry -> u.templatee.tenv, ty -> u.poly.name);
                // S_enter(tenv, ty -> u.poly.name, tempEntry);
                list -> head = dec;
                list -> tail = T_ModuleList(NULL, NULL);
                list = list -> tail;
                list -> head = NULL;
                list -> tail = NULL;
                S_enter(tenv, ty -> u.poly.name, templateEntry);
                // E_enventry tempEntry = (E_enventry)S_look(tenv, ty -> u.poly.name);
                // classEntry = (E_enventry)TAB_search(tempEntry  -> u.templatee.specific, typeParam);
                // printf("%d classEntry check\n", classEntry);
            }
            classEntry = (E_enventry)TAB_search(templateEntry -> u.templatee.specific, typeParam);
            return Ty_Poly(ty -> u.poly.name, typeParam, classEntry -> u.classs.size);
        }
        else
            EM_error(ty -> pos, "noexist.template %s", ty -> u.poly.name);
    }
    case A_funcTy:
    {
        Ty_tyList paramTyList = makeParamTypeList(venv, tenv, ty -> u.func.params, level);
        Ty_ty resultTy = Ty_Void();
        if(ty -> u.func.result -> head)
            resultTy = transTy(venv, tenv, ty -> u.func.result -> head -> typ, level);
        typeList -> head = T_TypeMod(convertAllType(paramTyList), convertType(resultTy));
        typeList -> tail = T_ModuleList(NULL, NULL);
        typeList = typeList -> tail;
        typeList -> head = NULL;
        typeList -> tail = NULL;
        typeIndex++;
        return Ty_Func(paramTyList, resultTy, typeIndex - 1);
    }
    }
}

static Ty_ty actual_ty(Ty_ty ty)
{
    if (ty->kind == Ty_pointer)
    {
        return actual_ty(ty->u.pointer);
    }
    else if (ty->kind == Ty_array)
    {
        return actual_ty(ty->u.array.type);
    }
    return ty;
}

static Ty_tyList makeParamTypeList(S_table venv, S_table tenv, A_fieldList params, Tr_level level)
{
    if (params == NULL){
        return NULL;
    }
    Ty_tyList typeList = Ty_TyList(NULL, NULL);
    //make typeList_head point to typeList
    Ty_tyList typeList_head = typeList;
    A_ty type = NULL;
    A_var var = NULL;
    Ty_ty ty = NULL;
    if (!params->head)
        return NULL;
    for (; params != NULL; params = params->tail)
    {
        var = params -> head -> var;
        type = params->head->typ;
        if(var -> kind == A_simpleVar){

        }
        else if(var -> kind == A_subscriptVar){
            for(; var -> kind == A_subscriptVar; var = var -> u.subscript.var)
            {
                if(var -> u.subscript.exp -> kind!= A_intExp)
                {
                    EM_error(params -> head -> pos, "array.invalidsize");
                }
                type = A_ArrayTy(params -> head -> pos, type, var -> u.subscript.exp -> u.intt);
            }
        }
        else{
            EM_error(params -> head -> pos, "func.unknownfield");
        }

        if (ty = transTy(venv, tenv, type, level))
        {
          //printf("%d ty\n", ty -> kind);
            typeList->head = ty;
        }
        else
        {
            S_symbol sym = returnSymFromType(actual_ty(transTy(venv, tenv, type, level)));
            ty = (Ty_ty)S_look(tenv, sym);
            if (ty != NULL)
            {
                typeList->head = ty;
            }
            else
            {
                EM_error(params->head->pos, "noexist.type %s", S_name(sym));
            }
        }
        typeList->tail = Ty_TyList(NULL, NULL);
        typeList->head = ty;
        typeList = typeList->tail;
        typeList->tail = NULL;
        typeList->head = NULL;
    }

    return typeList_head;
}

static Ty_ty checkSymType(S_symbol sym)
{
    if (sym == S_Symbol("int"))
    {
        return Ty_Int();
    }
    // else if (sym == S_Symbol("string"))
    // {
    //     return Ty_String();
    // }
    else if (sym == S_Symbol("real"))
    {
        return Ty_Real();
    }
    else if (sym == S_Symbol("bool"))
    {
        return Ty_Bool();
    }
    else if (sym == S_Symbol("void"))
    {
        return Ty_Void();
    }
    else if (sym == S_Symbol("char"))
    {
        return Ty_Char();
    }
    else if (sym == S_Symbol("short"))
    {
        return Ty_Short();
    }
    else
    {
        return NULL;
    }
}

static S_symbol returnSymFromType(Ty_ty type)
{
    if (type->kind == Ty_nil)
    {
        return S_Symbol("null");
    }
    else if (type->kind == Ty_int)
    {
        return S_Symbol("int");
    }
    else if (type->kind == Ty_real)
    {
        return S_Symbol("real");
    }
    else if (type->kind == Ty_bool)
    {
        return S_Symbol("bool");
    }
    else if (type->kind == Ty_string)
    {
        return S_Symbol("string");
    }
    else if (type->kind == Ty_void)
    {
        return S_Symbol("void");
    }
    else if (type->kind == Ty_char)
    {
        return S_Symbol("char");
    }
    else if (type->kind == Ty_name)
    {
        return type->u.name.sym;
    }
    else if (type->kind == Ty_pointer)
    {
        return NULL;
    }
    else if (type->kind == Ty_short)
    {
        return S_Symbol("short");
    }
}
static U_boolList makeEscapeList(A_fieldList params)
{
    U_boolList list = U_BoolList(FALSE, NULL);
    U_boolList retValue = list;
    if (!params->head)
        return NULL;
    for (; params; params = params->tail)
    {
        // printf("----------------------%s %d params head name\n", S_name(params -> head -> name), params -> head -> escape);
        list->head = params->head->escape;
        list->tail = U_BoolList(FALSE, NULL);
        list = list->tail;
        list->tail = NULL;
        list->head = T_none;
    }
    return retValue;
}
static T_type convertType(Ty_ty type)
{
    T_type result = T_i64;
    switch (type->kind)
    {
    case Ty_func:
    {
        result = T_i32;
        break;
    }
    // case Ty_record:
    // {
    //     EM_error(0, "Records not supported right now.");
    // }
    case Ty_nil:
    {
        result = T_i32;
        break;
    }
    case Ty_int:
    {
        result = T_i64;
        break;
    }
    case Ty_short:
    {
        result = T_i32;
        break;
    }
    case Ty_array:
    {
        result = T_i32;
        break;
    }
    case Ty_void:
    {
        result = T_none;
        break;
    }
    case Ty_real:
    {
        result = T_f64;
        break;
    }
    case Ty_bool:
    {
        result = T_i32;
        break;
    }
    case Ty_char:
    {
        result = T_i32;
        break;
    }
    case Ty_name:
    {
        if (type->u.name.sym == S_Symbol("int"))
        {
            result = T_i64;
            break;
        }
        else if (type->u.name.sym == S_Symbol("real"))
        {
            result = T_f64;
            break;
        }
        else
        {
            result = T_i32;
            break;
        }
    }
    case Ty_pointer:
    {
        result = T_i32;
        break;
    }
    case Ty_poly:
    {
        result = T_i32;
        break;
    }
    }
    return result;
}
static T_typeList convertAllType(Ty_tyList types)
{
    T_typeList list = T_TypeList(T_none, NULL);
    T_typeList result = list;
    if(!types)
    {
        return result;
    }
    for (; types->tail; types = types->tail)
    {
        list->head = convertType(types->head);
        list->tail = T_TypeList(T_none, NULL);
        list = list->tail;
        list->head = T_none;
        list->tail = NULL;
    }

    return result;
}

static bool compType(Ty_ty lhs, Ty_ty rhs)
{
    // printf("%d %d fldasjfkljo\n", lhs -> kind, rhs -> kind);
    bool result = TRUE;
    if (lhs->kind != rhs->kind)
        result = FALSE;
    else
    {
        if (lhs->kind == Ty_array)
        {
            result = compType(lhs->u.array.type, lhs->u.array.type);
        }
        else if (lhs->kind == Ty_pointer)
        {
            if(rhs -> u.pointer -> kind == Ty_void || lhs -> u.pointer -> kind == Ty_void)
                result = TRUE;
            else
                result = (actual_ty(lhs -> u.pointer )-> kind == actual_ty(rhs->u.pointer)->kind);
            // result = compType(lhs->u.pointer, rhs->u.pointer);
        }
        else if (lhs->kind == Ty_name)
        {
            if (lhs->u.name.sym != rhs->u.name.sym)
            {
                result = FALSE;
            }
            else
                result = TRUE;
        }
        else if(lhs -> kind == Ty_poly)
        {
            if(lhs -> u.poly.name != rhs -> u.poly.name)
            {
                result = FALSE;
            }
            return compType(lhs -> u.poly.typeParam, lhs -> u.poly.typeParam);
        }
    }
    return result;
}

static T_exp convertAssign(Ty_ty type, struct expty exp, int pos)
{
    T_exp result = NULL;
    if (type->kind == Ty_int && exp.ty->kind == Ty_short)
        result = T_ConvertExp(T_i64, exp.exp -> u.exp);
    else if (type->kind == Ty_int && exp.ty->kind == Ty_real)
        result = T_ConvertExp(T_i64, exp.exp -> u.exp);
    else if (type->kind == Ty_real && exp.ty->kind == Ty_int)
        result = T_ConvertExp(T_f64, exp.exp -> u.exp);
    else if (type->kind == Ty_real && exp.ty->kind == Ty_short)
        result = T_ConvertExp(T_f64, exp.exp -> u.exp);
    else if (type->kind == Ty_short && exp.ty->kind == Ty_int)
        result = T_ConvertExp(T_i32, exp.exp -> u.exp);
    else if (type->kind == Ty_short && exp.ty->kind == Ty_real)
        result = T_ConvertExp(T_i32, exp.exp -> u.exp);
    else
        EM_error(pos, "type.nomatch %s %s", S_name(returnSymFromType(actual_ty(exp.ty))), S_name(returnSymFromType(actual_ty(type))));
    return result;
}

static A_ty reverseArrayTy(A_ty type)
{
    if(type -> kind != A_arrayTy)
    {
        EM_error(type -> pos, "type.notarray %s", "この変数");
    }
    A_ty result = NULL;
    A_ty temp = NULL, base = NULL;
    for(temp = type;temp -> kind == A_arrayTy; temp = temp -> u.array.type);
    base = temp;
    // printf("base %d type\n", base -> kind);
    for(temp = type, result = A_ArrayTy(type -> pos, base, temp -> u.array.size);temp -> kind == A_arrayTy; temp = temp -> u.array.type)
    {
        if(temp != type)
            result = A_ArrayTy(type -> pos, result, temp -> u.array.size);
    }
    // printf("%d result\n", result -> u.array.size);
    return result;
}

static A_var reverseSubVar(A_var var)
{
    
    if(!var)
        return NULL;
    if(var -> kind != A_subscriptVar)
        return var;
    
    A_var temp = NULL;
    A_var result = NULL;
    A_expList indexList = A_ExpList(NULL, NULL);
    A_expList indexResult = indexList;
    for(temp = var; temp -> kind == A_subscriptVar; temp = temp -> u.subscript.var)
    {
        indexList -> head = temp -> u.subscript.exp ;
        indexList -> tail = A_ExpList(NULL, NULL);
        indexList = indexList -> tail;
        indexList -> tail = NULL;
    }
    // printf("yo? %d\n", temp -> kind);
    for(result = temp;indexResult -> tail; indexResult = indexResult -> tail)
    {
        result = A_SubscriptVar(var -> pos, result, indexResult -> head);
    }
    return result;
}
static A_classMemberList flipClassMemberList(A_classMemberList list)
{
    A_classMemberList p = A_ClassMemberList(NULL, NULL);
    for(A_classMemberList temp = list; temp -> tail; temp = temp -> tail)
    {
        p = A_ClassMemberList(temp -> head, p);
    }
    return p;
}
static T_moduleList flipModList(T_moduleList list)
{
    T_moduleList p = T_ModuleList(NULL, NULL);
    for(T_moduleList temp = list; temp -> tail; temp = temp -> tail)
    {
        p = T_ModuleList(temp -> head, p);
    }
    return p;
}
static S_symbol actual_name(A_var var)
{
    while(var -> kind != A_simpleVar)
    {
        if(var -> kind == A_subscriptVar)
        {
            var = var -> u.subscript.var;
        }
        else if(var -> kind == A_fieldVar)
        {
            var = var -> u.field.var;
        }
        else if(var -> kind == A_derefVar)
        {
            var = var -> u.deref;
        }
    }
    return var -> u.simple;
}
static S_symbol getName(A_var var)
{
    if(var -> kind == A_subscriptVar)
    {
        return var -> u.subscript.name;
    }
    else if(var -> kind == A_fieldVar)
    {
        return var -> u.field.sym;
    }
    else if(var -> kind == A_derefVar)
    {
        return getName(var -> u.deref);
    }
    else if(var -> kind == A_simpleVar)
    {
        return var -> u.simple;
    }
}
static S_symbol operToSym(A_oper oper){
    string opId = "";
    switch(oper){
        case A_plusOp:
            opId = "+";
            break;
        case A_minusOp:
            opId = "-";
            break;
        case A_timesOp:
            opId = "*";
            break;
        case A_divideOp:
            opId = "/";
            break;
        case A_modOp:
            opId = "%";
            break;
        case A_eqOp:
            opId = "==";
            break;
        case A_neqOp:
            opId = "!=";
            break;
        case A_ltOp:
            opId = "<";
            break;
        case A_leOp:
            opId = "<=";
            break;
        case A_gtOp:
            opId = ">";
            break;
        case A_geOp:
            opId = ">=";
            break;
        case A_andOp:
            opId = "&&";
            break;
        case A_orOp:
            opId = "||";
            break;
    }
    return S_Symbol(opId);
}