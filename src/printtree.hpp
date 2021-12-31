#pragma once
#include "semantic.hpp"
#ifdef __cplusplus
extern "C" {
#endif

void Pr_printTree(T_moduleList list, string fileName);//
void Pr_printStm(T_stm stm, FILE *file);//
void Pr_printExp(T_exp exp, FILE *file);//
void Pr_printFundec(T_fundec fundec, FILE *file);//
void Pr_printModule(T_module module, FILE *file);//
void Pr_printType(T_type type, FILE *file);//
void Pr_printExpList(T_expList list, FILE *file);//
void Pr_printBiOp(T_binOp oper, T_type type, FILE *file);//
void Pr_printUniOp(T_uniOp oper, FILE *file);//
void Pr_printAExp(A_exp exp, FILE *file);//
void Pr_printModPrototype(T_module module, FILE *file);
#ifdef __cplusplus
}
#endif