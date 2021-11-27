#include "converter.hpp"
#include <fstream>
#include <sstream>
#include <iostream>

static std::string symbols[] = {",",":",";","(",")","[","]","{","}",".","->","<-","=>","+","-","*","/","==","!=","<","<=",">",">=","&&","||","=", "%","&"};
static std::string convertOperToString(int oper);

void CON_convert(std::string ast, std::string targetLang, std::string fname){
    std::ifstream ASTinput(ast);
    json inputAST;
    ASTinput >> inputAST;
    std::ifstream targetLangRules(targetLang);
    json rules;
    targetLangRules >> rules;
    json keywords = rules["tokens"]["keywords"];
    json grammar = rules["grammar"];
    std::stringstream outputStream;
    std::ofstream output(fname);
    output << jsonToString(inputAST, "declist", grammar, keywords);
    output.close();
}

std::string jsonToString(json target, std::string parentRule, const json &rule, const json &keywords){
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

        std::stringstream ruleStringStream(ruleString);
        std::stringstream outputStream;
        std::string token;
        while(ruleStringStream >> token){
            if(keywords.find(token) != keywords.end()){
                if(token == "string"){
                    outputStream << "\"" << keywords[token].get<std::string>() << "\"";
                }
                else{
                    outputStream << keywords[token].get<std::string>();
                }
            }
            else if(info.find(token) != info.end()){
                if(token.substr(0, 6) == "string"){
                    outputStream << "\"" << jsonToString(info[token], token, rule, keywords) << "\"";
                }
                else if(token == "oper"){
                    outputStream << " " << convertOperToString(info[token]) << " ";
                }
                else{
                    outputStream << jsonToString(info[token], token, rule, keywords);
                }
            }
            else if(std::find(std::begin(symbols), std::end(symbols), token) != std::end(symbols)){
                if(token == "}"){
                    outputStream.seekp(-1, outputStream.cur);
                }
                else if(token == "="){
                    outputStream << " ";
                }
                else if(token == "=>"){
                    outputStream << " ";
                }
                outputStream << token;
                if(token == "{"){
                    outputStream << "\n";
                    indentTabs += 1;
                    for(int i = 0; i < indentTabs; i++){
                        outputStream << "\t";
                    }
                }
                else if(token == ";"){
                    outputStream << "\n";
                    for(int i = 0; i < indentTabs; i++){
                        outputStream << "\t";
                    }
                }
                else if(token == "}"){
                    indentTabs -= 1;
                    outputStream << "\n";
                    for(int i = 0; i < indentTabs; i++){
                        outputStream << "\t";
                    }
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
        }
        // std::cout << outputStream.str() << std::endl << std::endl;
        return outputStream.str();
    }
    else if(target.is_string()){
        return target.get<std::string>();
    }
    else if(target.is_array()){
        std::stringstream outputStream;
        for(const auto &element: target){
            if(element != target.front() && ((element["type"].get<std::string>() == "exp") || (element["type"].get<std::string>() == "field"))){
                outputStream << ", ";
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
            outputStream << jsonToString(element, type, rule, keywords);
        }
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

static std::string convertOperToString(int oper){
    switch(oper){
        case A_plusOp:
            return "+";
            break;
        case A_minusOp:
            return "-";
            break;
        case A_timesOp:
            return "*";
            break;
        case A_divideOp:
            return "/";
            break;
        case A_modOp:
            return "%";
            break;
        case A_eqOp:
            return "==";
            break;
        case A_neqOp:
            return "!=";
            break;
        case A_ltOp:
            return "<";
            break;
        case A_leOp:
            return "<=";
            break;
        case A_gtOp:
            return ">";
            break;
        case A_geOp:
            return ">=";
            break;
        case A_andOp:
            return "&&";
            break;
        case A_orOp:
            return "||";
            break;
    }
}