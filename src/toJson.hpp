#pragma once
#include "env.hpp"
#ifdef __cplusplus
extern "C" {
#endif

typedef json_object *jobj;

jobj JS_toJson(A_decList dl);

jobj JS_StmToJson(A_stm stm);
jobj JS_ExpToJson(A_exp exp);
jobj JS_VarToJson(A_var var);
jobj JS_DecToJson(A_dec dec);
jobj JS_TyToJson(A_ty ty);
jobj JS_FieldToJson(A_field field);
jobj JS_ClassMemberToJson(A_classMember member);
jobj JS_IfelseToJson(A_ifelse ifelse);

jobj JS_StmListToJson(A_stmList list);
jobj JS_ExpListToJson(A_expList list);
jobj JS_DecListToJson(A_decList list);
jobj JS_FieldListToJson(A_fieldList fieldList);
jobj JS_ClassMemberListToJson(A_classMemberList memberList);
jobj JS_MemsToJson(A_classMemberList list);
jobj JS_IfelseListToJson(A_ifelseList list);
#ifdef __cplusplus
}
#endif