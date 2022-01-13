#pragma once
#include <nlohmann/json.hpp>
#include <string>
#include <cstring>
#include <memory>
#include <deque>
#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>
#include <vector>
#include <locale>
#include <codecvt>
#include "absyn.h"
#include "ppsemant.hpp"
using json = nlohmann::json;

using L_token = std::shared_ptr<struct L_token_>;
using L_tokenList = std::deque<L_token>;
using delimDataTy = std::vector<std::pair<std::string, std::string>>;
// typedef struct L_token_ *L_token;

union L_tokenVal
{
    void *none;
    char charr[3];
    char* id;
    char* stringg;
    long long intt;
    double real;
    bool booll;
    A_oper oper;
    A_var var;
    A_exp exp;
    S_symbol sym;
    S_symbolList symlist;
    A_expList expList;
    A_stm stm;
    A_stmList stmList;
    A_ty type;
    A_dec dec;
    A_fundec funcdec;
    A_field field;
    A_fieldList fieldList;
    A_decList decList;
    A_classMemberList memList;

    L_tokenVal(){
        none = nullptr;
        id = nullptr;
        stringg = nullptr;
    };
};
struct L_token_
{
    int start;
    int end;
    std::string kind;
    delimDataTy delimData;
    L_token_(const L_token_&) = delete;
    L_token_& operator=(const L_token_&) = delete;
    L_token_() = default;
    L_tokenVal u;
};

struct L_errorPos
{
    std::string fileName = "";
    int fileNum = 0;
    int lineNum = 0;
    int columnNum = 0;
};

L_token L_delimToken(delimDataTy data);

L_tokenList L_Lexer(const char* filename1, const char *filename2, std::string mode);
std::vector<std::pair<std::string, std::wregex>> L_genTokenNames(const std::string &inputFname);
L_errorPos L_getErrorPos(int cursorPos);
std::map<std::string, unsigned> L_getOperators();
std::deque<std::string> L_getTokenNames();
std::map<std::string, unsigned> L_getKeywords();