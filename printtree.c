#include "printtree.h"
#include <string.h>

extern int memorySize;

void Pr_printTree(T_moduleList list, string fileName)
{
    FILE *file = fopen(fileName, "w");
    fprintf(file, "(module ");
    T_moduleList modList = list;
    for(;modList -> tail != NULL; modList = modList -> tail)
    {
        // printf("aaaaaaaaaaaaaaaaaaaaaaaaaaaaa\n");
        Pr_printModule(modList -> head, file);
    }
    fprintf(file, "(func $memorySize (result i32) (return (i32.const %d))) (export \"memorySize\"(func $memorySize))", memorySize);
    fprintf(file, ")");
    fclose(file);
}
void Pr_printModule(T_module module, FILE *file)
{
    if(!module)
    {
        // assert(module);
        return;
    }
    switch(module -> kind)
    {
        case T_typee:
        {
            EM_error(0, "Type module is not implemented yet.");
            break;
        }
        case T_func:
        {
            Pr_printFundec(module -> u.func, file);   
            break;
        }
        case T_table:
        {
            break;
        }
        case T_mem:
        {
            break;
        }
        case T_global:
        {
            fprintf(file, "(global ");
            fprintf(file, "(mut ");
            Pr_printType(module -> u.global.type, file);
            fprintf(file, ")");
            Pr_printExp(module -> u.global.exp, file);
            fprintf(file, ")");
            break;
        }
        case T_elem:
        {
            break;
        }
        case T_data:
        {
            break;
        }
        case T_start:
        {
            break;
        }
        case T_import:
        {
            fprintf(file, "(import \"%s\" \"%s\" ", module -> u.import.module, module -> u.import.name);
            Pr_printModPrototype(module -> u.import.import, file);
            fprintf(file, ")");
            break;
        }
        case T_export:
        {
            fprintf(file, "(export \"%s\" ", module -> u.export.name);
            fprintf(file, "(func %d)", module -> u.export.export);
            fprintf(file, ")");
            break;
        }
        case T_seqMod:
        {
            for(T_moduleList list = module -> u.seq; list -> tail; list = list -> tail)
            {
                Pr_printModule(list -> head, file);
            }
            break;
        }
    }
}
void Pr_printStm(T_stm stm, FILE *file)
{
    if(!stm)
    {
        // assert(stm);
        return;
    }
    switch(stm -> kind)
    {
        case T_ifStm:
        {
            fprintf(file, "(if");
            Pr_printExp(stm -> u.iff.test, file);
            fprintf(file, "(then ");
            Pr_printStm(stm -> u.iff.then, file);
            fprintf(file, ")");
            if(stm -> u.iff.elsee)
            {
                fprintf(file, "(else ");
                Pr_printStm(stm -> u.iff.elsee, file);
                fprintf(file, ")");
            }
            fprintf(file, ")");
            break;
        } 
        case T_blockStm:
        {
            fprintf(file, "(block");
            Pr_printStm(stm -> u.block.body, file);
            fprintf(file, ")");
            break;
        } 
        case T_loopStm:
        {
            fprintf(file, "(loop");
            if(!(stm -> u.loop.isFor))
            {
                Pr_printStm(stm -> u.loop.body, file);
                fprintf(file, "(br_if %d", (stm -> u.loop.index < 0) ? (1) : (stm -> u.loop.index));
                Pr_printExp(stm -> u.loop.test, file);
                fprintf(file, ")");
            }
            else
            {
                fprintf(file, "(br_if %d", (stm -> u.loop.index < 0) ? (1) : (stm -> u.loop.index));
                Pr_printExp(stm -> u.loop.test, file);
                fprintf(file, ")");
                Pr_printStm(stm -> u.loop.body, file);
            }
            fprintf(file, "(br %d)", (stm -> u.loop.index < 0) ? (0) : (stm -> u.loop.index - 1));
            fprintf(file, ")");
            break;
        }
        case T_binOpStm:
        {
            EM_error(0, "T_binOpStm does not exist.");
            break;
        }
        case T_setLocalStm:
        {
            fprintf(file, "(set_local %d", stm -> u.setLocal.index);
            Pr_printExp(stm -> u.setLocal.exp, file);
            fprintf(file, ")");
            break;
        }
        case T_setGlobalStm:
        {
            fprintf(file, "(set_global %d", stm -> u.setGlobal.index);
            Pr_printExp(stm -> u.setGlobal.exp, file);
            fprintf(file, ")");
            break;
        }
        case T_seqStm:
        {
            T_stmList tempList = stm -> u.seq;
            for(;tempList -> tail; tempList = tempList -> tail)
            {
                Pr_printStm(tempList -> head, file);
            }
            break;
        }
        case T_storeStm:
        {
            fprintf(file, "(");
            Pr_printType(stm -> u.store.data -> type, file);
            fprintf(file, ".store ");
            Pr_printExp(stm -> u.store.addr, file);
            Pr_printExp(stm -> u.store.data, file);
            fprintf(file, ")");
            break;
        }
        case T_breakStm:
        {
            fprintf(file, "(br %d)", stm -> u.depth);
            break;
        }
        case T_callStm:
        {
            // printf("%d index\n", stm -> u.call.index);
            fprintf(file, "(call %d", stm -> u.call.index);
            T_expList list = stm -> u.call.args;
            for(; list -> tail!= NULL; list = list -> tail)
            {
                Pr_printExp(list -> head, file);
            }
            fprintf(file, ")");
            break;
        }
        case T_returnStm:
        {
            fprintf(file, "(return ");
            if(stm -> u.returnn)
                Pr_printExp(stm -> u.returnn, file);
            fprintf(file, ")");
            break;
        }
        case T_copyStm:
        {
            fprintf(file, "(memory.copy ");
            Pr_printExp(stm -> u.copy.dest, file);
            Pr_printExp(stm -> u.copy.src, file);
            Pr_printExp(stm -> u.copy.size, file);
            fprintf(file, ")");
            break;
        }
    }
}
void Pr_printExp(T_exp exp, FILE *file)
{
    if(!exp)
    {
        // assert(exp);
        return;
    }
    switch(exp -> kind)
    {
        case T_binOpExp:
        {
            fprintf(file, "(");
            Pr_printType(exp -> type, file);
            fprintf(file, ".");
            Pr_printBiOp(exp -> u.binOp.oper, exp -> type, file);
            Pr_printExp(exp -> u.binOp.lhs, file);
            Pr_printExp(exp -> u.binOp.rhs, file);
            fprintf(file, ")");
            break;
        }
        case T_uniOpExp:
        {
            fprintf(file, "(");
            Pr_printType(exp -> type, file);
            fprintf(file, ".");
            Pr_printUniOp(exp -> u.uniOp.oper, file);
            Pr_printExp(exp -> u.uniOp.exp, file);
            fprintf(file, ")");
            break;
        }
        case T_constExp:
        {
            fprintf(file, "(");
            Pr_printType(exp -> type, file);
            fprintf(file, ".const ");
            Pr_printAExp(exp -> u.constt.exp, file);
            fprintf(file, ")");
            break;
        }
        case T_getLocalExp:
        {
            fprintf(file, "(get_local %d)", exp -> u.getLocal.index);
            break;
        }
        case T_getGlobalExp:
        {
            fprintf(file, "(get_global %d)", exp -> u.getGlobal.index);
            break;
        }
        case T_callExp:
        {
            fprintf(file, "(call %d", exp -> u.call.index);
            Pr_printExpList(exp -> u.call.args, file);
            fprintf(file, ")");
            // fprintf(file, "(call %d", exp -> u.call.index);
            // T_expList list = exp -> u.call.args;
            // for(; list -> tail!= NULL; list = list -> tail)
            // {
            //     Pr_printExp(list -> head, file);
            // }
            // fprintf(file, ")");
            break;
        }
        case T_ifExp:
        {
            fprintf(file, "(if(");
            Pr_printType(exp -> type, file);
            fprintf(file, ")");
            Pr_printExp(exp -> u.iff.test, file);
            fprintf(file, "(block ");
            Pr_printExp(exp -> u.iff.then, file);
            fprintf(file, ")");
            Pr_printExp(exp -> u.iff.elsee, file);
            
            fprintf(file, ")");
            break;
        }
        case T_convertExp:
        {
            fprintf(file, "(");
            switch(exp -> type)
            {
                case T_i32:
                {
                    switch(exp -> u.convert.exp -> type)
                    {
                        case T_i32:
                            break;
                        case T_i64:
                            fprintf(file, "i32.wrap_i64");
                            break;
                        case T_f32:
                            fprintf(file, "i32.trunc_f32_s");
                            break;
                        case T_f64:
                            fprintf(file, "i32.trunc_f64_s");
                            break;
                        default:
                            EM_error(0, "Could not convert to type i32. Type %d", exp -> u.convert.exp -> type);
                            break;
                    }
                    break;
                }
                case T_i64:
                {
                    switch(exp -> u.convert.exp -> type)
                    {
                        case T_i32:
                            fprintf(file, "i64.extend_i32_s");
                            break;
                        case T_i64:
                            break;
                        case T_f32:
                            fprintf(file, "i64.trunc_f32_s");
                            break;
                        case T_f64:
                            fprintf(file, "i64.trunc_f64_s");
                            break;
                        default:
                            EM_error(0, "Could not convert to type i64. Exp type %d", exp -> u.convert.exp -> kind);
                            break;
                    }
                    break;
                }
                case T_f32:
                {
                    switch(exp -> u.convert.exp -> type)
                    {
                        case T_i32:
                            fprintf(file, "f32.convert_i32_s");
                            break;
                        case T_i64:
                            fprintf(file, "f32.convert_i64_s");
                            break;
                        case T_f32:
                            break;
                        case T_f64:
                            fprintf(file, "f32.demote_f64");
                            break;
                        default:
                            EM_error(0, "Could not convert to type f32.");
                            break;
                    }
                    break;
                }
                case T_f64:
                {
                    switch(exp -> u.convert.exp -> type)
                    {
                        case T_i32:
                            fprintf(file, "f64.convert_i32_s");
                            break;
                        case T_i64:
                            fprintf(file, "f64.convert_i64_s");
                            break;
                        case T_f32:
                            fprintf(file, "f64.promote_f32");
                            break;
                        case T_f64:
                            break;
                        default:
                            EM_error(0, "Could not convert to type f64.");
                            break;
                    }
                    break;
                }
            }
            Pr_printExp(exp -> u.convert.exp, file);
            fprintf(file, ")");
            break;
        }
        case T_loadExp:
        {
            fprintf(file, "(");
            Pr_printType(exp -> type, file);
            fprintf(file, ".load ");
            Pr_printExp(exp -> u.load.addr, file);
            fprintf(file, ")");
            break;
        }  
    }
}
void Pr_printFundec(T_fundec fundec, FILE *file)
{
    if(!fundec)
    {
        // assert(fundec);
        return;
    }
    fprintf(file, "(func ");
    if(strlen(fundec -> name) > 0)
        fprintf(file, "$%s", fundec -> name);
    T_typeList listP;
    T_typeList listL;
    if(fundec -> params){
        for(listP = fundec -> params; listP -> tail; listP = listP -> tail)
        {
            fprintf(file, "(param ");
            Pr_printType(listP -> head, file);
            fprintf(file, ")");
        }
    }
    if(fundec -> result != T_none)
    {
        fprintf(file, "(result ");
        Pr_printType(fundec -> result, file);
        fprintf(file, ")");
    }
    for(listL = fundec -> locals; listL -> tail; listL = listL -> tail)
    {
        fprintf(file, "(local ");
        Pr_printType(listL -> head, file);
        fprintf(file, ")");
    }
    if(strcmp(fundec -> name, "main") == 0)
        fprintf(file, "(memory.fill (i32.const 0) (i32.const 0) (i32.const 1114112))");
    Pr_printStm(fundec -> body, file);
    if(fundec -> var)
    {
        Pr_printExp(fundec -> var, file);
    }
    fprintf(file, ")");
}
void Pr_printExpList(T_expList list, FILE *file)
{
    // printf("test\n");
    if(!list)
        return;
    T_expList tempList = list;
    for(; tempList -> tail; tempList = tempList -> tail)
    {
        Pr_printExp(tempList -> head, file);
    }
}
void Pr_printType(T_type type, FILE *file)
{
    switch(type)
    {
        case T_i32:
        {
            fprintf(file, "i32");
            break;
        }
        case T_i64:
        {
            fprintf(file, "i64");
            break;
        }
        case T_f32:
        {
            fprintf(file, "f32");
            break;
        }
        case T_f64:
        {
            fprintf(file, "f64");
            break;
        }
    }
}
void Pr_printBiOp(T_binOp oper, T_type type, FILE *file)
{
    switch(oper)
    {
        case T_add:
        {
            fprintf(file, "add");
            break;
        }
        case T_sub:
        {
            fprintf(file, "sub");
            break;
        }
        case T_mul:
        {
            fprintf(file, "mul");
            break;
        }
        case T_div_s:
        {
            if(type == T_i64 || type == T_i32)
                fprintf(file, "div_s");
            else if(type == T_f64)
                fprintf(file, "div");
            break;
        }
        case T_div_u:
        {
            fprintf(file, "div_u");
            break;
        }
        case T_rem_s:
        {
            fprintf(file, "rem_s");
            break;
        }
        case T_rem_u:
        {
            fprintf(file, "rem_u");
            break;
        }
        case T_eq:
        {
            fprintf(file, "eq");
            break;
        }
        case T_ne:
        {
            fprintf(file, "ne");
            break;
        }
        case T_lt_s:
        {
            if(type == T_i64 || type == T_i32)
                fprintf(file, "lt_s");
            else if(type == T_f64)
                fprintf(file, "lt");
            break;
        }
        case T_lt_u:
        {
            fprintf(file, "lt_u");
            break;
        }
        case T_gt_s:
        {
            if(type == T_i64 || type == T_i32)
                fprintf(file, "gt_s");
            else if(type == T_f64)
                fprintf(file, "gt");
            break;
        }
        case T_gt_u:
        {
            fprintf(file, "gt_u");
            break;
        }
        case T_le_s:
        {
            if(type == T_i64 || type == T_i32)
                fprintf(file, "le_s");
            else if(type == T_f64)
                fprintf(file, "le");
            break;
        }
        case T_ge_s:
        {
            if(type == T_i64 || type == T_i32)
                fprintf(file, "ge_s");
            else if(type == T_f64)
                fprintf(file, "ge");
            break;
        }
        case T_ge_u:
        {
            fprintf(file, "ge_u");
            break;
        }
        case T_and:
        {
            fprintf(file, "and");
            break;
        }
        case T_or:
        {
            fprintf(file, "or");
            break;
        }
    }
}
void Pr_printUniOp(T_uniOp oper, FILE *file)
{
    switch(oper)
    {
        case T_abs:
        {
            fprintf(file, "abs");
            break;
        }
        case T_neg:
        {
            fprintf(file, "neg");
            break;
        }
        case T_ceil:
        {
            fprintf(file, "ceil");
            break;
        }
        case T_floor:
        {
            fprintf(file, "floor");
            break;
        }
        case T_trunc:
        {
            fprintf(file, "trunc");
            break;
        }
        case T_nearest:
        {
            fprintf(file, "nearest");
            break;
        }
        case T_sqrt:
        {
            fprintf(file, "sqrt");
            break;
        }
    }
}
void Pr_printAExp(A_exp exp, FILE *file)
{
    if(!exp)
        return;
    switch(exp -> kind)
    {
        case A_intExp:
        {
            // printf("%lld", exp -> u.intt);
            fprintf(file, "%lld", exp -> u.intt);
            break;
        }
        case A_realExp:
        {
            fprintf(file, "%.15f", exp -> u.real);
            break;
        }
        case A_boolExp:
        {
            fprintf(file, "%d", exp -> u.booll);
            break;
        }
    }
}
void Pr_printModPrototype(T_module module, FILE *file)
{
    if(!module)
        return;
    switch(module -> kind)
    {
        case T_func:
        {
            fprintf(file, "(func ");
            T_typeList list = module -> u.func -> params;
            for(;list -> tail; list = list -> tail)
            {
                fprintf(file, "(param ");
                Pr_printType(list -> head, file);
                fprintf(file, ")");
            }
            if(module ->u.func -> result != T_none)
            {
                fprintf(file, "(result ");
                Pr_printType(module -> u.func -> result, file);
                fprintf(file, ")");
            }
            fprintf(file, ")");
            break;
        }
        case T_mem:
        {
            fprintf(file, "(memory %d)", module -> u.memory.pageSize);
            break;
        }
    }
}