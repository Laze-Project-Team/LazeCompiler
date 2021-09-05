/*
 * absyn.c - Abstract Syntax Functions. Most functions create an instance of an
 *           abstract syntax rule.
 */

#include <string.h>
#include "util.h"
#include "symbol.h" /* symbol table data structures */
#include "absyn.h"  /* abstract syntax data structures */
#include "escape.h"

A_stm A_AssignStm(A_pos pos, A_var var, A_exp exp, bool isDec)
{
    A_stm p = checked_malloc(sizeof(*p));
    p-> kind = A_assignStm;
    p-> pos = pos;
    p-> u.assign.var = var;
    p-> u.assign.exp = exp;
    p -> u.assign.isDec = isDec;
    p -> u.assign.isArrayExp = FALSE;
    return p;
}
A_stm A_IfStm(A_pos pos, A_exp test, A_stm then, A_stm elsee)
{
    A_stm p = checked_malloc(sizeof(*p));
    p-> kind = A_ifStm;
    p-> pos = pos;
    p-> u.iff.test = test;
    p-> u.iff.then = then;
    p-> u.iff.elsee = elsee;
    return p;
}
A_stm A_WhileStm(A_pos pos, A_exp test, A_stm body)
{
    A_stm p = checked_malloc(sizeof(*p));
    p-> kind = A_whileStm;
    p-> pos = pos;
    p-> u.whilee.test = test;
    p-> u.whilee.body = body;
    return p;
}
A_stm A_ForStm(A_pos pos, A_stm assign, A_exp condition, A_stm increment, A_stm body)
{
    A_stm p = checked_malloc(sizeof(*p));
    p-> kind = A_forStm;
    p-> pos = pos;
    p-> u.forr.assign = assign;
    p-> u.forr.condition = condition;
    p-> u.forr.increment = increment;
    p-> u.forr.body = body;
    return p;
}
A_stm A_BreakStm(A_pos pos)
{
    A_stm p = checked_malloc(sizeof(*p));
    p-> kind = A_breakStm;
    p-> pos = pos;
    return p;
}
A_stm A_ContinueStm(A_pos pos)
{
    A_stm p = checked_malloc(sizeof(*p));
    p-> kind = A_continueStm;
    return p;
}
A_stm A_CompoundStm(A_pos pos, A_stmList stmlist)
{
    A_stm p = checked_malloc(sizeof(*p));
    p-> kind = A_compoundStm;
    p-> pos = pos;
    p -> u.compound = stmlist;
    return p;
}
A_stm A_DeclarationStm(A_pos pos, A_dec dec)
{
    A_stm p = checked_malloc(sizeof(*p));
    p-> kind = A_declarationStm;
    p-> pos = pos;
    p-> u.declaration.dec = dec;
    return p;
}
A_stm A_CallStm(A_pos pos, A_exp func, A_expList args)
{
    A_stm p = checked_malloc(sizeof(*p));
    p-> kind = A_callStm;
    p-> pos = pos;
    p-> u.call.func = func;
    p->u.call.args  = args;
    return p;
}
A_stm A_ReturnStm(A_pos pos, A_exp exp)
{
    A_stm p = checked_malloc(sizeof(*p));
    p-> kind = A_returnStm;
    p -> pos = pos;
    p-> u.returnn.ret = exp;
    return p;
}
A_stm A_LoopStm(A_pos pos, A_stm body)
{
    A_stm p = checked_malloc(sizeof(*p));
    p -> kind = A_loopStm;
    p -> pos = pos;
    p -> u.loop.body = body;
    return p;
}

A_var A_SimpleVar(A_pos pos, S_symbol sym)
{
    A_var p = checked_malloc(sizeof(*p));
    p->kind=A_simpleVar;
    p -> lvalue = FALSE;
    p->pos=pos;
    p->u.simple=sym;
    return p;
}

A_var A_LvalSimpleVar(A_pos pos, S_symbol sym)
{
    A_var p = checked_malloc(sizeof(*p));
    p -> kind = A_simpleVar;
    p -> lvalue = TRUE;
    p -> u.simple=sym;
    return p;
}

A_var A_FieldVar(A_pos pos, A_var var, S_symbol sym)
{
    A_var p = checked_malloc(sizeof(*p));
    p->kind=A_fieldVar;
    p -> lvalue = FALSE;
    p->pos=pos;
    p->u.field.var=var;
    p->u.field.sym=sym;
    return p;
}

A_var A_SubscriptVar(A_pos pos, A_var var, A_exp exp)
{
    A_var p = checked_malloc(sizeof(*p));
    p->kind=A_subscriptVar;
    p -> lvalue = FALSE;
    p->pos=pos;
    p->u.subscript.var=var;
    p -> u.subscript.depth=0;
    if(p -> u.subscript.var -> kind == A_subscriptVar)
    {
        p -> u.subscript.depth= var -> u.subscript.depth + 1;
    }
    switch(var -> kind)
    {
        case A_simpleVar:
            p -> u.subscript.name = var -> u.simple;
            break;
        case A_subscriptVar:
            p -> u.subscript.name = var -> u.subscript.name;
            break;
        case A_fieldVar:
            p -> u.subscript.name = var -> u.field.sym;
            break;
    }
    
    p->u.subscript.exp=exp;
    return p;
}

A_var A_DerefVar(A_pos pos, A_var deref)
{
    A_var p = checked_malloc(sizeof(*p));
    p -> kind = A_derefVar;
    p -> lvalue = FALSE;
    p -> pos = pos;
    p -> u.deref = deref;
    return p;
}
A_var A_ArrowFieldVar(A_pos pos, A_var pointer, S_symbol member)
{
    A_var p = checked_malloc(sizeof(*p));
    p -> kind = A_arrowFieldVar;
    p -> pos = pos;
    p -> lvalue = FALSE;
    p -> u.arrowfield.pointer = pointer;
    p -> u.arrowfield.member = member;
    return p;
}

A_exp A_VarExp(A_pos pos, A_var var)
{
    A_exp p = checked_malloc(sizeof(*p));
    p->kind=A_varExp;
    p->pos=pos;
    p->u.var=var;
    return p;
}

A_exp A_NilExp(A_pos pos)
{
    A_exp p = checked_malloc(sizeof(*p));
    p->kind=A_nilExp;
    p->pos=pos;
    return p;
}

A_exp A_CharExp(A_pos pos, char c[3])
{
    A_exp p = checked_malloc(sizeof(*p));
    p -> kind = A_charExp;
    p -> pos = pos;
    strcpy(p -> u.charr, c);
    return p;
}

A_exp A_IntExp(A_pos pos, long long i)
{
    A_exp p = checked_malloc(sizeof(*p));
    p->kind=A_intExp;
    p->pos=pos;
    p->u.intt=i;
    return p;
}

A_exp A_StringExp(A_pos pos, string s)
{
    A_exp p = checked_malloc(sizeof(*p));
    p->kind=A_stringExp;
    p->pos=pos;
    p->u.stringg=s;
    return p;
}

A_exp A_RealExp(A_pos pos, double f)
{
    A_exp p = checked_malloc(sizeof(*p));
    p->kind=A_realExp;
    p->pos=pos;
    p->u.real=f;
    return p;
}

A_exp A_BoolExp(A_pos pos, bool b)
{
    A_exp p = checked_malloc(sizeof(*p));
    p -> kind = A_boolExp;
    p -> pos = pos;
    p -> u.booll = b;
    return p;
}

A_exp A_CallExp(A_pos pos, A_exp func, A_expList args)
{
    A_exp p = checked_malloc(sizeof(*p));
    p->kind=A_callExp;
    p->pos=pos;
    p->u.call.func=func;
    p->u.call.args =args;
    return p;
}

A_exp A_OpExp(A_pos pos, A_oper oper, A_exp left, A_exp right)
{
    A_exp p = checked_malloc(sizeof(*p));
    p->kind=A_opExp;
    p->pos=pos;
    p->u.op.oper=oper;
    p->u.op.left=left;
    p->u.op.right=right;
    return p;
}

A_exp A_RecordExp(A_pos pos, S_symbol typ, A_efieldList fields)
{
    A_exp p = checked_malloc(sizeof(*p));
    p->kind=A_recordExp;
    p->pos=pos;
    p->u.record.typ=typ;
    p->u.record.fields=fields;
    return p;
}

A_exp A_SeqExp(A_pos pos, A_expList seq)
{
    A_exp p = checked_malloc(sizeof(*p));
    p->kind=A_seqExp;
    p->pos=pos;
    p->u.seq=seq;
    return p;
}

A_exp A_AssignExp(A_pos pos, A_var var, A_exp exp)
{
    A_exp p = checked_malloc(sizeof(*p));
    p->kind=A_assignExp;
    p->pos=pos;
    p->u.assign.var=var;
    p->u.assign.exp=exp;
    return p;
}

A_exp A_IfExp(A_pos pos, A_exp test, A_exp then, A_exp elsee)
{
    A_exp p = checked_malloc(sizeof(*p));
    p->kind=A_ifExp;
    p->pos=pos;
    p->u.iff.test=test;
    p->u.iff.then=then;
    p->u.iff.elsee=elsee;
    return p;
}

A_exp A_ArrayExp(A_pos pos, A_expList list)
{
    A_exp p = checked_malloc(sizeof(*p));
    p->kind=A_arrayExp;
    p->pos=pos;
    p->u.array.list = list;
    // p -> u.array.type = type;
    return p;
}

A_exp A_DerefExp(A_pos pos, A_var deref)
{
    A_exp p = checked_malloc(sizeof(*p));
    p -> kind = A_derefExp;
    p -> pos = pos;
    p -> u.deref = deref;
    return p;
}

A_exp A_AddressExp(A_pos pos, A_var address)
{
    A_exp p = checked_malloc(sizeof(*p));
    p -> kind = A_addressExp;
    p -> pos = pos;
    p -> u.address = address;
    return p;
}
A_exp A_SizeofExp(A_pos pos, A_var sizeOf)
{
    A_exp p = checked_malloc(sizeof(*p));
    p -> kind = A_sizeofExp;
    p -> pos = pos;
    p -> u.sizeOf = sizeOf;
    return p;
}
A_exp A_FieldExp(A_pos pos, A_exp field, S_symbol member)
{
    A_exp p = checked_malloc(sizeof(*p));
    p -> kind = A_fieldExp;
    p -> pos = pos;
    p -> u.field.field = field;
    p -> u.field.member = member;
    return p;
}
A_exp A_SubscriptExp(A_pos pos, A_exp array, A_exp index)
{
    A_exp p = checked_malloc(sizeof(*p));
    p -> kind = A_subscriptExp;
    p -> pos = pos;
    p -> u.subscript.array = array;
    p -> u.subscript.index = index;
    return p;
}
A_exp A_ArrowFieldExp(A_pos pos, A_exp pointer, S_symbol member)
{
    A_exp p = checked_malloc(sizeof(*p));
    p -> kind = A_arrowFieldExp;
    p -> pos = pos;
    p -> u.arrowfield.pointer = pointer;
    p -> u.arrowfield.member = member;
    return p;
}

A_dec A_FunctionDec(A_pos pos, A_fundecList function)
{
    A_dec p = checked_malloc(sizeof(*p));
    p->kind=A_functionDec;
    p->pos=pos;
    p->u.function=function;
    return p;
}

A_dec A_VarDec(A_pos pos, A_stm assign, A_ty typ)
{
    A_dec p = checked_malloc(sizeof(*p));
    p->kind=A_varDec;
    p->pos=pos;
    p->u.var.var=assign->u.assign.var;
    if(assign -> u.assign.var -> kind == A_subscriptVar)
    {
        for(A_var var = assign -> u.assign.var; var -> kind == A_subscriptVar; var = var -> u.subscript.var)
        {
            // printf("test\n");
            typ = A_ArrayTy(pos, typ, var -> u.subscript.exp -> u.intt);
        }
        // printf("dasfdas\n");
    }
    p->u.var.typ=typ;
    p->u.var.init=assign->u.assign.exp;
    p->u.var.escape=TRUE;
    return p;
}

A_dec A_TypeDec(A_pos pos, A_nametyList type)
{
    A_dec p = checked_malloc(sizeof(*p));
    p->kind=A_typeDec;
    p->pos=pos;
    p->u.type=type;
    return p;
}

A_dec A_FuncImport(A_pos pos, S_symbol name, A_fieldList params, A_fieldList result, string mod, string func)
{
    A_dec p = checked_malloc(sizeof(*p));
    p -> kind = A_funcImportDec;
    p -> pos = pos;
    p -> u.funcImport.name = name;
    p -> u.funcImport.params = params;
    p -> u.funcImport.result = result;
    p -> u.funcImport.mod = mod;
    p -> u.funcImport.func = func;
    return p;
}

A_dec A_FuncExport(A_pos pos, S_symbol name, string exportName){
    A_dec p = checked_malloc(sizeof(*p));
    p -> kind = A_funcExportDec;
    p -> pos = pos;
    p -> u.funcExport.name = name;
    p -> u.funcExport.exportName = exportName;
    return p;
}

A_dec A_ClassDec(A_pos pos, S_symbol name, A_classMemberList members, S_symbolList inheritance)
{
    A_dec p = checked_malloc(sizeof(*p));
    p -> kind = A_classDec;
    p -> pos = pos;
    p -> u.class.name = name;
    p -> u.class.members = members;
    p -> u.class.inheritance = inheritance;
    return p;
}

A_dec A_ObjectDec(A_pos pos, A_ty className, S_symbol varName, A_expList explist)
{
    A_dec p = checked_malloc(sizeof(*p));
    p -> kind = A_objectDec;
    p -> u.object.className = className;
    p -> u.object.name = varName;
    p -> u.object.explist = explist;
    return p;
}

A_dec A_TemplateDec(A_pos pos, S_symbol name, A_dec dec)
{
    A_dec p = checked_malloc(sizeof(*p));
    p -> kind = A_templateDec;
    p -> u.template.dec = dec;
    p -> u.template.name = name;
    return p;
}

A_ty A_NameTy(A_pos pos, S_symbol name)
{
    A_ty p = checked_malloc(sizeof(*p));
    p->kind=A_nameTy;
    p->pos=pos;
    p->u.name=name;
    return p;
}

A_ty A_RecordTy(A_pos pos, A_fieldList record)
{
    A_ty p = checked_malloc(sizeof(*p));
    p->kind=A_recordTy;
    p->pos=pos;
    p->u.record=record;
    return p;
}

A_ty A_ArrayTy(A_pos pos, A_ty array, int size)
{
    A_ty p = checked_malloc(sizeof(*p));
    p->kind=A_arrayTy;
    p->pos=pos;
    p->u.array.type=array;
    p -> u.array.size = size;
    return p;
}

A_ty A_PointerTy(A_pos pos, A_ty type)
{
    A_ty p = checked_malloc(sizeof(*p));
    p -> kind = A_pointerTy;
    p -> pos = pos;
    p -> u.pointer = type;
    return p;
}

A_ty A_PolyTy(A_pos pos, S_symbol name, A_ty typeParam)
{
    A_ty p = checked_malloc(sizeof(*p));
    p -> kind = A_polyTy;
    p -> pos = pos;
    p -> u.poly.name = name;
    p -> u.poly.typeParam = typeParam;
    return p;
}

A_field A_Field(A_pos pos, S_symbol name, A_ty typ)
{
    A_field p = checked_malloc(sizeof(*p));
    p->pos=pos;
    p->name=name;
    p->typ=typ;
    p -> escape = ESC_checkEscapeFromA_ty(typ);
    return p;
}

A_fieldList A_FieldList(A_field head, A_fieldList tail)
{
    A_fieldList p = checked_malloc(sizeof(*p));
    p->head=head;
    p->tail=tail;
    return p;
}

A_expList A_ExpList(A_exp head, A_expList tail)
{
    A_expList p = checked_malloc(sizeof(*p));
    p->head=head;
    p->tail=tail;
    return p;
}

A_stmList A_StmList(A_stm head, A_stmList tail)
{
    A_stmList p = checked_malloc(sizeof(*p));
    // printf("%d", head -> kind);
    p->head=head;
    p->tail=tail;
    return p;
}

A_fundec A_Fundec(A_pos pos, S_symbol name, A_fieldList params, A_fieldList result, A_stm body)
{
    A_fundec p = checked_malloc(sizeof(*p));
    p->pos=pos;
    p->name=name;
    p->params=params;
    p->result=result;
    p->body=body;
    return p;
}

A_fundecList A_FundecList(A_fundec head, A_fundecList tail)
{
    A_fundecList p = checked_malloc(sizeof(*p));
    p->head=head;
    p->tail=tail;
    return p;
}

A_decList A_DecList(A_dec head, A_decList tail)
{
    A_decList p = checked_malloc(sizeof(*p));
    p->head=head;
    p->tail=tail;
    return p;
}

A_namety A_Namety(S_symbol name, A_ty ty)
{
    A_namety p = checked_malloc(sizeof(*p));
    p->name=name;
    p->ty=ty;
    return p;
}

A_nametyList A_NametyList(A_namety head, A_nametyList tail)
{
    A_nametyList p = checked_malloc(sizeof(*p));
    p->head=head;
    p->tail=tail;
    return p;
}

A_efield A_Efield(S_symbol name, A_exp exp)
{
    A_efield p = checked_malloc(sizeof(*p));
    p->name=name;
    p->exp=exp;
    return p;
}

A_efieldList A_EfieldList(A_efield head, A_efieldList tail)
{
    A_efieldList p = checked_malloc(sizeof(*p));
    p->head=head;
    p->tail=tail;
    return p;
}

A_classMember A_ClassMember(A_classMemberSpecifier specifier, A_dec dec)
{
    A_classMember p = checked_malloc(sizeof(*p));
    p -> accessSpecifier = specifier;
    p -> dec = dec;
    return p;
}

A_classMemberList A_ClassMemberList(A_classMember head, A_classMemberList tail)
{
    A_classMemberList p = checked_malloc(sizeof(*p));
    p -> head = head;
    p -> tail = tail;
    return p;
}

A_classMemberList A_ClassMemFromDecList(A_decList decs, A_classMemberSpecifier specifier)
{
    A_classMemberList p = checked_malloc(sizeof(*p));
    A_classMemberList result = p;
    for(A_decList list = decs; list; list = list -> tail)
    {
        p -> head = A_ClassMember(specifier, list -> head);
        p -> tail = checked_malloc(sizeof(*p));
        p = p -> tail;
        p -> head = NULL;
        p -> tail = NULL;
    }
    return result;
}

A_classMemberList A_ClassMemFromTwoList(A_classMemberList list1, A_classMemberList list2)
{
    A_classMemberList p = checked_malloc(sizeof(*p));
    
    if(!list2){
        p = A_ClassMemberList(NULL, NULL);
    }
    else{
        p = list2;
    }
    for(A_classMemberList list = list1; list -> tail; list = list -> tail)
    {
        p = A_ClassMemberList(list -> head, p);
    }
    return p;
}
