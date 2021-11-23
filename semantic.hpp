#pragma once
#include "lexer.hpp"
#ifdef __cplusplus
extern "C" {
#endif
#include "env.hpp"
#include "tree.h"
#include "errormsg.hpp"

struct expty {Tr_exp exp; Ty_ty ty;};

struct expty expTy(Tr_exp exp, Ty_ty ty);

struct expty transStm(S_table venv, S_table tenv, A_stm s, Tr_level level, bool isLoop, Ty_ty classs);
struct expty transVar(S_table venv, S_table tenv, A_var v, Tr_level level, bool isLoop, bool reverse, bool isDec, Ty_ty classs);
struct expty transExp(S_table venv, S_table tenv, A_exp e, Tr_level level, bool isLoop, Ty_ty classs);
T_module     transDec(S_table venv, S_table tenv, A_dec d, Tr_level level, bool isLoop, S_table table, Ty_ty classs);
Ty_ty        transTy (S_table venv, S_table tenv, A_ty ty, Tr_level level);

T_moduleList SEM_transProg(A_decList declist);
#ifdef __cplusplus
}
#endif