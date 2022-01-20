#include "toJson.hpp"

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
            json_object_object_add(info, "stmlist", list);
            break;
        }
        case A_assignStm:
        {
            name = json_object_new_string("assign");
            jobj var = JS_VarToJson(stm -> u.assign.var);
            json_object_object_add(info, "var", var);
            jobj exp = JS_ExpToJson(stm -> u.assign.exp);
            json_object_object_add(info, "exp", exp);
            jobj specificType = json_object_new_string("normal");
            json_object_object_add(info, "specificType", specificType);
            break;
        }
        case A_declarationStm:
        {
            name = json_object_new_string("dec");
            jobj dec = JS_DecToJson(stm -> u.declaration.dec);
            json_object_object_add(info, "dec(stm)", dec);
            break;
        }
        case A_ifStm:
        {
            name = json_object_new_string("if");
            jobj test = JS_ExpToJson(stm -> u.iff.test);
            json_object_object_add(info, "exp", test);
            jobj then = JS_StmToJson(stm -> u.iff.then);
            json_object_object_add(info, "stm(then)", then);
            jobj specificType = json_object_new_string("if");
            if(stm -> u.iff.elsee){
                specificType = json_object_new_string("ifelse");
                jobj elsee = JS_StmToJson(stm -> u.iff.elsee);
                json_object_object_add(info, "stm(else)", elsee);
            }
            json_object_object_add(info, "specificType", specificType);
            break;
        }
        case A_whileStm:
        {
            name = json_object_new_string("while");
            jobj test = JS_ExpToJson(stm -> u.whilee.test);
            json_object_object_add(info, "exp", test);
            jobj body = JS_StmToJson(stm -> u.whilee.body);
            json_object_object_add(info, "stm", body);
            break;
        }
        case A_forStm:
        {
            name = json_object_new_string("for");
            jobj assign = JS_StmToJson(stm -> u.forr.assign);
            json_object_object_add(info, "stm(init)", assign);
            jobj condition = JS_ExpToJson(stm -> u.forr.condition);
            json_object_object_add(info, "exp", condition);
            jobj increment = JS_StmToJson(stm -> u.forr.increment);
            json_object_object_add(info, "stm(incr)", increment);
            jobj body = JS_StmToJson(stm -> u.forr.body);
            json_object_object_add(info, "stm", body);
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
            json_object_object_add(info, "exp", func);
            jobj specificType = json_object_new_string("noexp");
            if(stm -> u.call.args -> head){
                jobj args = JS_ExpListToJson(stm -> u.call.args);
                json_object_object_add(info, "explist", args);
                specificType = json_object_new_string("normal");
            }
            json_object_object_add(info, "specificType", specificType);
            break;
        }
        case A_returnStm:
        {
            name = json_object_new_string("return");
            jobj specificType = json_object_new_string("noexp");
            if(stm -> u.returnn.ret){
                specificType = json_object_new_string("exp");
                jobj returnExp = JS_ExpToJson(stm -> u.returnn.ret);
                json_object_object_add(info, "exp", returnExp);
            }
            json_object_object_add(info, "specificType", specificType);
            break;
        }
        case A_loopStm:
        {
            name = json_object_new_string("loop");
            jobj body = JS_StmToJson(stm -> u.loop.body);
            json_object_object_add(info, "stm", body);
            break;
        }
        case A_repeatStm:
        {
            name = json_object_new_string("repeat");
            jobj body = JS_StmToJson(stm -> u.repeat.body);
            json_object_object_add(info, "stm", body);
            jobj count = JS_ExpToJson(stm -> u.repeat.count);
            json_object_object_add(info, "exp", count);
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
            json_object_object_add(info, "exp", func);
            jobj args = JS_ExpListToJson(exp -> u.call.args);
            json_object_object_add(info, "explist", args);
            break;
        }
        case A_opExp:
        {
            name = json_object_new_string("op");
            jobj left = JS_ExpToJson(exp -> u.op.left);
            json_object_object_add(info, "exp(1)", left);
            jobj oper = json_object_new_int(exp -> u.op.oper);
            json_object_object_add(info, "oper", oper);
            jobj right = JS_ExpToJson(exp -> u.op.right);
            json_object_object_add(info, "exp(2)", right);
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
            json_object_object_add(info, "explist", list);
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
            json_object_object_add(info, "explist", list);
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
            if(exp -> u.booll){
                name = json_object_new_string("true");
            }
            else {
                name = json_object_new_string("false");
            }
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
            json_object_object_add(info, "var", address);
            break;
        }
        case A_sizeofExp:
        {
            name = json_object_new_string("sizeof");
            jobj sizeOf = JS_VarToJson(exp -> u.sizeOf);
            json_object_object_add(info, "var", sizeOf);
            break;
        }
        case A_subscriptExp:
        {
            name = json_object_new_string("subscript");
            jobj array = JS_ExpToJson(exp -> u.subscript.array);
            json_object_object_add(info, "exp(arrayname)", array);
            jobj index = JS_ExpToJson(exp -> u.subscript.index);
            json_object_object_add(info, "exp(index)", index);
            break;
        }
        case A_fieldExp:
        {
            name = json_object_new_string("field");
            jobj field = JS_ExpToJson(exp -> u.field.field);
            json_object_object_add(info, "exp", field);
            jobj member = json_object_new_string(S_name(exp -> u.field.member));
            json_object_object_add(info, "id", member);
            break;
        }
        case A_arrowFieldExp:
        {
            name = json_object_new_string("arrowfield");
            jobj pointer = JS_ExpToJson(exp -> u.arrowfield.pointer);
            json_object_object_add(info, "exp", pointer);
            jobj member = json_object_new_string(S_name(exp -> u.arrowfield.member));
            json_object_object_add(info, "id", member);
            break;
        }
        case A_funcExp:
        {
            name = json_object_new_string("func");
            jobj params = JS_FieldListToJson(exp -> u.func.params);
            json_object_object_add(info, "fieldlist(params)", params);
            jobj result = JS_FieldListToJson(exp -> u.func.result);
            json_object_object_add(info, "fieldlist(result)", result);
            jobj body = JS_StmToJson(exp -> u.func.body);
            json_object_object_add(info, "stm", body);
            break;
        }
        case A_parenExp:
        {
            name = json_object_new_string("paren");
            jobj paren = JS_ExpToJson(exp -> u.paren.paren);
            json_object_object_add(info, "exp", paren);
            break;
        }
        case A_typeEqExp:
        {
            name = json_object_new_string("typeeq");
            jobj ty1 = JS_TyToJson(exp -> u.typeeq.type1);
            json_object_object_add(info, "ty(1)", ty1);
            jobj ty2 = JS_TyToJson(exp -> u.typeeq.type2);
            json_object_object_add(info, "ty(2)", ty2);
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
            json_object_object_add(info, "id", simple);
            break;
        }
        case A_fieldVar:
        {
            name = json_object_new_string("field");
            jobj v = JS_VarToJson(var -> u.field.var);
            json_object_object_add(info, "var", v);
            jobj sym = json_object_new_string(S_name(var -> u.field.sym));
            json_object_object_add(info, "id", sym);
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
            json_object_object_add(info, "var", deref);
            break;
        }
        case A_arrowFieldVar:
        {
            name = json_object_new_string("arrowfield");
            jobj pointer = JS_VarToJson(var -> u.arrowfield.pointer);
            json_object_object_add(info, "var", pointer);
            jobj member = json_object_new_string(S_name(var -> u.arrowfield.member));
            json_object_object_add(info, "id", member);
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
            name = json_object_new_string("func");
            jobj funcName = json_object_new_string(S_name(dec -> u.function -> head -> name));
            json_object_object_add(info, "id", funcName);
            std::string specificTypeStr = "";
            if(dec -> u.function -> head -> params -> head){
                jobj params = JS_FieldListToJson(dec -> u.function -> head -> params);
                json_object_object_add(info, "fieldlist(params)", params);
            }
            else{
                specificTypeStr += "noparam";
            }
            if(dec -> u.function -> head -> result -> head){
                jobj result = JS_FieldListToJson(dec -> u.function -> head -> result);
                json_object_object_add(info, "fieldlist(result)", result);
            }
            else{
                specificTypeStr += "noresult";
            }
            jobj body = JS_StmToJson(dec -> u.function -> head -> body);
            json_object_object_add(info, "stm", body);
            if(specificTypeStr.size() == 0){
                specificTypeStr = "normal";
            }
            jobj specificType = json_object_new_string(specificTypeStr.c_str());
            json_object_object_add(info, "specificType", specificType);
            break;
        }
        case A_varDec:
        {
            name = json_object_new_string("var");
            jobj v = JS_VarToJson(dec -> u.var.var);
            json_object_object_add(info, "var", v);
            jobj typ = JS_TyToJson(dec -> u.var.typ);
            json_object_object_add(info, "ty", typ);
            jobj specificType = json_object_new_string("noinit");
            if(dec -> u.var.init){
                specificType = json_object_new_string("init");
                jobj init = JS_ExpToJson(dec -> u.var.init);
                json_object_object_add(info, "exp", init);
            }
            json_object_object_add(info, "specificType", specificType);
            break;
        }
        case A_objectDec:
        {
            name = json_object_new_string("object");
            jobj className = JS_TyToJson(dec -> u.object.className);
            json_object_object_add(info, "ty", className);
            jobj objectName = JS_VarToJson(A_SimpleVar(dec -> pos, dec -> u.object.name));
            json_object_object_add(info, "var", objectName);
            if(dec -> u.object.explist -> head){
                jobj expList = JS_ExpListToJson(dec -> u.object.explist);
                json_object_object_add(info, "explist", expList);
            }
            else{
                name = json_object_new_string("var");
                jobj specificType = json_object_new_string("noinit");
                json_object_object_add(info, "specificType", specificType);
            }
            break;
        }
        case A_funcImportDec:
        {
            name = json_object_new_string("jsload");
            jobj funcName = json_object_new_string(S_name(dec -> u.funcImport.name));
            json_object_object_add(info, "id", funcName);
            std::string specificTypeStr = "";
            if(dec -> u.funcImport.params -> head){
                jobj params = JS_FieldListToJson(dec -> u.funcImport.params);
                json_object_object_add(info, "fieldlist(params)", params);
            }
            else{
                specificTypeStr += "noparam";
            }
            if(dec -> u.funcImport.result -> head){
                jobj result = JS_FieldListToJson(dec -> u.funcImport.result);
                json_object_object_add(info, "fieldlist(result)", result);
            }
            else{
                specificTypeStr += "noresult";
            }
            jobj mod = json_object_new_string(dec -> u.funcImport.mod);
            json_object_object_add(info, "string(module)", mod);
            jobj func = json_object_new_string(dec -> u.funcImport.func);
            json_object_object_add(info, "string(func)", func);
            if(specificTypeStr.size() == 0){
                specificTypeStr = "normal";
            }
            json_object_object_add(info, "specificType", json_object_new_string(specificTypeStr.c_str()));
            break;
        }
        case A_funcExportDec:
        {
            name = json_object_new_string("jsexport");
            jobj funcName = json_object_new_string(S_name(dec -> u.funcExport.name));
            json_object_object_add(info, "id", funcName);
            jobj exportName = json_object_new_string(dec -> u.funcExport.exportName);
            json_object_object_add(info, "string", exportName);
            break;
        }
        case A_classDec:
        {
            name = json_object_new_string("class");
            jobj className = json_object_new_string(S_name(dec -> u.classs.name));
            json_object_object_add(info, "id(name)", className);
            jobj members = JS_ClassMemberListToJson(dec -> u.classs.members);
            json_object_object_add(info, "memlist", members);
            jobj specificType = json_object_new_string("noinherit");
            if(dec -> u.classs.inheritance){
                specificType = json_object_new_string("inherit");
                jobj inheritance = json_object_new_string(S_name(dec -> u.classs.inheritance -> head));
                json_object_object_add(info, "id(inherit)", inheritance);
            }
            json_object_object_add(info, "specificType", specificType);
            break;
        }
        case A_templateDec:
        {
            name = json_object_new_string("template");
            jobj d = JS_DecToJson(dec -> u.templatee.dec);
            json_object_object_add(info, "dec", d);
            jobj templateName = json_object_new_string(S_name(dec -> u.templatee.name));
            // printf("%s", S_name(dec -> u.templatee.name));
            json_object_object_add(info, "id", templateName);
            break;
        }
        case A_operatorDec:
        {
            name = json_object_new_string("operator");
            std::string specificTypeStr = "";
            if(dec -> u.op.params){
                jobj params = JS_FieldListToJson(dec -> u.op.params);
                json_object_object_add(info, "fieldlist(params)", params);
            }
            else{
                specificTypeStr += "noparam";
            }
            if(dec -> u.funcImport.result){
                jobj result = JS_FieldListToJson(dec -> u.funcImport.result);
                json_object_object_add(info, "fieldlist(result)", result);
            }
            else{
                specificTypeStr += "noresult";
            }
            if(specificTypeStr.size() == 0){
                specificTypeStr = "normal";
            }
            jobj stm = JS_StmToJson(dec -> u.op.body);
            json_object_object_add(info, "stm", stm);
            jobj oper = json_object_new_int(A_assignOp);
            jobj specificType = json_object_new_string("assign");
            oper = json_object_new_int(dec -> u.op.oper);
            if(dec -> u.op.oper == A_assignOp){
                oper = json_object_new_int(A_assignOp);
                specificTypeStr = "assign" + specificTypeStr;
            }
            json_object_object_add(info, "oper", oper);
            json_object_object_add(info, "specificType", json_object_new_string(specificTypeStr.c_str()));
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
            name = json_object_new_string("name");
            jobj tyName = json_object_new_string(S_name(ty -> u.name));
            json_object_object_add(info, "id", tyName);
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
            json_object_object_add(info, "int", size);
            break;
        }
        case A_pointerTy:
        {
            name = json_object_new_string("pointer");
            jobj pointer = JS_TyToJson(ty -> u.pointer);
            json_object_object_add(info, "ty", pointer);
            break;
        }
        case A_funcTy:
        {
            std::string specificTypeStr = "";
            if(ty -> u.func.params){
                jobj params = JS_FieldListToJson(ty -> u.func.params);
                json_object_object_add(info, "fieldlist(params)", params);
            }
            else{
                specificTypeStr += "noparam";
            }
            if(ty -> u.func.result){
                jobj result = JS_FieldListToJson(ty -> u.func.result);
                json_object_object_add(info, "fieldlist(result)", result);
            }
            else{
                specificTypeStr += "noresult";
            }
            if(specificTypeStr.size() == 0){
                specificTypeStr = "normal";
            }
            specificTypeStr = "func" + specificTypeStr;
            name = json_object_new_string(specificTypeStr.c_str());
            break;
        }
        case A_polyTy:
        {
            name = json_object_new_string("poly");
            jobj polyName = json_object_new_string(S_name(ty -> u.poly.name));
            json_object_object_add(info, "id", polyName);
            jobj typeParam = JS_TyToJson(ty -> u.poly.typeParam);
            json_object_object_add(info, "ty", typeParam);
            break;
        }
    }
    json_object_object_add(result, "kind", name);
    json_object_object_add(result, "info", info);
    return result;
}
jobj JS_FieldToJson(A_field field)
{
    if(!field)
        return NULL;
    jobj result = json_object_new_object();
    
    // jobj type = json_object_new_string("field");
    json_object_object_add(result, "type", json_object_new_string("field"));
    json_object_object_add(result, "kind", json_object_new_string("noinit"));
    
    jobj info = json_object_new_object();
    jobj var = JS_VarToJson(field -> var);
    jobj typ = JS_TyToJson(field -> typ);
    json_object_object_add(info, "var", var);
    json_object_object_add(info, "ty", typ);
    json_object_object_add(result, "info", info);
    return result;
}
jobj JS_ClassMemberToJson(A_classMember member)
{
    jobj result = json_object_new_object();
    // jobj type = json_object_new_string("member");
    // json_object_object_add(result, "type", type);

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
        if(stmList -> head){
            jobj stm = JS_StmToJson(stmList -> head);
            json_object_array_add(stmArray, stm);
        }
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
        if(expList -> head){
            jobj exp = JS_ExpToJson(expList -> head);
            json_object_array_add(expArray, exp);
        }
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
        if(decList -> head){
            jobj dec = JS_DecToJson(decList -> head);
            json_object_array_add(decArray, dec);
        }
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
        if(field){
            json_object_array_add(fieldArray, field);
        }
    }
    return fieldArray;
}

jobj JS_ClassMemberListToJson(A_classMemberList list)
{
    A_classMemberList classMemberList = list;
    jobj memsArray = json_object_new_array();
    if(!list)
        return memsArray;
    A_classMemberList memsTemp = NULL;
    A_classMemberSpecifier specifierBefore;
    jobj memsJson = NULL;
    for(specifierBefore = classMemberList -> head -> accessSpecifier, memsJson = json_object_new_array();classMemberList -> tail; classMemberList = classMemberList -> tail){
        if(classMemberList -> head){
            if(specifierBefore != classMemberList -> head -> accessSpecifier){
                jobj mems = json_object_new_object();
                jobj kind;
                switch(specifierBefore){
                    case A_public:{
                        kind = json_object_new_string("public");
                        break;
                    }
                    case A_private:{
                        kind = json_object_new_string("private");
                        break;
                    }
                    case A_protected:{
                        kind = json_object_new_string("protected");
                        break;
                    }
                    default:{
                        kind = json_object_new_string("public");
                        break;
                    }
                }
                jobj type = json_object_new_string("mems");
                json_object_object_add(mems, "type", type);
                json_object_object_add(mems, "kind", kind);
                // json_object_array_add(memsArray, memsJson);
                jobj info = json_object_new_object();
                json_object_object_add(info, "declist", memsJson);
                json_object_object_add(mems, "info", info);
                json_object_array_add(memsArray, mems);
                memsJson = json_object_new_array();
            }
            json_object_array_add(memsJson, JS_DecToJson(classMemberList -> head -> dec));
            specifierBefore = classMemberList -> head -> accessSpecifier;
        }
    }
    {
        jobj mems = json_object_new_object();
        jobj kind;
        switch(specifierBefore){
            case A_public:{
                kind = json_object_new_string("public");
                break;
            }
            case A_private:{
                kind = json_object_new_string("private");
                break;
            }
            case A_protected:{
                kind = json_object_new_string("protected");
                break;
            }
            default:{
                kind = json_object_new_string("public");
                break;
            }
        }
        jobj type = json_object_new_string("mems");
        json_object_object_add(mems, "type", type);
        json_object_object_add(mems, "kind", kind);
        // json_object_array_add(memsArray, memsJson);
        jobj info = json_object_new_object();
        json_object_object_add(info, "declist", memsJson);
        json_object_object_add(mems, "info", info);
        json_object_array_add(memsArray, mems);
    }
    return memsArray;
}