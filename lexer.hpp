#pragma once
#include <nlohmann/json.hpp>
#include <string>
#include <cstring>
#include <memory>
#include <deque>
#include "absyn.h"
#include "ppsemant.hpp"
using json = nlohmann::json;

using L_token = std::shared_ptr<struct L_token_>;
using L_tokenList = std::deque<L_token>;
using delimDataTy = std::vector<std::pair<std::string, std::string>>;
// typedef struct L_token_ *L_token;

enum tokenType
{
    CHAR,
    ID,
    STRING,
    INT,
    REAL,
    COMMA,
    COLON,
    SEMICOLON,
    LPAREN,
    RPAREN,
    LBRACK,
    RBRACK,
    LBRACE,
    RBRACE,
    DOT,
    PLUS,
    MINUS,
    TIMES,
    DIVIDE,
    EQ,
    NEQ,
    LT,
    LE,
    GT,
    GE,
    AND,
    OR,
    ASSIGN,
    IF,
    THEN,
    ELSE,
    FROM,
    TO,
    BREAK,
    INTTYPE,
    REALTYPE,
    CONTINUE,
    RETURN,
    TYPE,
    VOID,
    NUL,
    TRUEE,
    FALSEE,
    BOOLEAN,
    CHARTYPE,
    MOD,
    AMPERSAND,
    SHORTTYPE,
    FUNCTION,
    ARROW,
    LOOP,
    JSLOAD,
    SIZEOF,
    CLASS,
    LEFTARROW,
    PRIVATE,
    PUBLIC,
    PROTECTED,
    RIGHTARROW,
    REPEAT,
    JSEXPORT,
    UMINUS,
    LOWER_THAN_ELSE 
};
union L_tokenVal
{
    char charr[3];
    char* id;
    char* stringg;
    long long intt;
    double real;
    bool booll;
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
    A_fieldList fieldList;
    A_decList decList;
    A_classMemberList memList;

    L_tokenVal(){
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

typedef struct L_errorPos
{
    int lineNum;
    int columnNum;
};

L_token L_delimToken(delimDataTy data);

L_tokenList L_Lexer(const char* filename1, const char *filename2);
L_errorPos L_getErrorPos(int cursorPos);