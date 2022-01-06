#include "converter.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>

extern std::string _mainFuncName;

static std::string symbols[] = {",",":",";","(",")","[","]","{","}",".","->","<-","=>","+","-","*","/","==","!=","<","<=",">",">=","&&","||","=", "%","&"};
static std::string convertOperToString(int oper, const json &rule);

void CON_convert(std::string ast, std::string targetLang, std::string fname){
    std::ifstream ASTinput(ast);
    json inputAST;
    ASTinput >> inputAST;
    std::ifstream targetLangRules(targetLang);
    json rules;
    targetLangRules >> rules;
    json config = rules["config"];
    json tokens = rules["tokens"];
    json grammar = rules["grammar"];
    std::cout << config["type"].get<std::string>() << std::endl;
    std::stringstream outputStream;
    std::ofstream output(fname);
    output << jsonToString(inputAST, "declist", grammar, tokens, config);
    output.close();

}

std::string jsonToString(json target, std::string parentRule, const json &rule, const json &tokens, const json &config){
    static unsigned int indentTabs;
    if(target.is_object()){
        std::string type = target["type"].get<std::string>();
        std::string kind;
        if(target["kind"].is_string()){
            kind = target["kind"].get<std::string>();
        }
        else{
            kind = "noinit";
        }
        json info  = target["info"];
        std::string ruleString;
        // std::cout << type + "/" + kind << std::endl;
        if(type == "ty" && kind == "array"){
            exit(0);
        }
        if(type == "ty" && kind == "name"){
            std::string typeName = info["id"];
            if(typeName == "void") kind = "void";
            else if(typeName == "int") kind = "int";
            else if(typeName == "real") kind = "real";
            else if(typeName == "bool") kind = "bool";
            else if(typeName == "char") kind = "char";
            else if(typeName == "short") kind = "short";
        }
        std::string ruleName = "";
        if(rule[type][kind].is_string()){
            ruleString = rule[type][kind].get<std::string>();
            ruleName = type + "." + kind;
        }
        else if(rule[type][kind].is_object()){
            // std::cout << type << kind << std::endl;
            std::string specificType = info["specificType"].get<std::string>();
            ruleString = rule[type][kind][specificType].get<std::string>();
            ruleName = type + "." + kind + "." + specificType;
            // std::cerr << "!!!!!is not rule!!!!!" << std::endl;
        }
        const json &keywords = tokens["keywords"];
        std::stringstream ruleStringStream(ruleString);
        std::stringstream outputStream;
        std::string token;
        while(ruleStringStream >> token){
            // std::cout << token << std::endl;
            if(keywords.find(token) != keywords.end()){
                if(token == "string"){
                    outputStream << "\"" << keywords[token].get<std::string>() << "\"";
                }
                else if(token == tokens["stmliststart"].get<std::string>()){
                    outputStream << token;
                    indentTabs += 1;
                    outputStream << "\n";
                }
                else if(token == tokens["stmlistend"].get<std::string>()){
                    indentTabs -= 1;
                    // outputStream.seekp(-1, outputStream.cur);
                    for(int i = 0; i < indentTabs; i++){
                        outputStream << "\t";
                    }
                    outputStream << token;
                    outputStream << "\n";
                }
                else{
                    outputStream << keywords[token].get<std::string>();
                    if(config["type"].get<std::string>() == "natural"){
                        outputStream << " ";
                    }
                }
            }
            else if(info.find(token) != info.end()){
                if(token.substr(0, 6) == "string"){
                    outputStream << "\"" << jsonToString(info[token], type, rule, tokens, config) << "\"";
                }
                else if(token == "char"){
                    outputStream << "\'" << jsonToString(info[token], type, rule, tokens, config) << "\'";
                }
                else if(token == "oper"){
                    outputStream << " " << convertOperToString(info[token], rule) << " ";
                }
                // else if(token == "int" || token == "real"){
                //     outputStream << jsonToString(info[token], type, rule, tokens, config);
                //     if(config["type"].get<std::string>() == "natural"){
                //         outputStream << " ";
                //     }
                // }
                else if(token == "int"){
                    outputStream << std::setprecision(18) << std::noshowpoint << info[token].get<int>();
                    if(config["type"].get<std::string>() == "natural"){
                        outputStream << " ";
                    }
                }
                else if(token == "real"){
                    outputStream << std::setprecision(18) << std::noshowpoint << info[token].get<double>();
                    float f = 0.0;
                    if(std::modf(info[token].get<double>(), &f) == 0.0){
                        outputStream << ".0";
                    }
                    if(config["type"].get<std::string>() == "natural"){
                        outputStream << " ";
                    }
                }
                else if(token == "id"){
                    std::string id = jsonToString(info[token], type, rule, tokens, config);
                    if(ruleName == "dec.func"){
                        if(id == _mainFuncName){
                            id = tokens["main"];
                        }
                    }
                    outputStream << id;
                    if(config["type"].get<std::string>() == "natural"){
                        outputStream << " ";
                    }
                }
                else{
                    outputStream << jsonToString(info[token], type, rule, tokens, config);
                }
            }
            else if(std::find(std::begin(symbols), std::end(symbols), token) != std::end(symbols)){
                if(token == "}"){
                    indentTabs -= 1;
                    // outputStream.seekp(-1, outputStream.cur);
                    for(int i = 0; i < indentTabs; i++){
                        outputStream << "\t";
                    }
                }
                if(config["type"].get<std::string>() == "programming"){
                    if(token == "="){
                        outputStream << " ";
                    }
                    else if(token == "=>"){
                        outputStream << " ";
                    }
                }
                outputStream << token;
                if(config["type"].get<std::string>() == "programming"){
                    if(token == "{"){
                        outputStream << "\n";
                        indentTabs += 1;
                    }
                    else if(token == "="){
                        outputStream << " ";
                    }
                    else if(token == ":"){
                        outputStream << " ";
                    }
                    else if(token == ","){
                        outputStream << " ";
                    }
                    else if(token == "=>"){
                        outputStream << " ";
                    }
                }
                else if(config["type"].get<std::string>() == "natural"){
                    if(token == "{"){
                        outputStream << "\n";
                        indentTabs += 1;
                    }
                    else{
                        outputStream << " ";
                    }
                }
            }
        }
        return outputStream.str();
    }
    else if(target.is_string()){
        return target.get<std::string>();
    }
    else if(target.is_array()){
        std::stringstream outputStream;
        int i = 0;
        std::string output = "";
        for(const auto &element: target){
            std::string eachOutput = "";
            if(i != 0 && ((element["type"].get<std::string>() == "exp") || (element["type"].get<std::string>() == "field"))){
                eachOutput += ", ";
            }
            std::string ruleName;
            std::string type = element["type"].get<std::string>();
            std::string kind;
            if(element["kind"].is_string()){
                kind = element["kind"].get<std::string>();
            }
            else{
                kind = "noinit";
            }

            if(rule[type][kind].is_string()){
                ruleName = type + "." + kind;
            }
            else if(rule[type][kind].is_object()){
                std::string specificType = element["info"]["specificType"].get<std::string>();
                ruleName = type + "." + kind + "." + specificType;
            }
            if(type == "dec" || type == "stm"){
                for(int i = 0; i < indentTabs; i++){
                    eachOutput += "\t";
                }
            }
            eachOutput += jsonToString(element, type, rule, tokens, config);
            if(type == "dec" || type == "stm"){
                eachOutput += "\n";
            }
            i += 1;
            if(parentRule == "mems"){
                output = eachOutput + output;
            }
            else{
                output += eachOutput;
            }

        }
        outputStream << output;
        return outputStream.str();
    }
    else if(target.is_number_integer()){
        return std::to_string(target.get<int>());
    }
    else if(target.is_number_float()){
        return std::to_string(target.get<double>());
    }
    std::cerr << "what is this json object." << std::endl;
}

static std::string convertOperToString(int oper, const json &rule){
    switch(oper){
        case A_plusOp:
            return rule["oper"]["add"].get<std::string>();
            break;
        case A_minusOp:
            return rule["oper"]["sub"].get<std::string>();
            break;
        case A_timesOp:
            return rule["oper"]["mul"].get<std::string>();
            break;
        case A_divideOp:
            return rule["oper"]["div"].get<std::string>();
            break;
        case A_modOp:
            return rule["oper"]["mod"].get<std::string>();
            break;
        case A_eqOp:
            return rule["oper"]["eq"].get<std::string>();
            break;
        case A_neqOp:
            return rule["oper"]["neq"].get<std::string>();
            break;
        case A_ltOp:
            return rule["oper"]["lt"].get<std::string>();
            break;
        case A_leOp:
            return rule["oper"]["le"].get<std::string>();
            break;
        case A_gtOp:
            return rule["oper"]["gt"].get<std::string>();
            break;
        case A_geOp:
            return rule["oper"]["ge"].get<std::string>();
            break;
        case A_andOp:
            return rule["oper"]["and"].get<std::string>();
            break;
        case A_orOp:
            return rule["oper"]["or"].get<std::string>();
            break;
        case A_assignOp:
            return "=";
            break;
    }
}