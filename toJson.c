#include "toJson.h"

jobj JS_toJson(A_decList dl)
{
    A_decList decList = dl;
    jobj decArray = json_object_new_array();
    for(;decList; decList = decList -> tail){
        jobj dec = JS_DecToJson(decList -> head);
        json_object_array_add(decArray, dec);
    }
    return decArray;
}

jobj JS_StmToJson(A_stm stm)
{
    jobj result = json_object_new_object();
    jobj type = json_object_new_string("stm");
    jobj name = NULL;
    jobj info = json_object_new_object();
    json_object_object_add(result, "type", type);
    if(!stm)
        return result;
    switch(stm -> kind){
        case A_compoundStm:
        {
            name = json_object_new_string("compound");
            jobj list = JS_StmListToJson(stm -> u.compound);
            json_object_object_add(info, "list", list);
            break;
        }
        case A_assignStm:
        {
            name = json_object_new_string("assign");
            jobj var = JS_VarToJson(stm -> u.assign.var);
            json_object_object_add(info, "var", var);
            jobj exp = JS_ExpToJson(stm -> u.assign.exp);
            json_object_object_add(info, "exp", exp);
            break;
        }
        case A_declarationStm:
        {
            name = json_object_new_string("declaration");
            jobj dec = JS_DecToJson(stm -> u.declaration.dec);
            json_object_object_add(info, "dec", dec);
            break;
        }
        case A_ifStm:
        {
            name = json_object_new_string("if");
            jobj test = JS_ExpToJson(stm -> u.iff.test);
            json_object_object_add(info, "test", test);
            jobj then = JS_StmToJson(stm -> u.iff.then);
            json_object_object_add(info, "then", then);
            jobj elsee = JS_StmToJson(stm -> u.iff.elsee);
            json_object_object_add(info, "else", elsee);
            break;
        }
        case A_whileStm:
        {
            name = json_object_new_string("while");
            jobj test = JS_ExpToJson(stm -> u.whilee.test);
            json_object_object_add(info, "test", test);
            jobj body = JS_StmToJson(stm -> u.whilee.body);
            json_object_object_add(info, "body", body);
            break;
        }
        case A_forStm:
        {
            name = json_object_new_string("for");
            jobj assign = JS_StmToJson(stm -> u.forr.assign);
            json_object_object_add(info, "assign", assign);
            jobj condition = JS_ExpToJson(stm -> u.forr.condition);
            json_object_object_add(info, "condition", condition);
            jobj increment = JS_StmToJson(stm -> u.forr.increment);
            json_object_object_add(info, "increment", increment);
            jobj body = JS_StmToJson(stm -> u.forr.body);
            json_object_object_add(info, "body", body);
            break;
        }
        case A_breakStm:
        {
            name = json_object_new_string("break");
            break;
        }
        case A_continueStm:
        {
            name = json_object_new_string("continue");
            break;
        }
        case A_callStm:
        {
            name = json_object_new_string("call");
            jobj func = JS_ExpToJson(stm -> u.call.func);
            json_object_object_add(info, "func", func);
            jobj args = JS_ExpListToJson(stm -> u.call.args);
            json_object_object_add(info, "args", args);
            break;
        }
        case A_returnStm:
        {
            name = json_object_new_string("return");
            break;
        }
        case A_loopStm:
        {
            name = json_object_new_string("loop");
            jobj body = JS_StmToJson(stm -> u.loop.body);
            json_object_object_add(info, "body", body);
            break;
        }
    } 
    json_object_object_add(result, "kind", name);
    json_object_object_add(result, "info", info);
    return result;
}
jobj JS_ExpToJson(A_exp exp)
{
    jobj result = json_object_new_object();
    jobj name = NULL;
    jobj info = json_object_new_object();
    jobj type = json_object_new_string("exp");
    json_object_object_add(result, "type", type);

    if(!exp)
        return result;
    switch(exp -> kind)
    {
        case A_varExp:
        {
            name = json_object_new_string("var");
            jobj var = JS_VarToJson(exp -> u.var);
            json_object_object_add(info, "var", var);
            break;
        }
        case A_nilExp:
        {
            name = json_object_new_string("nil");
            break;
        }
        case A_intExp:
        {
            name = json_object_new_string("int");
            jobj intt = json_object_new_int64(exp -> u.intt);
            json_object_object_add(info, "int", intt);
            break;
        }
        case A_stringExp:
        {
            name = json_object_new_string("string");
            jobj stringg = json_object_new_string(exp -> u.stringg);
            json_object_object_add(info, "string", stringg);
            break;
        }
        case A_callExp:
        {
            name = json_object_new_string("call");
            jobj func = JS_ExpToJson(exp -> u.call.func);
            json_object_object_add(info, "func", func);
            jobj args = JS_ExpListToJson(exp -> u.call.args);
            json_object_object_add(info, "args", args);
            break;
        }
        case A_opExp:
        {
            name = json_object_new_string("op");
            jobj left = JS_ExpToJson(exp -> u.op.left);
            json_object_object_add(info, "left", left);
            jobj oper = json_object_new_int(exp -> u.op.oper);
            json_object_object_add(info, "oper", oper);
            jobj right = JS_ExpToJson(exp -> u.op.right);
            json_object_object_add(info, "right", right);
            break;
        }
        //case A_recordExp:
        //{
        //    break;
        //}
        case A_seqExp:
        {
            name = json_object_new_string("seq");
            jobj list = JS_ExpListToJson(exp -> u.seq);
            json_object_object_add(info, "list", list);
            break;
        }
        case A_ifExp:
        {
            name = json_object_new_string("if");
            jobj test = JS_ExpToJson(exp -> u.iff.test);
            json_object_object_add(info, "test", test);
            jobj then = JS_ExpToJson(exp -> u.iff.then);
            json_object_object_add(info, "then", then);
            jobj elsee = JS_ExpToJson(exp -> u.iff.elsee);
            json_object_object_add(info, "elsee", elsee);
            break;
        }
        case A_arrayExp:
        {
            name = json_object_new_string("array");
            jobj list = JS_ExpListToJson(exp -> u.array.list);
            json_object_object_add(info, "list", list);
            break;
        }
        case A_realExp:
        {
            name = json_object_new_string("real");
            jobj real = json_object_new_double(exp -> u.real);
            json_object_object_add(info, "real", real);
            break;
        }
        case A_boolExp:
        {
            name = json_object_new_string("bool");
            jobj booll = json_object_new_boolean(exp -> u.booll);
            json_object_object_add(info, "bool", booll);
            break;
        }
        case A_charExp:
        {
            name = json_object_new_string("char");
            jobj charr = json_object_new_string(exp -> u.charr);
            json_object_object_add(info, "char", charr);
            break;
        }
        case A_derefExp:
        {
            name = json_object_new_string("deref");
            jobj var = JS_VarToJson(exp -> u.deref);
            json_object_object_add(info, "var", var);
            break;
        }
        case A_addressExp:
        {
            name = json_object_new_string("address");
            jobj address = JS_VarToJson(exp -> u.address);
            json_object_object_add(info, "address", address);
            break;
        }
        case A_sizeofExp:
        {
            name = json_object_new_string("sizeOf");
            jobj sizeOf = JS_VarToJson(exp -> u.sizeOf);
            json_object_object_add(info, "sizeOf", sizeOf);
            break;
        }
        case A_subscriptExp:
        {
            name = json_object_new_string("subscript");
            jobj array = JS_ExpToJson(exp -> u.subscript.array);
            json_object_object_add(info, "array", array);
            jobj index = JS_ExpToJson(exp -> u.subscript.index);
            json_object_object_add(info, "index", index);
            break;
        }
        case A_fieldExp:
        {
            name = json_object_new_string("field");
            jobj field = JS_ExpToJson(exp -> u.field.field);
            json_object_object_add(info, "field", field);
            jobj member = json_object_new_string(S_name(exp -> u.field.member));
            json_object_object_add(info, "member", member);
            break;
        }
        case A_arrowFieldExp:
        {
            name = json_object_new_string("arrowfield");
            jobj pointer = JS_ExpToJson(exp -> u.arrowfield.pointer);
            json_object_object_add(info, "pointer", pointer);
            jobj member = json_object_new_string(S_name(exp -> u.arrowfield.member));
            json_object_object_add(info, "member", member);
            break;
        }
        case A_funcExp:
        {
            name = json_object_new_string("func");
            jobj params = JS_FieldListToJson(exp -> u.func.params);
            json_object_object_add(info, "params", params);
            jobj result = JS_FieldListToJson(exp -> u.func.result);
            json_object_object_add(info, "result", result);
            jobj body = JS_StmToJson(exp -> u.func.body);
            json_object_object_add(info, "body", body);
            break;
        }
    }
    json_object_object_add(result, "kind", name);
    json_object_object_add(result, "info", info);
    return result;
}
jobj JS_VarToJson(A_var var)
{
    jobj result = json_object_new_object();
    jobj name = NULL;
    jobj info = json_object_new_object();
    jobj type = json_object_new_string("var");
    json_object_object_add(result, "type", type);

    if(!var)
        return result;
    switch(var -> kind)
    {
        case A_simpleVar:
        {
            name = json_object_new_string("simple");
            jobj simple = json_object_new_string(S_name(var -> u.simple));
            json_object_object_add(info, "simple", simple);
            break;
        }
        case A_fieldVar:
        {
            name = json_object_new_string("field");
            jobj v = JS_VarToJson(var -> u.field.var);
            json_object_object_add(info, "var", v);
            jobj sym = json_object_new_string(S_name(var -> u.field.sym));
            json_object_object_add(info, "sym", sym);
            break;
        }
        case A_subscriptVar:
        {
            name = json_object_new_string("subscript");
            jobj v = JS_VarToJson(var -> u.subscript.var);
            json_object_object_add(info, "var", v);
            jobj exp = JS_ExpToJson(var -> u.subscript.exp);
            json_object_object_add(info, "exp", exp);
            break;
        }
        case A_derefVar:
        {
            name = json_object_new_string("deref");
            jobj deref = JS_VarToJson(var -> u.deref);
            json_object_object_add(info, "deref", deref);
            break;
        }
        case A_arrowFieldVar:
        {
            name = json_object_new_string("arrowfield");
            jobj pointer = JS_VarToJson(var -> u.arrowfield.pointer);
            json_object_object_add(info, "pointer", pointer);
            jobj member = json_object_new_string(S_name(var -> u.arrowfield.member));
            json_object_object_add(info, "member", member);
            break;
        }
    }
    json_object_object_add(result, "kind", name);
    json_object_object_add(result, "info", info);
    return result;
}
jobj JS_DecToJson(A_dec dec)
{
    jobj result = json_object_new_object();
    jobj name = NULL;
    jobj info = json_object_new_object();
    jobj type = json_object_new_string("dec");
    json_object_object_add(result, "type", type);

    if(!dec)
        return result;
    switch(dec -> kind)
    {
        case A_functionDec:
        {
            name = json_object_new_string("function");
            jobj funcName = json_object_new_string(S_name(dec -> u.function -> head -> name));
            json_object_object_add(info, "funcName", funcName);
            jobj params = JS_FieldListToJson(dec -> u.function -> head -> params);
            json_object_object_add(info, "params", params);
            jobj result = JS_FieldListToJson(dec -> u.function -> head -> result);
            json_object_object_add(info, "result", result);
            jobj body = JS_StmToJson(dec -> u.function -> head -> body);
            json_object_object_add(info, "body", body);
            break;
        }
        case A_varDec:
        {
            name = json_object_new_string("var");
            jobj v = JS_VarToJson(dec -> u.var.var);
            json_object_object_add(info, "var", v);
            jobj init = JS_ExpToJson(dec -> u.var.init);
            json_object_object_add(info, "init", init);
            break;
        }
        case A_objectDec:
        {
            name = json_object_new_string("object");
            jobj className = JS_TyToJson(dec -> u.object.className);
            json_object_object_add(info, "className", className);
            jobj objectName = json_object_new_string(S_name(dec -> u.object.name));
            json_object_object_add(info, "objectName", objectName);
            jobj expList = JS_ExpListToJson(dec -> u.object.explist);
            json_object_object_add(info, "explist", expList);
            break;
        }
        case A_funcImportDec:
        {
            name = json_object_new_string("funcImport");
            jobj funcName = json_object_new_string(S_name(dec -> u.funcImport.name));
            json_object_object_add(info, "funcName", funcName);
            jobj params = JS_FieldListToJson(dec -> u.funcImport.params);
            json_object_object_add(info, "params", params);
            jobj result = JS_FieldListToJson(dec -> u.funcImport.result);
            json_object_object_add(info, "result", result);
            jobj mod = json_object_new_string(dec -> u.funcImport.mod);
            json_object_object_add(info, "mod", mod);
            jobj func = json_object_new_string(dec -> u.funcImport.func);
            json_object_object_add(info, "func", func);
            break;
        }
        case A_funcExportDec:
        {
            name = json_object_new_string("funcExport");
            jobj funcName = json_object_new_string(S_name(dec -> u.funcExport.name));
            json_object_object_add(info, "funcName", funcName);
            jobj exportName = json_object_new_string(dec -> u.funcExport.exportName);
            json_object_object_add(info, "exportName", exportName);
            break;
        }
        case A_classDec:
        {
            name = json_object_new_string("class");
            jobj className = json_object_new_string(S_name(dec -> u.class.name));
            json_object_object_add(info, "className", className);
            jobj members = JS_ClassMemberListToJson(dec -> u.class.members);
            json_object_object_add(info, "members", members);
            if(dec -> u.class.inheritance){
                jobj inheritance = json_object_new_string(S_name(dec -> u.class.inheritance -> head));
                json_object_object_add(info, "inheritance", inheritance);
            }
            break;
        }
        case A_templateDec:
        {
            name = json_object_new_string("template");
            jobj d = JS_DecToJson(dec -> u.template.dec);
            json_object_object_add(info, "dec", d);
            jobj templateName = json_object_new_string(S_name(dec -> u.template.name));
            json_object_object_add(info, "templateName", templateName);
            break;
        }
    }
    json_object_object_add(result, "kind", name);
    json_object_object_add(result, "info", info);
    return result;
}
jobj JS_TyToJson(A_ty ty)
{
    jobj result = json_object_new_object();
    jobj name = NULL;
    jobj info = json_object_new_object();
    jobj type = json_object_new_string("ty");
    json_object_object_add(result, "type", type);

    if(!ty)
        return result;
    switch(ty -> kind)
    {
        case A_nameTy:
        {
            name = json_object_new_string("kind");
            jobj tyName = json_object_new_string(S_name(ty -> u.name));
            json_object_object_add(info, "tyName", tyName);
            break;
        }
        //case A_recordTy:
        //{
        //    
        //    
        //    break;
        //}
        case A_arrayTy:
        {
            name = json_object_new_string("array");
            jobj type = JS_TyToJson(ty -> u.array.type);
            json_object_object_add(info, "type", type);
            jobj size = json_object_new_int(ty -> u.array.size);
            json_object_object_add(info, "size", size);
            break;
        }
        case A_pointerTy:
        {
            name = json_object_new_string("pointer");
            jobj pointer = JS_TyToJson(ty -> u.pointer);
            json_object_object_add(info, "pointer", pointer);
            break;
        }
        case A_funcTy:
        {
            name = json_object_new_string("func");
            jobj params = JS_FieldListToJson(ty -> u.func.params);
            json_object_object_add(info, "params", params);
            jobj result = JS_FieldListToJson(ty -> u.func.result);
            json_object_object_add(info, "result", result);
            break;
        }
        case A_polyTy:
        {
            name = json_object_new_string("poly");
            jobj polyName = json_object_new_string(S_name(ty -> u.poly.name));
            json_object_object_add(info, "polyName", polyName);
            jobj typeParam = JS_TyToJson(ty -> u.poly.typeParam);
            json_object_object_add(info, "typeParam", typeParam);
            break;
        }
    }
    json_object_object_add(result, "kind", name);
    json_object_object_add(result, "info", info);
    return result;
}
jobj JS_FieldToJson(A_field field)
{
    jobj result = json_object_new_object();
    jobj type = json_object_new_string("field");
    json_object_object_add(result, "type", type);

    if(!field)
        return result;
    jobj name = json_object_new_string(S_name(field -> name));
    json_object_object_add(result, "kind", name);
    jobj typ = JS_TyToJson(field -> typ);
    json_object_object_add(result, "typ", typ);
    return result;
}
jobj JS_ClassMemberToJson(A_classMember member)
{
    jobj result = json_object_new_object();
    jobj type = json_object_new_string("member");
    json_object_object_add(result, "type", type);

    if(!member)
        return result;
    jobj memberSpecifier = json_object_new_int(member -> accessSpecifier);
    json_object_object_add(result, "specifier", memberSpecifier);
    jobj dec = JS_DecToJson(member -> dec);
    json_object_object_add(result, "dec", dec);
    return result;
}

jobj JS_StmListToJson(A_stmList list)
{
    A_stmList stmList = list;
    jobj stmArray = json_object_new_array();
    if(!list)
        return stmArray;
    for(;stmList; stmList = stmList -> tail){
        jobj stm = JS_StmToJson(stmList -> head);
        json_object_array_add(stmArray, stm);
    }
    return stmArray;
}
jobj JS_ExpListToJson(A_expList list)
{
    A_expList expList = list;
    jobj expArray = json_object_new_array();
    if(!list)
        return expArray;
    for(;expList; expList = expList -> tail){
        jobj exp = JS_ExpToJson(expList -> head);
        json_object_array_add(expArray, exp);
    }
    return expArray;
}
jobj JS_DecListToJson(A_decList list)
{
    jobj decArray = json_object_new_array();
    if(!list)
        return decArray;
    A_decList decList = list;
    for(;decList; decList = decList -> tail){
        jobj dec = JS_DecToJson(decList -> head);
        json_object_array_add(decArray, dec);
    }
    return decArray;
}
jobj JS_FieldListToJson(A_fieldList list)
{
    A_fieldList fieldList = list;
    jobj fieldArray = json_object_new_array();
    if(!list){
        return fieldArray;
    }
    for(;fieldList; fieldList = fieldList -> tail){
        jobj field = JS_FieldToJson(fieldList -> head);
        json_object_array_add(fieldArray, field);
    }
    return fieldArray;
}
jobj JS_ClassMemberListToJson(A_classMemberList list)
{
    A_classMemberList classMemberList = list;
    jobj memberArray = json_object_new_array();
    if(!list)
        return memberArray;
    for(;classMemberList; classMemberList = classMemberList -> tail){
        jobj field = JS_ClassMemberToJson(classMemberList -> head);
        json_object_array_add(memberArray, field);
    }
    return memberArray;
}