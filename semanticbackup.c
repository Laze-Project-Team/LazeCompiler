#include <stdio.h>
#include <string.h>
#include "semantic.h"

extern int funcs;

extern A_decList absyn_root;
static T_moduleList list;
static T_moduleList result;
static bool hasLoop = FALSE;
static T_module loopFunc;
static T_stmList loopVarsStart;
static T_stmList loopVarsEnd;

struct expty expTy(Tr_exp exp, Ty_ty ty)
{
    struct expty e;
    e.exp = exp;
    e.ty = ty;
    return e;
}

static Ty_ty actual_ty(Ty_ty ty);
static Ty_tyList makeParamTypeList(S_table tenv, A_fieldList params);
static Ty_ty checkSymType(S_symbol sym);
static S_symbol returnSymFromType(Ty_ty);
static U_boolList makeEscapeList(A_fieldList list);
static T_type convertType(Ty_ty type);
static T_typeList convertAllType(Ty_tyList types);
static bool compType(Ty_ty lhs, Ty_ty rhs);
static T_exp convertAssign(Ty_ty type, struct expty exp, int pos);
static A_ty reverseArrayTy(A_ty type);
static A_var reverseSubVar(A_var var);
T_moduleList SEM_transProg(A_decList declist)
{
    loopFunc = checked_malloc(sizeof(*loopFunc));
    loopFunc = T_FuncMod(NULL);
    loopVarsStart = checked_malloc(sizeof(*loopVarsStart));
    loopVarsEnd = loopVarsStart;
    list = checked_malloc(sizeof(*list));
    result = list;
    T_module mainFunc;
    S_table venv = E_base_fenv(), tenv = E_base_tenv();
    if (declist == NULL)
    {
        EM_error(0, "No code.");
    }
    A_decList decList = declist;
    // E_enventry logEntry = S_look(venv, S_Symbol("表示"));
    // T_fundec log = T_Fundec(convertAllType(logEntry->u.func.formals), convertAllType(Ty_TyList(NULL, NULL)), T_none, NULL, FALSE, logEntry->u.func.index, NULL);
    // list->head = T_ImportMod("console", "log", T_FuncMod(log));
    // list->tail = checked_malloc(sizeof(*list));
    // list = list->tail;
    list->head = T_ImportMod("js", "mem", T_MemMod(17));
    // list -> tail = checked_malloc(sizeof(*list));
    // list = list -> tail;
    // list -> head = T_FuncMod(initDrawPixelFunc(venv));
    list->tail = checked_malloc(sizeof(*list));
    list = list->tail;
    list->head = NULL;
    list->tail = NULL;
    for (; decList != NULL; decList = decList->tail)
    {
        A_dec dec = decList->head;
        if (dec == NULL)
        {
            continue;
        }
      //printf("%d type dec\n", dec -> kind);
        T_module decMod = transDec(venv, tenv, dec, Tr_outermost(), FALSE, venv);
        if (decMod)
        {
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
            list->tail = checked_malloc(sizeof(*list));
            list = list->tail;
            list->head = NULL;
            list->tail = NULL;
        }
    }
    list->head = T_ExportMod("main", mainFunc);
    list->tail = checked_malloc(sizeof(*list));
    list = list->tail;
    list->head = loopFunc;
    list->tail = checked_malloc(sizeof(*list));
    list = list->tail;
    if (hasLoop)
    {
        list->head = T_ExportMod("loop", loopFunc);
        list->tail = checked_malloc(sizeof(*list));
        list = list->tail;
    }
    list->head = NULL;
    list->tail = NULL;
    return result;
}

struct expty transStm(S_table venv, S_table tenv, A_stm stm, Tr_level level, bool isLoop)
{
    assert(stm);
        switch (stm->kind)
        {
        case A_compoundStm:
        {
            //debug(stm->pos, "Compound Statement");
            A_stmList stmlist;
            T_stmList list = checked_malloc(sizeof(*list));
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
                    struct expty exp = transStm(venv, tenv, stmHead, level, isLoop);
                    // printf("hey %d\n", exp.exp -> kind);
                    if (exp.exp->kind == Tr_t_stm)
                    {
                        list->head = exp.exp->u.stm;
                        list->tail = checked_malloc(sizeof(*list->tail));
                        list = list->tail;
                        list->head = NULL;
                        list->tail = NULL;
                    }
                }
            }
            S_endScope(venv);
            S_endScope(tenv);
            return expTy(Tr_CompoundStm(stm->pos, result), Ty_Void());
            break;
        }
        case A_assignStm:
        {
            //debug(stm->pos, "Assign Statement");
            A_var var = stm->u.assign.var;
            // printf("%d type var\n", var -> kind);
            struct expty varExpty = transVar(venv, tenv, var, level, isLoop, FALSE, stm -> u.assign.isDec);
            if(varExpty.ty == Ty_array)
            {
                if (stm->u.assign.exp)
                {
                    struct expty assignVal = transExp(venv, tenv, stm->u.assign.exp, level, isLoop);
                    if(stm -> u.assign.exp -> kind == A_arrayExp)
                    {
                        T_stmList assignList = checked_malloc(sizeof(assignList));
                        T_stmList result = assignList;
                        T_expList list = assignVal.exp -> u.exp -> u.seq.list;
                        A_exp expp;
                        for(int i = 0; i < assignVal.ty -> u.array.size; i++)
                        {
                            expp = stm -> u.assign.exp -> u.array.list -> head;
                            A_var assignVar;
                            if(stm -> u.assign.isDec)
                                assignVar = A_SimpleVar(stm -> pos, var -> u.subscript.name);
                            else
                                assignVar = var;
                            A_stm assign = A_AssignStm(stm -> pos, A_SubscriptVar(stm -> pos, assignVar, A_IntExp(stm -> pos, i)), expp, stm -> u.assign.isDec);
                            assign -> u.assign.isArrayExp = TRUE;
                            assignList -> head = transStm(venv, tenv, assign, level, isLoop).exp -> u.stm;
                            assignList -> tail = checked_malloc(sizeof(assignList -> tail));
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
                        T_stmList assignList = checked_malloc(sizeof(assignList));
                        T_stmList result = assignList;
                        Tr_exp arrayAddr;
                        if(varExpty.ty -> u.array.type != Ty_char){
                            EM_error(stm -> pos, "Cannot assign string to non string variable.");
                        }
                        if(var -> kind == A_subscriptVar){
                            E_enventry array = S_look(venv, var -> u.subscript.name);
                            int addr = array -> u.var.access -> access -> u.offset + array -> u.var.access -> level -> frame -> offset;
                            arrayAddr = Tr_AddrExp(stm -> pos, addr);
                        }
                        else if(var -> kind == A_simpleVar){
                            arrayAddr = varExpty.exp;
                        }
                        else if(var -> kind == A_fieldVar){
                            arrayAddr = varExpty.exp;
                        }
                        else if(var -> kind == A_derefVar){

                        }
                        T_expList list = assignVal.exp -> u.exp -> u.seq.list;
                      //printf("%d size 276\n", assignVal.ty -> u.array.size);
                        for(int i = 0; i < assignVal.ty -> u.array.size; i++)
                        {
                            int elementAddr = arrayAddr + i * assignVal.ty -> u.array.type -> size;
                            assignList -> head = Tr_AddrAssignStm(stm -> pos, Tr_AddrExp(stm -> pos, elementAddr)->u.exp, list -> head)->u.stm;
                            assignList -> tail = checked_malloc(sizeof(assignList -> tail));
                            assignList = assignList -> tail;
                            assignList -> head = NULL;
                            assignList -> tail = NULL;
                            list = list -> tail;
                        }
                        return expTy(Tr_CompoundStm(stm -> pos, result), Ty_Void());
                    }
                    if(stm -> u.assign.isArrayExp == TRUE)
                    {
                        var = reverseSubVar(var);
                    }
                    struct expty varExp = transVar(venv, tenv, var, level, isLoop, TRUE, FALSE);
                    // printf("hello????????? %d\n", varExp.exp -> u.exp -> kind);
                    return expTy(Tr_AddrAssignStm(stm->pos, varExp.exp->u.exp, transExp(venv, tenv, stm->u.assign.exp, level, isLoop).exp->u.exp), Ty_Void());
                }
                else
                    return varExpty;
            }
        }
        case A_assignStm:
        {
            //debug(stm->pos, "Assign Statement");
            A_var var = stm->u.assign.var;
            // printf("%d type var\n", var -> kind);
            struct expty varExpty = transVar(venv, tenv, var, level, isLoop, FALSE, stm -> u.assign.isDec);
            if(varExpty.ty == Ty_array)
            {

            }
            if (var->kind == A_derefVar)
            {
                E_enventry varType = S_look(venv, var->u.deref->u.simple);
                return expTy(Tr_AddrAssignStm(stm->pos, Tr_VarExp(stm->pos, T_i32, varType->u.var.access, FALSE, FALSE)->u.exp, transExp(venv, tenv, stm->u.assign.exp, level, isLoop).exp->u.exp), varType->u.var.ty);
            }
            else if (var->kind == A_subscriptVar)
            {
                // printf("%s\n", S_name(var -> u.subscript.name));
                E_enventry array = S_look(venv, var->u.subscript.name);
                // printf("test %d\n", array);
                
                if(!stm -> u.assign.isDec || (stm -> u.assign.isDec && stm -> u.assign.exp -> kind != A_arrayExp))
                {
                    int arrayAddr = array->u.var.access->access->u.offset +
                        array->u.var.access->level->frame->offset;
                    struct expty exp = transExp(venv, tenv, var->u.subscript.exp, level, isLoop);
                    
                    if (stm->u.assign.exp)
                    {
                        struct expty assignVal = transExp(venv, tenv, stm->u.assign.exp, level, isLoop);
                        if(stm -> u.assign.exp -> kind == A_arrayExp)
                        {
                            T_stmList assignList = checked_malloc(sizeof(assignList));
                            T_stmList result = assignList;
                            T_expList list = assignVal.exp -> u.exp -> u.seq.list;
                            A_exp expp;
                            for(int i = 0; i < assignVal.ty -> u.array.size; i++)
                            {
                                // printf("arrayexpppppppppp %d kind\n", stm -> u.assign.exp -> u.array.list);
                                expp = stm -> u.assign.exp -> u.array.list -> head;
                                // printf("%d %d size\n", assignVal.ty -> u.array.type -> size, assignVal.ty -> u.array.size);
                                int elementAddr = arrayAddr + i * assignVal.ty -> u.array.type -> size;
                                // printf("%d addresssssss\n", elementAddr);
                                A_stm assign = A_AssignStm(stm -> pos, A_SubscriptVar(stm -> pos, stm -> u.assign.var, A_IntExp(stm -> pos, i)), expp, stm -> u.assign.isDec);
                                assign -> u.assign.isArrayExp = TRUE;
                                assignList -> head = transStm(venv, tenv, assign, level, isLoop).exp -> u.stm;
                                // assignList -> head = Tr_AddrAssignStm(stm -> pos, Tr_AddrExp(stm -> pos, elementAddr)->u.exp, list -> head)->u.stm;
                                assignList -> tail = checked_malloc(sizeof(assignList -> tail));
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
                            T_stmList assignList = checked_malloc(sizeof(assignList));
                            T_stmList result = assignList;
                            int arrayAddr = array -> u.var.access -> access -> u.offset +
                                array -> u.var.access -> level -> frame -> offset;
                            T_expList list = assignVal.exp -> u.exp -> u.seq.list;
                          //printf("%d size 276\n", assignVal.ty -> u.array.size);
                            for(int i = 0; i < assignVal.ty -> u.array.size; i++)
                            {
                                int elementAddr = arrayAddr + i * assignVal.ty -> u.array.type -> size;
                                assignList -> head = Tr_AddrAssignStm(stm -> pos, Tr_AddrExp(stm -> pos, elementAddr)->u.exp, list -> head)->u.stm;
                                assignList -> tail = checked_malloc(sizeof(assignList -> tail));
                                assignList = assignList -> tail;
                                assignList -> head = NULL;
                                assignList -> tail = NULL;
                                list = list -> tail;
                            }
                            return expTy(Tr_CompoundStm(stm -> pos, result), Ty_Void());
                        }
                        if(stm -> u.assign.isArrayExp == TRUE)
                        {
                            var = reverseSubVar(var);
                        }
                        struct expty varExp = transVar(venv, tenv, var, level, isLoop, TRUE, FALSE);
                        // printf("hello????????? %d\n", varExp.exp -> u.exp -> kind);
                        return expTy(Tr_AddrAssignStm(stm->pos, varExp.exp->u.exp, transExp(venv, tenv, stm->u.assign.exp, level, isLoop).exp->u.exp), Ty_Void());
                    }
                    else
                        return expTy(Tr_AddrExp(stm->pos, arrayAddr), array->u.var.ty);
                }
                else
                {
                    int arrayAddr = array->u.var.access->access->u.offset +
                        array->u.var.access->level->frame->offset;
                    if(!stm -> u.assign.exp)
                    {
                        return expTy(Tr_AddrExp(stm->pos, arrayAddr), array->u.var.ty);
                    }
                    // printf("%d fdsalijoijfs\n", stm->u.assign.exp);
                    struct expty assignVal = transExp(venv, tenv, stm->u.assign.exp, level, isLoop);
                    T_stmList assignList = checked_malloc(sizeof(assignList));
                    T_stmList result = assignList;
                    T_expList list = assignVal.exp -> u.exp -> u.seq.list;
                    A_exp expp;
                    for(int i = 0; i < assignVal.ty -> u.array.size; i++)
                    {
                        // printf("arrayexpppppppppp %d %d %d size\n", assignVal.ty -> u.array.type -> size, assignVal.ty -> u.array.size, i);
                        expp = stm -> u.assign.exp -> u.array.list -> head;
                        int elementAddr = arrayAddr + i * assignVal.ty -> u.array.type -> size;
                        A_stm assign = A_AssignStm(stm -> pos, A_SubscriptVar(stm -> pos, A_SimpleVar(stm -> pos, var -> u.subscript.name), A_IntExp(stm -> pos, i)), expp, FALSE);
                        assign -> u.assign.isArrayExp = TRUE;
                        assignList -> head = transStm(venv, tenv, assign, level, isLoop).exp -> u.stm;
                        // assignList -> head = Tr_AddrAssignStm(stm -> pos, Tr_AddrExp(stm -> pos, elementAddr)->u.exp, list -> head)->u.stm;
                        assignList -> tail = checked_malloc(sizeof(assignList -> tail));
                        assignList = assignList -> tail;
                        assignList -> head = NULL;
                        assignList -> tail = NULL;
                        list = list -> tail;
                        stm -> u.assign.exp -> u.array.list = stm -> u.assign.exp -> u.array.list -> tail;
                    }
                    return expTy(Tr_CompoundStm(stm -> pos, result), Ty_Void());
                    break;
                }
            }
            else if(var -> kind == A_fieldVar)
            {

            }
            else
            {
                // printf("%s var -> u.smple\n", S_name(level->frame->name));
                E_enventry varType = S_look(venv, var->u.simple);
                if (stm->u.assign.exp)
                {
                    A_exp exp = stm->u.assign.exp;
                    // printf("%d left hand type\n", exp->kind);
                    int index = 0;
                    E_enventry addrVar;
                    if (exp->kind == A_addressExp)
                        if (addrVar = S_look(venv, exp->u.address->u.simple))
                            if (addrVar->u.var.access->access->kind != inFrame)
                                index = addrVar->u.var.access->access->u.index;
                    struct expty assignVal = transExp(venv, tenv, exp, level, isLoop);
                    if (varType == NULL)
                    {
                        EM_error(stm->pos, "%s is undefined.", S_name(var->u.simple));
                    }
                    if (exp->kind == A_addressExp)
                    {
                        // printf("%d aa\n", addrVar->u.var.access->access->kind);
                        return expTy(Tr_CompoundStm(stm->pos, T_StmList(Tr_AssignStm(stm->pos, addrVar->u.var.access,
                            T_GetLocalExp(convertType(addrVar->u.var.ty), index))
                            ->u.stm,
                            T_StmList(Tr_AssignStm(stm->pos, varType->u.var.access, assignVal.exp->u.exp)->u.stm,
                                T_StmList(NULL, NULL)))),
                            Ty_Void());
                    }
                    else if(exp -> kind == A_stringExp)
                    {
                        T_stmList assignList = checked_malloc(sizeof(assignList));
                        T_stmList result = assignList;
                        int arrayAddr = varType -> u.var.access -> access -> u.offset +
                            varType -> u.var.access -> level -> frame -> offset;
                        T_expList list = assignVal.exp -> u.exp -> u.seq.list;
                      //printf("%d size 276\n", assignVal.ty -> u.array.size);
                        for(int i = 0; i < assignVal.ty -> u.array.size; i++)
                        {
                            int elementAddr = arrayAddr + i * assignVal.ty -> u.array.type -> size;
                            assignList -> head = Tr_AddrAssignStm(stm -> pos, Tr_AddrExp(stm -> pos, elementAddr)->u.exp, list -> head)->u.stm;
                            assignList -> tail = checked_malloc(sizeof(assignList -> tail));
                            assignList = assignList -> tail;
                            assignList -> head = NULL;
                            assignList -> tail = NULL;
                            list = list -> tail;
                        }
                        return expTy(Tr_CompoundStm(stm -> pos, result), Ty_Void());
                    }
                    else if(exp -> kind == A_arrayExp)
                    {
                        T_stmList assignList = checked_malloc(sizeof(assignList));
                        T_stmList result = assignList;
                        int arrayAddr = varType->u.var.access->access->u.offset +
                        varType->u.var.access->level->frame->offset;
                        T_expList list;
                        A_expList explist = exp -> u.array.list;
                        // printf("%d assignVal.ty -> kind\n", assignVal.ty -> kind);
                        if(assignVal.exp -> u.exp)
                        {
                            list = assignVal.exp -> u.exp -> u.seq.list;
                            A_exp expp;
                          //printf("%d size 299\n", assignVal.ty -> u.array.size);
                            for(int i = 0; i < assignVal.ty -> u.array.size; i++)
                            {
                                expp = explist -> head;
                                // printf("arraayyy %d %d %d size\n", assignVal.ty -> u.array.type -> size, assignVal.ty -> u.array.size, i);
                                int elementAddr = arrayAddr + i * assignVal.ty -> u.array.type -> size;
                              //printf("%d 302\n", stm -> u.assign.var ->kind);
                                assignList -> head = transStm(venv, tenv, A_AssignStm(stm -> pos, A_SubscriptVar(stm -> pos, stm -> u.assign.var, A_IntExp(stm -> pos, i)), expp, stm -> u.assign.isDec), level, isLoop).exp -> u.stm;
                                assignList -> tail = checked_malloc(sizeof(assignList));
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
                        transVar(venv, tenv, var, level, isLoop, FALSE, stm -> u.assign.isDec);
                      //printf("%s %d varName 352\n", S_name(var -> u.simple), varType -> u.var.ty -> kind);
                        if (!compType(varType->u.var.ty, assignVal.ty))
                        {
                          //printf("%d %d flaj\n", varType -> u.var.ty -> kind, assignVal.ty -> kind);
                            return expTy(Tr_AssignStm(stm -> pos, varType -> u.var.access, convertAssign(varType -> u.var.ty, assignVal, exp -> pos)), Ty_Void());
                        }
                        return expTy(Tr_AssignStm(stm->pos, varType->u.var.access, assignVal.exp->u.exp), Ty_Void());
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
                T_module dec = transDec(venv, tenv, stm->u.declaration.dec, level, isLoop, venv);
                if(stm -> u.declaration.dec -> u.var.init)
                    exp = transStm(venv, tenv, A_AssignStm(stm->pos, stm->u.declaration.dec->u.var.var, stm->u.declaration.dec->u.var.init, TRUE), level, FALSE);
                else
                    exp = expTy(Tr_NoStm(stm -> pos), Ty_Void());
            }
            else if (stm->u.declaration.dec->kind == A_functionDec)
            {
                T_module dec = transDec(venv, tenv, stm->u.declaration.dec, level, TRUE, venv);
                hasLoop = TRUE;
                loopFunc = dec;
                exp = expTy(Tr_CompoundStm(stm -> pos, loopVarsStart), Ty_Void());
            }
            // S_endScope(venv);
            // S_beginScope(tenv);
            return exp;
            break;
        }
        case A_ifStm:
        {
            //debug(stm->pos, "If Statement");
            if (transExp(venv, tenv, stm->u.iff.test, level, isLoop).ty != Ty_Bool())
            {
                EM_error(stm->pos, "If statement test expression must have a type of boolean.");
            }
            struct expty then = transStm(venv, tenv, stm->u.iff.then, level, isLoop);
            struct expty elsee = expTy(NULL, Ty_Void());
            if (stm->u.iff.elsee != NULL)
            {
                elsee = transStm(venv, tenv, stm->u.iff.elsee, level, isLoop);
                return expTy(Tr_IfStm(stm->pos, transExp(venv, tenv, stm->u.iff.test, level, isLoop).exp->u.exp,
                    then.exp->u.stm, elsee.exp->u.stm),
                    Ty_Void());
            }
            return expTy(Tr_IfStm(stm->pos, transExp(venv, tenv, stm->u.iff.test, level, isLoop).exp->u.exp,
                then.exp->u.stm, NULL),
                Ty_Void());
        }
        case A_whileStm:
        {
            //debug(stm->pos, "While Statement");
            struct expty body;
            if (transExp(venv, tenv, stm->u.whilee.test, level, isLoop).ty != Ty_Bool())
            {
                EM_error(stm->pos, "While statement test expression must have a type of boolean.");
            }
            body = transStm(venv, tenv, stm->u.whilee.body, level, isLoop);
            return expTy(Tr_WhileStm(stm->pos, transExp(venv, tenv, stm->u.whilee.test, level, isLoop).exp->u.exp,
                body.exp->u.stm),
                Ty_Void());
        }
        case A_forStm:
        {
            //debug(stm->pos, "For Statement");
            A_stm assignStm = stm->u.forr.assign;
            struct expty assign = transStm(venv, tenv, assignStm, level, isLoop);
            struct expty condition = transExp(venv, tenv, stm->u.forr.condition, level, isLoop);
            if (condition.ty != Ty_Bool())
            {
                EM_error(stm->pos, "For statement condition expression must have a type of boolean.");
            }
            struct expty increment = transStm(venv, tenv, stm->u.forr.increment, level, isLoop);
            struct expty body = transStm(venv, tenv, stm->u.forr.body, level, isLoop);
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
            return expTy(Tr_LoopStm(stm->pos), Ty_Void());
        }
        case A_callStm:
        {
            T_expList expList = checked_malloc(sizeof(*expList));
            T_expList result = expList;
            //debug(stm->pos, "Call Statement");
            S_symbol name;
            if(stm -> u.call.func -> kind == A_simpleVar)
                name = stm -> u.call.func -> u.simple;
            E_enventry entry = S_look(venv, name);
            if (entry != NULL && entry->kind == E_funcentry)
            {
                A_expList expArgs;
                Ty_tyList entryArgsTy;

                int i = 1;
                for (i = 1, expArgs = stm->u.call.args, entryArgsTy = entry->u.func.formals; expArgs != NULL && entryArgsTy != NULL;
                    expArgs = expArgs->tail, entryArgsTy = entryArgsTy->tail, i++)
                {
                    A_exp exp = expArgs->head;
                    struct expty expp = transExp(venv, tenv, exp, level, isLoop);
                    expList->head = expp.exp->u.exp;
                  //printf("%d expList -> head\n", expp.ty -> kind );
                    expList->tail = checked_malloc(sizeof(*expList));
                    Ty_ty type = entryArgsTy->head;

                    if (exp != NULL && type != NULL)
                    {
                        struct expty argType = transExp(venv, tenv, exp, level, isLoop);
                        if (!compType(type, argType.ty))
                        {
                            expList -> head = convertAssign(type, argType, exp -> pos);
                        }
                        
                    }
                    else
                    {
                        if (exp == NULL && type != NULL)
                        {
                            EM_error(stm->pos, "There are too few arguments in the call statement.");
                            break;
                        }
                        else if (exp != NULL && type == NULL)
                        {
                            EM_error(stm->pos, "There are too many arguments in the call statement.");
                            break;
                        }
                    }
                    expList = expList->tail;

                    expList->head = NULL;
                    expList->tail = NULL;
                }

                if (expArgs == NULL && entryArgsTy != NULL)
                {
                    if (entryArgsTy->head != NULL)
                    {
                        EM_error(stm->pos, "There are too few arguments in the call statement.");
                    }
                }
                else if (expArgs != NULL && entryArgsTy == NULL)
                {
                    if (expArgs->head != NULL)
                    {
                        EM_error(stm->pos, "There are too many arguments in the call statement.");
                    }
                }
                if (entry->u.func.result)
                    return expTy(Tr_CallStm(stm->pos, entry->u.func.index, S_name(entry->u.func.label), result), entry->u.func.result->access->type);
                return expTy(Tr_CallStm(stm->pos, entry->u.func.index, S_name(entry->u.func.label), result), Ty_Void());
            }
            else
            {
                EM_error(stm->pos, "Undefined function \'%s\'", S_name(name));
            }
            return expTy(Tr_CallStm(stm->pos, entry->u.func.index, S_name(entry->u.func.label), expList), Ty_Void());
        }
        case A_returnStm:
        {
            //debug(stm->pos, "Return Statement");
            if (stm->u.returnn.ret)
                return expTy(Tr_ReturnStm(stm->pos, transExp(venv, tenv, stm->u.returnn.ret, level, isLoop).exp->u.exp), Ty_Void());
            else
            {
                Temp_label label = level->label;
                E_enventry func = S_look(venv, label);
                if (func->kind == E_funcentry)
                    if (func->u.func.result)
                        return expTy(Tr_ReturnStm(stm->pos, Tr_VarExp(stm->pos, convertType(func->u.func.result->access->type), func->u.func.result, FALSE, FALSE)->u.exp), Ty_Void());
                    else
                        return expTy(Tr_ReturnStm(stm->pos, NULL), Ty_Void());
            }
        }
        }
}

struct expty transVar(S_table venv, S_table tenv, A_var v, Tr_level level, bool isLoop, bool reverse, bool isDec)
{
    assert(v);
    static int depth = 0;
    switch (v->kind)
    {
    case A_simpleVar:
    {
        E_enventry x = S_look(venv, v->u.simple);
        if (x && x->kind == E_varentry)
        {
            if (x->u.var.ty->kind == Ty_array)
            {
                int arrayAddr = x->u.var.access->level->frame->offset + x->u.var.access->access->u.offset;
                
                return expTy(Tr_VarExp(v->pos, convertType(x -> u.var.ty), x -> u.var.access, FALSE, TRUE), x->u.var.ty);
            }
            else
            {
                if (!isLoop)
                {
                    if (x->u.var.access->level->parent)
                        return expTy(Tr_VarExp(v->pos, convertType(x->u.var.ty), x->u.var.access, FALSE, FALSE), x->u.var.ty);
                    else
                        return expTy(Tr_VarExp(v->pos, convertType(x->u.var.ty), x->u.var.access, TRUE, FALSE), x->u.var.ty);
                }
                else
                {
                  //printf("[[[[[[[[[[[[[[[[[\n");
                    //debug(v -> pos, "%s", S_name(x -> u.var.access -> level -> label));
                    if(strcmp(S_name(x -> u.var.access -> level -> label), "loop") != 0)
                    {
                        if(x -> u.var.access -> access -> kind != inFrame)
                        {
                            int arrayAddr = x->u.var.access->level->frame->offset + x->u.var.access->level -> frame -> frameSize;
                          //printf("not inframe %d\n", arrayAddr);
                            loopVarsEnd -> head = Tr_AddrAssignStm(v -> pos, Tr_AddrExp(v -> pos, arrayAddr)->u.exp, Tr_VarExp(v->pos, convertType(x->u.var.ty), x->u.var.access, FALSE, FALSE)->u.exp)->u.stm;
                            loopVarsEnd -> tail = checked_malloc(sizeof(*loopVarsEnd));
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
            EM_error(v->pos, "It doesn't exist");
        }
        else if (x->kind != E_varentry)
        {
            EM_error(v->pos, "It is not a variable. %s", S_name(x->u.var.ty->u.name.sym));
        }
        else
        {
            EM_error(v->pos, "undefined variable %s", S_name(v->u.simple));
            return expTy(NULL, Ty_Int());
        }
        break;
    }
    case A_fieldVar:
    {
        struct expty varExpty = transVar(venv, tenv, v -> u.field.var, level, isLoop, reverse, FALSE);
        if(varExpty.ty -> kind != Ty_name)
            EM_error(v -> pos, "Cannot access a member of a non-class object.");
        E_enventry classEntry = S_look(tenv, varExpty.ty -> u.name.sym);
        Ty_member varEntry = S_look(classEntry -> u.class.varTypes, v -> u.field.sym);
        return expTy(Tr_DerefExp(v -> pos, Tr_OpExp(v -> pos, T_i32, T_add, varExpty.exp -> u.exp, Tr_AddrExp(v -> pos, varEntry -> offset)->u.exp) -> u.exp, T_i32), varEntry -> ty);
    }
    case A_subscriptVar:
    {
        E_enventry entry = S_look(venv, v->u.subscript.name);
        if(isDec)
        {
            int addr = entry -> u.var.access -> level -> frame -> offset + entry -> u.var.access -> access -> u.offset;
            return expTy(Tr_AddrExp(v -> pos, addr), v);
        }
        struct expty exp = transExp(venv, tenv, v->u.subscript.exp, level, isLoop);
        // printf("%d index 600\n", v->u.subscript.exp -> u.intt);
        if (entry)
        {
            // printf("632 %d\n", entry -> u.var.ty -> kind);
            if (entry->u.var.ty->kind == Ty_array)
            {
                // printf("test test tes %d %d\n", v -> u.subscript.depth, v -> u.subscript.exp -> u.intt);
                Ty_ty type = entry->u.var.ty;
                for (int i = 0; i < depth + 1; i++)
                {
                    type = type->u.array.type;
                }
                // printf("type %d\n", type -> kind);
                depth++;
                struct expty var = transVar(venv, tenv, v->u.subscript.var, level, isLoop, FALSE, FALSE);
                depth = 0;
                return expTy(Tr_OpExp(v->pos, T_i32, T_add, var.exp->u.exp, T_BinOpExp(T_i32, T_mul, T_ConstExp(T_i32, A_IntExp(v->pos, type->size)), T_ConvertExp(T_i32, exp.exp->u.exp))), Ty_Array(var.ty, type->u.array.size));
            }
            else if(entry -> u.var.ty -> kind== Ty_pointer)
            {
                Ty_ty type = entry -> u.var.ty;
                for(int i = 0; i < depth + 1; i++)
                {
                    type = type -> u.pointer;
                }
                depth++;
                struct expty var = transVar(venv, tenv, v -> u.subscript.var, level, isLoop, FALSE, FALSE);
                depth = 0;
                return expTy(Tr_OpExp(v->pos, T_i32, T_add, var.exp->u.exp, T_BinOpExp(T_i32, T_mul, T_ConstExp(T_i32, A_IntExp(v->pos, type->size)), T_ConvertExp(T_i32, exp.exp->u.exp))), Ty_Pointer(var.ty));

            }
            else
            {
                // printf("632 %d\n", entry -> u.var.ty -> kind);
                EM_error(v->pos, "It's not an array!");
            }
        }
        else
        {
            EM_error(v->pos, "%s does not exist.", S_name(v->u.subscript.name));
        }
    }
    case A_derefVar:
    {
        E_enventry entry = S_look(venv, v->u.deref->u.simple);
        int address = entry->u.var.access->level->frame->offset +
            entry->u.var.access->access->u.offset;
        return expTy(Tr_VarExp(EM_tokPos, T_i32, entry->u.var.access,
            (entry->u.var.access->access->kind == inGlobal), FALSE),
            entry->u.var.access->access->type -> u.pointer);
    }
    }
}

struct expty transExp(S_table venv, S_table tenv, A_exp e, Tr_level level, bool isLoop)
{
    // assert(e);
    switch (e->kind)
    {
    case A_intExp:
    {
        return expTy(Tr_IntExp(e->pos, e->u.intt), Ty_Int());
        break;
    }
    case A_stringExp:
    {
        // printf("%s %d\n", e -> u.stringg, strlen(e -> u.stringg));
        T_expList list = checked_malloc(sizeof(*list));
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
            list -> tail = checked_malloc(sizeof(list -> tail));
            list = list -> tail;
            list -> head = NULL;
            list -> tail = NULL;
            size++;
        }
        // printf("%d size string\n", size);
        return expTy(Tr_ArrayExp(e -> pos, result), Ty_Array(Ty_Char(), size));
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
            return transVar(venv, tenv, e->u.var, level, isLoop, FALSE, FALSE);
        else if (e->u.var->kind == A_derefVar || e->u.var->kind == A_subscriptVar)
        {
            struct expty var = transVar(venv, tenv, e->u.var, level, isLoop, FALSE, FALSE);
            return expTy(Tr_DerefExp(e->pos, var.exp->u.exp, convertType(actual_ty(var.ty))), actual_ty(var.ty));
        }
        break;
    }
    case A_derefExp:
    {
        E_enventry entry = S_look(venv, e->u.deref->u.simple);
        return expTy(Tr_DerefExp(e->pos, Tr_VarExp(e->pos, T_i32, entry->u.var.access, !(entry->u.var.access->level->parent), FALSE)->u.exp, convertType(actual_ty(entry->u.var.ty))), transVar(venv, tenv, e->u.deref, level, isLoop, FALSE, FALSE).ty);
        break;
    }
    case A_addressExp:
    {
        E_enventry varEntry = S_look(venv, e->u.address->u.simple);
        int address = 0;
        if (!varEntry)
        {
            EM_error(e->pos, "Variable does not exist.");
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
    case A_callExp:
    {
        S_symbol name;
        if(e -> u.call.func -> kind == A_simpleVar)
            name = e -> u.call.func -> u.simple;
        E_enventry entry = S_look(venv, name);
        T_expList expList = checked_malloc(sizeof(*expList));
        T_expList result = expList;
        if (entry != NULL && entry->kind == E_funcentry)
        {
            A_expList expArgs;
            Ty_tyList entryArgsTy;
            int i = 1;
            for (i = 1, expArgs = e->u.call.args, entryArgsTy = entry->u.func.formals; expArgs != NULL && entryArgsTy != NULL;
                expArgs = expArgs->tail, entryArgsTy = entryArgsTy->tail, i++)
            {

                A_exp exp = expArgs->head;
                expList->head = transExp(venv, tenv, exp, level, isLoop).exp->u.exp;
                expList->tail = checked_malloc(sizeof(*expList));
                Ty_ty type = entryArgsTy->head;
                if (exp != NULL && type != NULL)
                {
                    struct expty argType = transExp(venv, tenv, exp, level, isLoop);
                    if (!compType(type, argType.ty))
                    {
                        // printf("%d %d flaj\n", type -> kind, argType.ty -> kind);
                        expList -> head = convertAssign(type, argType, e -> pos);
                    }
                }
                else
                {
                    if (exp == NULL && type != NULL)
                    {
                        EM_error(e->pos, "There are too few arguments in the call expression.");
                        break;
                    }
                    else if (exp != NULL && type == NULL)
                    {
                        EM_error(e->pos, "There are too many arguments in the call expression.");
                        break;
                    }
                }
                expList = expList->tail;
                expList->tail = NULL;
                expList->head = NULL;
            }
            if (expArgs == NULL && entryArgsTy != NULL)
            {
                if (entryArgsTy->head != NULL)
                {
                    EM_error(e->pos, "There are too many arguments in the call expression.");
                }
            }
            else if (expArgs != NULL && entryArgsTy == NULL)
            {
                if (expArgs->head != NULL)
                {
                    EM_error(e->pos, "There are too few arguments in the call expression.");
                }
            }
            // printf("%s %d fdoaijfoasdjfoijsijo\n", S_name(e ->u.call.func), entry -> kind);
            return expTy(Tr_CallExp(e->pos, convertType(entry->u.func.result->access->type),
                entry->u.func.index, S_name(entry->u.func.label), result),
                entry->u.func.result->access->type);
        }
        else
        {
            EM_error(e->pos, "Undefined function \'%s\'", S_name(name));
        }
    }
    case A_recordExp:
    {
        return expTy(NULL, S_look(tenv, e->u.record.typ));
    }
    case A_assignExp:
    {
        A_var var = e->u.assign.var;
        A_exp exp = e->u.assign.exp;
        E_enventry varType = S_look(venv, var->u.simple);
        struct expty expp = transExp(venv, tenv, exp, level, isLoop);
        if (varType->u.var.ty != expp.ty)
        {
            EM_error(e->pos, "Cannot assign %s to %s.", S_name(expp.ty->u.name.sym), returnSymFromType(varType->u.var.ty));
        }
        return expTy(expp.exp, transExp(venv, tenv, exp, level, isLoop).ty);
    }
    case A_ifExp:
    {
        struct expty test = transExp(venv, tenv, e->u.iff.test, level, isLoop);
        struct expty then = transExp(venv, tenv, e->u.iff.then, level, isLoop);
        struct expty elsee = transExp(venv, tenv, e->u.iff.elsee, level, isLoop);
        if (then.ty != Ty_Bool())
        {
            EM_error(e->pos, "Test expression needs to be a bool type.");
        }
        if (then.ty != elsee.ty)
        {
            EM_error(e->pos, "Then expression and else expression must be of the same type.");
        }
        return expTy(Tr_IfExp(e->pos, convertType(then.ty), test.exp->u.exp, then.exp->u.exp, elsee.exp->u.exp), actual_ty(transExp(venv, tenv, e->u.iff.then, level, isLoop).ty));
    }
    case A_sizeofExp:
    {
        E_enventry entry;
        if(e -> u.sizeOf -> kind == A_simpleVar)
        {
            entry = S_look(venv, e -> u.sizeOf -> u.simple);
            return expTy(Tr_AddrExp(e -> pos, entry -> u.var.ty -> size), Ty_Short());
        }
        else if(e -> u.sizeOf -> kind == A_subscriptVar)
        {
            entry = S_look(venv, e -> u.sizeOf -> u.subscript.name);
            return expTy(Tr_AddrExp(e -> pos, actual_ty(entry -> u.var.ty) -> size), Ty_Short());
        }
        else
            EM_error(e -> pos, "The sizeof operator is only applicable on simple variables.");
        
    }
    case A_arrayExp:
    {
        T_expList list = checked_malloc(sizeof(*list));
        T_expList result = list;
        A_expList explist = e -> u.array.list;
        if(explist == NULL)
            return expTy(Tr_NoExp(e -> pos), Ty_Void());
        int i = 0;
        A_exp exp;
        struct expty expp;
        Ty_ty arrayType = NULL;
        for(; explist; explist = explist -> tail, i++)
        {
            exp = explist -> head;
            expp = transExp(venv, tenv, exp, level, isLoop);
            // printf("%d kind\n", expp.ty -> kind);
            list -> head = expp.exp -> u.exp;
            if(arrayType && compType(arrayType, expp.ty))
                arrayType = expp.ty;
            else if(!arrayType)
                arrayType = expp.ty;
            else if(arrayType && arrayType != expp.ty)
            {
                // printf("%d %d types\n", arrayType -> kind, expp.ty -> kind);
                EM_error(e -> pos, "Element %d does not match the other types.", i);
            }
            list -> tail = checked_malloc(sizeof(*list -> tail));
            list = list -> tail;
            list -> head = NULL;
            list -> tail = NULL;
        }
        // printf("testettsedtestest\n");
        return expTy(Tr_ArrayExp(e -> pos, result), Ty_Array(arrayType, i));
    }
    case A_opExp:
    {
        A_oper oper = e->u.op.oper;
        struct expty left = transExp(venv, tenv, e->u.op.left, level, isLoop);
        struct expty right = transExp(venv, tenv, e->u.op.right, level, isLoop);
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
                    EM_error(e->u.op.left->pos, "The two operands cannot be added.");
            }
            else
            {
                if (left.ty == NULL && right.ty != NULL)
                    EM_error(e->pos, "Left operand does not have a type.");
                else if (left.ty != NULL && right.ty == NULL)
                    EM_error(e->pos, "Right operand does not have a type.");
                if (left.ty == NULL && right.ty == NULL)
                    EM_error(e->pos, "Both operand do not have a type.");
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
                    EM_error(e->u.op.left->pos, "The two operands cannot be subtracted.");
            }
            else
            {
                if (left.ty == NULL && right.ty != NULL)
                    EM_error(e->pos, "Left operand does not have a type.");
                else if (left.ty != NULL && right.ty == NULL)
                    EM_error(e->pos, "Right operand does not have a type.");
                if (left.ty == NULL && right.ty == NULL)
                    EM_error(e->pos, "Both operand do not have a type.");
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
                    EM_error(e->u.op.left->pos, "The two operands cannot be multiplied.");
            }
            else
            {
                if (left.ty == NULL && right.ty != NULL)
                    EM_error(e->pos, "Left operand does not have a type.");
                else if (left.ty != NULL && right.ty == NULL)
                    EM_error(e->pos, "Right operand does not have a type.");
                if (left.ty == NULL && right.ty == NULL)
                    EM_error(e->pos, "Both operand do not have a type.");
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
                else
                {
                    EM_error(e->u.op.left->pos, "The two operands cannot be divided.");
                    return expTy(NULL, NULL);
                }
            }
            else
            {
                if (left.ty == NULL && right.ty != NULL)
                    EM_error(e->pos, "Left operand does not have a type.");
                else if (left.ty != NULL && right.ty == NULL)
                    EM_error(e->pos, "Right operand does not have a type.");
                if (left.ty == NULL && right.ty == NULL)
                    EM_error(e->pos, "Both operand do not have a type.");
            }
        }
        if (oper == A_eqOp)
        {
            if (left.ty != NULL && right.ty != NULL)
            {
                if (left.ty->kind != right.ty->kind)
                    EM_error(e->u.op.left->pos, "The two operands cannot be compared.");
                return expTy(Tr_OpExp(e->pos, convertType(left.ty), T_eq, left.exp->u.exp, right.exp->u.exp), Ty_Bool());
            }
            else
            {
                if (left.ty == NULL && right.ty != NULL)
                    EM_error(e->pos, "Left operand does not have a type.");
                else if (left.ty != NULL && right.ty == NULL)
                    EM_error(e->pos, "Right operand does not have a type.");
                if (left.ty == NULL && right.ty == NULL)
                    EM_error(e->pos, "Both operand do not have a type.");
            }
        }
        if (oper == A_neqOp)
        {
            if (left.ty != NULL && right.ty != NULL)
            {
                if (left.ty->kind != right.ty->kind)
                    EM_error(e->u.op.left->pos, "The two operands cannot be compared.");
                return expTy(Tr_OpExp(e->pos, convertType(left.ty), T_ne, left.exp->u.exp, right.exp->u.exp), Ty_Bool());
            }
            else
            {
                if (left.ty == NULL && right.ty != NULL)
                    EM_error(e->pos, "Left operand does not have a type.");
                else if (left.ty != NULL && right.ty == NULL)
                    EM_error(e->pos, "Right operand does not have a type.");
                if (left.ty == NULL && right.ty == NULL)
                    EM_error(e->pos, "Both operand do not have a type.");
            }
        }
        if (oper == A_leOp)
        {
            if (left.ty != NULL && right.ty != NULL)
            {
                if (left.ty->kind != right.ty->kind)
                    EM_error(e->u.op.left->pos, "The two operands cannot be compared.");
                return expTy(Tr_OpExp(e->pos, convertType(left.ty), T_le_s, left.exp->u.exp, right.exp->u.exp), Ty_Bool());
            }
            else
            {
                if (left.ty == NULL && right.ty != NULL)
                    EM_error(e->pos, "Left operand does not have a type.");
                else if (left.ty != NULL && right.ty == NULL)
                    EM_error(e->pos, "Right operand does not have a type.");
                if (left.ty == NULL && right.ty == NULL)
                    EM_error(e->pos, "Both operand do not have a type.");
            }
        }
        if (oper == A_ltOp)
        {
            if (left.ty != NULL && right.ty != NULL)
            {
                if (left.ty->kind != right.ty->kind)
                    EM_error(e->u.op.left->pos, "The two operands cannot be compared.");
                return expTy(Tr_OpExp(e->pos, convertType(left.ty), T_lt_s, left.exp->u.exp, right.exp->u.exp), Ty_Bool());
            }
            else
            {
                if (left.ty == NULL && right.ty != NULL)
                    EM_error(e->pos, "Left operand does not have a type.");
                else if (left.ty != NULL && right.ty == NULL)
                    EM_error(e->pos, "Right operand does not have a type.");
                if (left.ty == NULL && right.ty == NULL)
                    EM_error(e->pos, "Both operand do not have a type.");
            }
        }
        if (oper == A_geOp)
        {
            if (left.ty != NULL && right.ty != NULL)
            {
                if (left.ty->kind != right.ty->kind)
                    EM_error(e->u.op.left->pos, "The two operands cannot be compared.");
                return expTy(Tr_OpExp(e->pos, convertType(left.ty), T_ge_s, left.exp->u.exp, right.exp->u.exp), Ty_Bool());
            }
            else
            {
                if (left.ty == NULL && right.ty != NULL)
                    EM_error(e->pos, "Left operand does not have a type.");
                else if (left.ty != NULL && right.ty == NULL)
                    EM_error(e->pos, "Right operand does not have a type.");
                if (left.ty == NULL && right.ty == NULL)
                    EM_error(e->pos, "Both operand do not have a type.");
            }
        }
        if (oper == A_gtOp)
        {
            if (left.ty != NULL && right.ty != NULL)
            {
                if (left.ty->kind != right.ty->kind)
                    EM_error(e->u.op.left->pos, "The two operands cannot be compared.");
                return expTy(Tr_OpExp(e->pos, convertType(left.ty), T_gt_s, left.exp->u.exp, right.exp->u.exp), Ty_Bool());
            }
            else
            {
                if (left.ty == NULL && right.ty != NULL)
                    EM_error(e->pos, "Left operand does not have a type.");
                else if (left.ty != NULL && right.ty == NULL)
                    EM_error(e->pos, "Right operand does not have a type.");
                if (left.ty == NULL && right.ty == NULL)
                    EM_error(e->pos, "Both operand do not have a type.");
            }
        }
        if (oper == A_andOp)
        {
            if (left.ty->kind != Ty_bool)
            {
                EM_error(e->pos, "Left operand does not a bool type.");
            }
            if (right.ty->kind != Ty_bool)
            {
                EM_error(e->pos, "Right operand does not a bool type.");
            }
            return expTy(Tr_OpExp(e->pos, convertType(left.ty), T_and, left.exp->u.exp, right.exp->u.exp), Ty_Bool());
        }
        if (oper == A_orOp)
        {
            if (left.ty->kind != Ty_bool)
            {
                EM_error(e->pos, "Left operand does not a bool type.");
            }
            if (right.ty->kind != Ty_bool)
            {
                EM_error(e->pos, "Right operand does not a bool type.");
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
                    EM_error(e->u.op.left->pos, "The two operands cannot be divided.");
                    return expTy(NULL, NULL);
                }
            }
            else
            {
                if (left.ty == NULL && right.ty != NULL)
                    EM_error(e->pos, "Left operand does not have a type.");
                else if (left.ty != NULL && right.ty == NULL)
                    EM_error(e->pos, "Right operand does not have a type.");
                if (left.ty == NULL && right.ty == NULL)
                    EM_error(e->pos, "Both operand do not have a type.");
            }
        }
        break;
    }
    }
}

T_module transDec(S_table venv, S_table tenv, A_dec d, Tr_level level, bool isLoop, S_table table)
{
    assert(d);
    switch (d->kind)
    {
    case A_varDec:
    {
        struct expty exp;
        E_enventry varEntry = S_look(venv, d->u.var.var->u.simple);
        E_enventry typeEntry = S_look(tenv, d->u.var.var->u.simple);
        S_symbol sym;
        if (varEntry != NULL)
        {
            //debug(d->pos, "Variable or function of name %s is already declared.", S_name(d->u.var.var->u.simple));
        }
        if (typeEntry != NULL)
        {
            EM_error(d->pos, "Type of name %s is already declared.", S_name(d->u.var.var->u.simple));
        }
        if (d->u.var.init != NULL)
        {
            Ty_ty type = transTy(tenv, d->u.var.typ);
            if (d->u.var.var->kind == A_simpleVar)
            {
                //debug(d->pos, "Variable declared with value.");
                S_enter(venv, d->u.var.var->u.simple, E_VarEntry(Tr_allocLocal(level, ESC_checkEscapeFromType(type), type), type));
            }
            else if (d->u.var.var->kind == A_subscriptVar)
            {
                S_enter(venv, d->u.var.var->u.subscript.name, E_VarEntry(Tr_allocLocal(level, TRUE, transTy(tenv, d->u.var.typ)), transTy(tenv, d->u.var.typ)));
            }
        }
        else if (d->u.var.init == NULL)
        {
            //debug(d->pos, "Variable declared.");
            if (d->u.var.var->kind == A_simpleVar)
                S_enter(venv, d->u.var.var->u.simple, E_VarEntry(Tr_allocLocal(level, ESC_checkEscapeFromType(transTy(tenv, d->u.var.typ)), transTy(tenv, d->u.var.typ)), transTy(tenv, d->u.var.typ)));
            else if (d->u.var.var->kind == A_subscriptVar)
            {
                assert(d->u.var.var->u.subscript.name);
                // printf("arraaaaaaaaaay %s\n", S_name(d->u.var.var->u.subscript.name));
                Ty_ty type = transTy(tenv, d -> u.var.typ);
                // printf("%d kind 1256\n", type -> u.array.size);
                S_enter(venv, d->u.var.var->u.subscript.name, E_VarEntry(Tr_allocLocal(level, TRUE, type), type));
            }
        }
        if (!(level->parent))
        {
            return T_GlobalMod(convertType(transTy(tenv, d->u.var.typ)), transExp(venv, tenv, d->u.var.init, level, isLoop).exp->u.exp);
        }
        return NULL;
        break;
    }
    case A_functionDec:
    {
        A_fundec func = d->u.function->head;
        Ty_tyList paramTypes = makeParamTypeList(tenv, func->params);
        Tr_level newLevel = Tr_newLevel(level, func->name, makeEscapeList(func->params), paramTypes);
        A_fieldList returnList = func->result;
        if (returnList->tail)
            EM_error(d->pos, "Laze is not supporting multiple returning functions right now.\n");
        T_typeList newTypeList = checked_malloc(sizeof(*newTypeList));
        T_typeList result = newTypeList;
        E_enventry entry = NULL;
        A_field resultVar = NULL;
        // printf("%s func->name\n", S_name(func->name));
        if (returnList->head)
        {
            // printf("there is head\n");
            resultVar = returnList->head;
            entry = E_FuncEntry(level, func->name, paramTypes, Tr_allocLocal(newLevel, resultVar->escape, transTy(tenv, resultVar->typ)));
        }
        else
            entry = E_FuncEntry(level, func->name, paramTypes, Tr_allocLocal(newLevel, FALSE, Ty_Void()));

        S_enter(table, func->name, entry);
        S_beginScope(venv);
        {
            if (resultVar)
                S_enter(venv, resultVar->name, E_VarEntry(entry->u.func.result, entry->u.func.result->access->type));
            A_fieldList list;
            Ty_tyList typeList;
            Tr_accessList accessList;
            for (accessList = newLevel->formals->tail, list = func->params, typeList = paramTypes; list != NULL && typeList != NULL &&
                accessList != NULL;
                list = list->tail, typeList = typeList->tail, accessList = accessList->tail)
            {
                newTypeList->head = convertType(typeList->head);
                newTypeList->tail = checked_malloc(sizeof(*newTypeList));
                newTypeList = newTypeList->tail;
                newTypeList->head = T_none;
                newTypeList->tail = NULL;
                list->head->escape = ESC_checkEscapeFromType(typeList->head);
                if (typeList->head != NULL)
                    S_enter(venv, list->head->name, E_VarEntry(accessList->head, typeList->head));
            }
            struct expty body;
            if (func->body != NULL)
            {
                body = transStm(venv, tenv, func->body, newLevel, isLoop);
            }
            S_endScope(venv);
            //debug(d->pos, "Function declared. %s", S_name(func->name));

            T_typeList typeListt = convertAllType(newLevel->frame->localsType);
            string name = "";
            if (strcmp(S_name(d->u.function->head->name), "実行") == 0)
            {
                name = "main";
            }
            else if (strcmp(S_name(d->u.function->head->name), "loop") == 0)
            {
                name = "loop";
            }
            if (entry->u.func.result)
                return T_FuncMod(T_Fundec(result, typeListt, convertType(transTy(tenv, resultVar->typ)),
                    body.exp->u.stm, name, entry->u.func.index,
                    Tr_VarExp(d->pos, convertType(entry->u.func.result->access->type), entry->u.func.result, FALSE, FALSE)->u.exp));
            else
                return T_FuncMod(T_Fundec(result, typeListt, T_none,
                    body.exp->u.stm, name, entry->u.func.index,
                    NULL));
            break;
        }
    }
    case A_typeDec:
    {
        S_enter(tenv, d->u.type->head->name, transTy(tenv, d->u.type->head->ty));
        break;
    }
    case A_funcImportDec:
    {
        Ty_tyList paramTypes = makeParamTypeList(tenv, d -> u.funcImport.params);
        Tr_level newLevel = Tr_newLevel(level, d -> u.funcImport.name, makeEscapeList(d -> u.funcImport.params), paramTypes);
        A_fieldList returnList = d -> u.funcImport.result;
        if (returnList->tail)
            EM_error(d->pos, "Laze is not supporting multiple returning functions right now.\n");
        E_enventry entry = NULL;
        A_field resultVar = NULL;
        // printf("%s func->name\n", S_name(d -> u.funcImport.name));
        if (returnList->head)
        {
            // printf("there is head\n");
            resultVar = returnList->head;
            entry = E_FuncEntry(level, d -> u.funcImport.name, paramTypes, Tr_allocLocal(newLevel, resultVar->escape, transTy(tenv, resultVar->typ)));
        }
        else
            entry = E_FuncEntry(level, d -> u.funcImport.name, paramTypes, Tr_allocLocal(newLevel, FALSE, Ty_Void()));

        S_enter(venv, d -> u.funcImport.name, entry);
        T_module func;
        if(returnList -> head != NULL)
        {
            func = T_FuncMod(T_Fundec(convertAllType(paramTypes), NULL, convertType(transTy(tenv, resultVar->typ)),
                        NULL, S_name(d -> u.funcImport.name), entry->u.func.index, NULL));
        }
        else
        {
            func = T_FuncMod(T_Fundec(convertAllType(paramTypes), NULL, convertType(Ty_Void()),
                        NULL, S_name(d -> u.funcImport.name), entry->u.func.index, NULL));
        }
        return T_ImportMod(d -> u.funcImport.mod, d -> u.funcImport.func, func);
    }
    case A_classDec:
    {
        S_table varTypes = S_empty();
        S_table methods = S_empty();
        int classSize = 0;
        for(A_classMemberList list = d -> u.class.members; list -> tail; list = list -> tail)
        {
            // printf("debug\n");
            if(list -> head -> dec -> kind == A_varDec)
            {
                Ty_ty varType = transTy(tenv, list -> head -> dec -> u.var.typ);
                S_symbol name;
                if(list -> head -> dec -> u.var.var -> kind == A_simpleVar)
                    name = list -> head -> dec -> u.var.var -> u.simple;
                else if(list -> head -> dec -> u.var.var -> kind == A_subscriptVar)
                    name = list -> head -> dec -> u.var.var -> u.subscript.name;
                else
                    EM_error(list -> head -> dec -> pos, "Invalid declaration.");
                S_enter(varTypes, name, Ty_Member(classSize, varType));
                classSize += varType -> size;
            }
        }
        S_enter(tenv, d -> u.class.name, E_ClassEntry(d -> u.class.name, classSize, varTypes, methods));
        T_moduleList modlist = checked_malloc(sizeof(*modlist));
        for(A_classMemberList list = d -> u.class.members; list -> tail; list = list -> tail)
        {
            if(list -> head -> dec -> kind == A_functionDec)
            {
                modlist = T_ModuleList(transDec(venv, tenv, list -> head -> dec, level, isLoop, methods), modlist);
            }
        }
        return T_SeqMod(modlist);
    }
    }
}

Ty_ty transTy(S_table tenv, A_ty ty)
{
    static int arrayDepth = 0; 
    switch (ty->kind)
    {
    case A_nameTy:
    {
        Ty_ty type;
        if (type = checkSymType(ty->u.name))
        {
            return type;
        }
        else
        {
            type = S_look(tenv, ty->u.name);
            if (!type)
            {
                return Ty_Name(ty->u.name);
            }
            return type;
        }
    }
    case A_pointerTy:
    {
        Ty_ty type;
        if (type = transTy(tenv, ty->u.pointer))
        {
            type = Ty_Pointer(type);
            return type;
        }
        else
        {
            type = S_look(tenv, ty->u.name);
            if (!type)
            {
                return Ty_Name(ty->u.name);
            }
            return Ty_Pointer(type);
        }
    }
    case A_arrayTy:
    {
        // printf("%d arraydepth\n", arrayDepth);
        if(arrayDepth==0)
            ty=reverseArrayTy(ty);
        arrayDepth++;
        Ty_ty type = transTy(tenv, ty->u.array.type);
        arrayDepth = 0;
        int a = ty -> u.array.size;
        // printf("%d size kind1420\n", ty -> u.array.size);
        if (type)
        {
            type = Ty_Array(type, ty->u.array.size);
            // printf("%d 1424\n", type -> u.array.type -> size);
            return type;
        }
        else
        {
            type = S_look(tenv, ty->u.name);
            if (!type)
            {
                EM_error(ty->pos, "Type does not exist.\n");
            }
            return Ty_Array(type, ty->u.array.size);
        }
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

static Ty_tyList makeParamTypeList(S_table tenv, A_fieldList params)
{
    if (params == NULL)
    {
        return NULL;
    }
    Ty_tyList typeList = Ty_TyList(NULL, NULL);
    //make typeList_head point to typeList
    Ty_tyList typeList_head = typeList;
    A_ty type;
    Ty_ty ty;
    if (!params->head)
        return NULL;
    for (; params != NULL; params = params->tail)
    {
        type = params->head->typ;
        if (ty = transTy(tenv, type))
        {
            // printf("%d ty\n", ty -> kind);
            typeList->head = ty;
        }
        else
        {
            S_symbol sym = returnSymFromType(actual_ty(transTy(tenv, type)));
            ty = S_look(tenv, sym);
            if (ty != NULL)
            {
                typeList->head = ty;
            }
            else
            {
                EM_error(params->head->pos, "Undefined Type: %s", S_name(sym));
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
    else if (sym == S_Symbol("string"))
    {
        return Ty_String();
    }
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
    U_boolList list = checked_malloc(sizeof(*list));
    U_boolList retValue = list;
    if (!params->head)
        return NULL;
    for (; params; params = params->tail)
    {
        list->head = params->head->escape;
        list->tail = checked_malloc(sizeof(*list->tail));
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
    case Ty_record:
    {
        EM_error(0, "Records not supported right now.");
    }
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
    case Ty_string:
    {
        EM_error(0, "Strings not supported right now.");
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
            EM_error(0, "Name variables not supported right now.");
        }
    }
    case Ty_pointer:
    {
        result = T_i32;
        break;
    }
    }
    return result;
}
static T_typeList convertAllType(Ty_tyList types)
{
    T_typeList list = checked_malloc(sizeof(*list));
    T_typeList result = list;
    if(!types)
    {
        return result;
    }
    for (; types->tail; types = types->tail)
    {
        list->head = convertType(types->head);
        list->tail = checked_malloc(sizeof(*list));
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
    }
    return result;
}

static T_exp convertAssign(Ty_ty type, struct expty exp, int pos)
{
    T_exp result;
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
        EM_error(pos, "Type %s %d does not match with type \'%s %d\'.", S_name(returnSymFromType(actual_ty(exp.ty))), exp.ty -> kind, S_name(returnSymFromType(actual_ty(type))), type -> kind);
    return result;
}

static A_ty reverseArrayTy(A_ty type)
{
    if(type -> kind != A_arrayTy)
    {
        EM_error(0, "This type cannot be reversed.");
    }
    A_ty result;
    A_ty temp, base;
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
    
    A_var temp;
    A_var result;
    IntList indexList = checked_malloc(sizeof(*indexList));
    IntList indexResult = indexList;
    for(temp = var; temp -> kind == A_subscriptVar; temp = temp -> u.subscript.var)
    {
        indexList -> i = temp -> u.subscript.exp -> u.intt;
        indexList -> rest = checked_malloc(sizeof(*indexList));
        indexList = indexList -> rest;
        indexList -> rest = NULL;
    }
    // printf("yo? %d\n", indexResult -> rest);
    for(result = A_SimpleVar(var -> pos, var -> u.subscript.name);indexResult -> rest; indexResult = indexResult -> rest)
    {
        result = A_SubscriptVar(var -> pos, result, A_IntExp(var -> pos, indexResult -> i));
    }
    return result;
}