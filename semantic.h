#pragma once
#include "env.h"
#include "tree.h"
#include "errormsg.h"

struct expty {Tr_exp exp; Ty_ty ty;};

struct expty expTy(Tr_exp exp, Ty_ty ty);

struct expty transStm(S_table venv, S_table tenv, A_stm s, Tr_level level, bool isLoop, Ty_ty class);
struct expty transVar(S_table venv, S_table tenv, A_var v, Tr_level level, bool isLoop, bool reverse, bool isDec, Ty_ty class);
struct expty transExp(S_table venv, S_table tenv, A_exp e, Tr_level level, bool isLoop, Ty_ty class);
T_module     transDec(S_table venv, S_table tenv, A_dec d, Tr_level level, bool isLoop, S_table table, Ty_ty class);
Ty_ty        transTy (S_table venv, S_table tenv, A_ty ty, Tr_level level);

T_moduleList SEM_transProg(A_decList declist);