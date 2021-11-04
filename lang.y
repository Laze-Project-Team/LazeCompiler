%{
    #ifdef __cplusplus
    extern "C" {
    #endif
    #include <stdio.h>
    #include "util.h"
    #include "errormsg.h"
    #include "symbol.h"
    #include "absyn.h"
    #include "y.tab.h"
    #define YYDEBUG 1

    int yylex(void);
    int yyparse(void);
    A_decList absyn_root;

    void yyerror(const char *s)
    {
        EM_error(EM_tokPos, "%s", s);
    }
    #ifdef __cplusplus
    }
    #endif
%}

%union
{
    int pos;
    int shortt;
    char cval[3];
    long long ival;
    double fval;
    string sval;
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
}

%token <cval> CHAR
%token <sval> ID STRING
%token <ival> INT
%token <fval> REAL

%type <sym> id
%type <var> lvalue
%type <exp> exp varExp funcExp
%type <expList> explist
%type <stm> stm if while assign funcCall for return loop
%type <stmList> stmlist
%type <dec> declare funcDec jsLoad classDec funcAndVar templateDec jsExport
/* %type <funcdec> funcDec */
%type <type> type
%type <fieldList> tyfield1 tyfield
%type <decList> decs funcvarlist
%type <memList> mems memlist

%token
    COMMA COLON SEMICOLON LPAREN RPAREN LBRACK RBRACK LBRACE RBRACE DOT PLUS MINUS TIMES DIVIDE EQ 
    NEQ LT LE GT GE AND OR ASSIGN IF THEN ELSE FROM TO BREAK INTTYPE
    REALTYPE CONTINUE RETURN TYPE VOID NUL TRUEE FALSEE BOOLEAN CHARTYPE 
    MOD AMPERSAND SHORTTYPE FUNCTION ARROW LOOP JSLOAD SIZEOF CLASS LEFTARROW PRIVATE
    PUBLIC PROTECTED RIGHTARROW REPEAT JSEXPORT
 
%left SEMICOLON

%nonassoc UMINUS
%nonassoc LOWER_THAN_ELSE
%left ELSE
%left PLUS MINUS
%left TIMES DIVIDE MOD
%left OR AND
%left ASSIGN
%nonassoc EQ NEQ GT GE LT LE

%start program

%%

program: decs {absyn_root = $1;}

decs :      funcAndVar {$$ = A_DecList($1, NULL);}
            | jsLoad {$$ = A_DecList($1, NULL);}
            | classDec {$$ = A_DecList($1, NULL);}
            | templateDec {$$ = A_DecList($1, NULL);}
            | jsExport {$$ = A_DecList($1, NULL);}
            | templateDec decs {$$ = A_DecList($1, $2);}
            | classDec decs {$$ = A_DecList($1, $2);}
            | jsLoad decs {$$ = A_DecList($1, $2);}
            | funcAndVar decs {$$ = A_DecList($1, $2);}
            | jsExport decs {$$ = A_DecList($1, $2);}

funcAndVar: declare {$$ = $1;}
            | funcDec {$$ = $1;}

exp :       INT {$$ = A_IntExp(EM_tokPos, $1);}
            | STRING { $$ = A_StringExp(EM_tokPos, $1);}
            | REAL { $$ = A_RealExp(EM_tokPos, $1);}
            | NUL {$$ = A_NilExp(EM_tokPos);}
            | CHAR {$$ = A_CharExp(EM_tokPos, $1);}
            | TRUEE {$$ = A_BoolExp(EM_tokPos, TRUE);}
            | FALSEE {$$ = A_BoolExp(EM_tokPos, FALSE);}
            /* | assignExp {$$ = $1;} */
            | exp PLUS exp {$$ = A_OpExp(EM_tokPos, A_plusOp, $1, $3);}
            | exp MINUS exp {$$ = A_OpExp(EM_tokPos, A_minusOp, $1, $3);}
            | exp TIMES exp {$$ = A_OpExp(EM_tokPos, A_timesOp, $1, $3);}
            | exp DIVIDE exp {$$ = A_OpExp(EM_tokPos, A_divideOp, $1, $3);}
            | exp MOD exp {$$ = A_OpExp(EM_tokPos, A_modOp, $1, $3);}
            // 4 shift/reduce conflictss
            | MINUS exp %prec UMINUS {$$ = A_OpExp(EM_tokPos, A_minusOp, A_IntExp(0, 0), $2);}
            | exp EQ exp {$$ = A_OpExp(EM_tokPos, A_eqOp, $1, $3);}
            | exp NEQ exp {$$ = A_OpExp(EM_tokPos, A_neqOp, $1, $3);}
            | exp GE exp {$$ = A_OpExp(EM_tokPos, A_geOp, $1, $3);}
            | exp GT exp {$$ = A_OpExp(EM_tokPos, A_gtOp, $1, $3);}
            | exp LE exp {$$ = A_OpExp(EM_tokPos, A_leOp, $1, $3);}
            | exp LT exp {$$ = A_OpExp(EM_tokPos, A_ltOp, $1, $3);}
            // 6 shift/reduce conflicts
            | varExp LPAREN explist RPAREN {$$ = A_CallExp(EM_tokPos, $1, $3);} 
            | LPAREN exp RPAREN {$$ = $2;}
            | exp AND exp {$$ = A_OpExp(EM_tokPos, A_andOp, $1, $3);}
            | exp OR exp {$$ = A_OpExp(EM_tokPos, A_orOp, $1, $3);}
            /* | TIMES lvalue {$$ = A_DerefExp(EM_tokPos, $2);} */
            | AMPERSAND lvalue {$$ = A_AddressExp(EM_tokPos, $2);}
            | SIZEOF LPAREN lvalue RPAREN {$$ = A_SizeofExp(EM_tokPos, $3);}
            | LBRACK explist RBRACK {$$ = A_ArrayExp(EM_tokPos, $2);}
            | varExp {$$ = $1;}
            | funcExp {$$ = $1;}

varExp:     lvalue { $$ = A_VarExp(EM_tokPos, $1); }
            | exp DOT id {$$ = A_FieldExp(EM_tokPos, $1, $3);}
            | exp LBRACK exp RBRACK {$$ = A_SubscriptExp(EM_tokPos, $1, $3);}
            | exp RIGHTARROW id {$$ = A_ArrowFieldExp(EM_tokPos, $1, $3);}

funcExp:    LPAREN tyfield RPAREN ARROW LPAREN tyfield RPAREN stm {$$ = A_FuncExp(EM_tokPos, $2, $6, $8);}

/* assignExp : lvalue ASSIGN exp { $$ = A_AssignExp(EM_tokPos, $1, $3);} */

stm :       funcDec {$$ = A_DeclarationStm(EM_tokPos, $1);}
            | funcCall SEMICOLON 
            | assign SEMICOLON {$$ = $1;}
            | declare {$$ = A_DeclarationStm(EM_tokPos, $1);}
            | if {$$ = $1;}
            | while {$$ = $1;}
            | for {$$ = $1;}
            | return SEMICOLON{$$ = $1;}
            | loop {$$ = A_DeclarationStm(EM_tokPos, A_FunctionDec(EM_tokPos, A_FundecList(A_Fundec(EM_tokPos, S_Symbol("loop"), A_FieldList(NULL, NULL), A_FieldList(NULL, NULL), $1 -> u.loop.body), NULL)));}
            | BREAK SEMICOLON {$$ = A_BreakStm(EM_tokPos);}
            | CONTINUE SEMICOLON {$$ = A_ContinueStm(EM_tokPos);}
            | LBRACE stmlist RBRACE {$$ = A_CompoundStm(EM_tokPos, $2);}

/* stmlist :            {$$ = NULL;}
            | stm stmlist {$$ = A_CompoundStm(EM_tokPos, $1, $2);} */
stmlist :   stm {$$ = A_StmList($1, NULL);}
        |   stm stmlist {$$ = A_StmList($1, $2); }

return :    RETURN {$$ = A_ReturnStm(EM_tokPos, NULL);}
            | RETURN LPAREN exp RPAREN {$$ = A_ReturnStm(EM_tokPos, $3);}

type :      id {$$ = A_NameTy(EM_tokPos, $1);}
            | VOID {$$ = A_NameTy(EM_tokPos, S_Symbol("void"));}
            | INTTYPE {$$ = A_NameTy(EM_tokPos, S_Symbol("int"));}
            | REALTYPE {$$ = A_NameTy(EM_tokPos, S_Symbol("real"));}
            | BOOLEAN {$$ = A_NameTy(EM_tokPos, S_Symbol("int"));}
            | CHARTYPE {$$ = A_NameTy(EM_tokPos, S_Symbol("char"));}
            | SHORTTYPE {$$ = A_NameTy(EM_tokPos, S_Symbol("short"));}
            | TIMES type {$$ = A_PointerTy(EM_tokPos, $2);}
            | id LT type GT {$$ = A_PolyTy(EM_tokPos, $1, $3);}

tyfield :   /* empty */ {$$ = A_FieldList(NULL, NULL);}
            | VOID {$$ = A_FieldList(NULL, NULL);}
            | tyfield1 {$$ = $1;}

tyfield1 :  type COLON lvalue {
                if($3 -> kind == A_simpleVar)
                {
                    // printf("%s fasdlkfjlaj;dfklja;dklf\n", S_name($3 -> u.simple));
                    $$ = A_FieldList(A_Field(EM_tokPos, $3 -> u.simple, $1), NULL);
                }
                else if($3 -> kind == A_subscriptVar)
                {
                    A_ty type = $1;
                    for(A_var var = $3; var -> kind == A_subscriptVar; var = var -> u.subscript.var)
                    {
                        if(var -> u.subscript.exp -> kind!= A_intExp)
                        {
                            EM_error(EM_tokPos, "Cannot declare array with an unfixed size.");
                        }
                        type = A_ArrayTy(EM_tokPos, type, var -> u.subscript.exp -> u.intt);
                    }
                    $$ = A_FieldList(A_Field(EM_tokPos, $3 -> u.subscript.name, type), NULL);
                }
                else
                {
                    EM_error(EM_tokPos, "Not supported.");
                }
            }   
            | FUNCTION COLON id LPAREN tyfield RPAREN ARROW LPAREN tyfield RPAREN {$$ = A_FieldList(A_Field(EM_tokPos, $3, A_FuncTy(EM_tokPos, $5, $9)), NULL);}
            | type COLON lvalue COMMA tyfield1 {
                if($3 -> kind == A_simpleVar)
                {
                    $$ = A_FieldList(A_Field(EM_tokPos, $3 -> u.simple, $1), $5);
                }
                else if($3 -> kind == A_subscriptVar)
                {
                    A_ty type = $1;
                    for(A_var var = $3; var -> kind == A_subscriptVar; var = var -> u.subscript.var)
                    {
                        if(var -> u.subscript.exp -> kind!= A_intExp)
                        {
                            EM_error(EM_tokPos, "Cannot declare array with an unfixed size.");
                        }
                        type = A_ArrayTy(EM_tokPos, type, var -> u.subscript.exp -> u.intt);
                    }
                    $$ = A_FieldList(A_Field(EM_tokPos, $3 -> u.subscript.name, type), $5);
                }
                else
                {
                    EM_error(EM_tokPos, "Not supported.");
                }
            }
            | FUNCTION COLON id LPAREN tyfield RPAREN ARROW LPAREN tyfield RPAREN COMMA tyfield1 {$$ = A_FieldList(A_Field(EM_tokPos, $3, A_FuncTy(EM_tokPos, $5, $9)), $12);}

funcDec :   FUNCTION COLON id LPAREN tyfield RPAREN ARROW LPAREN tyfield RPAREN stm {$$ = A_FunctionDec(EM_tokPos, A_FundecList(A_Fundec(EM_tokPos, $3, $5, $9, $11), NULL));}
/* funcDec :   type COLON id LPAREN tyfield RPAREN stm {$$ = A_FunctionDec(EM_tokPos, A_FundecList(A_Fundec(EM_tokPos, $3, $5, $1, $7), NULL));} */

funcCall :  varExp LPAREN RPAREN {$$ = A_CallStm(EM_tokPos, $1, NULL);}
            | varExp LPAREN explist RPAREN {$$ = A_CallStm(EM_tokPos, $1, $3);}

jsLoad :    FUNCTION COLON id LPAREN tyfield RPAREN ARROW LPAREN tyfield RPAREN ASSIGN JSLOAD LPAREN STRING COMMA STRING RPAREN SEMICOLON {$$ = A_FuncImport(EM_tokPos, $3, $5, $9, $14, $16);}

jsExport:   JSEXPORT LPAREN id COMMA STRING RPAREN SEMICOLON {$$ = A_FuncExport(EM_tokPos, $3, $5);}

funcvarlist: funcAndVar {$$ = A_DecList($1, NULL);}
            | funcAndVar funcvarlist {$$ = A_DecList($1, $2);}

memlist :   funcvarlist {$$ = A_ClassMemFromDecList($1, A_public);}
            | PUBLIC COLON funcvarlist {$$ = A_ClassMemFromDecList($3, A_public);}
            | PROTECTED COLON funcvarlist {$$ = A_ClassMemFromDecList($3, A_protected);}
            | PRIVATE COLON funcvarlist {$$ = A_ClassMemFromDecList($3, A_private);}

mems:       memlist {$$ = A_ClassMemFromTwoList($1, NULL);}
            | memlist mems {$$ = A_ClassMemFromTwoList($1, $2);} 

classDec:   CLASS COLON id LBRACE mems RBRACE {$$ = A_ClassDec(EM_tokPos, $3, $5, NULL);}
            | CLASS COLON id LEFTARROW id LBRACE mems RBRACE {$$ = A_ClassDec(EM_tokPos, $3, $7, S_SymbolList($5, NULL));}
        // inheritance version

templateDec:TYPE LT id GT COLON funcDec {$$ = A_TemplateDec(EM_tokPos, $3, $6);}
            | TYPE LT id GT COLON classDec {$$ = A_TemplateDec(EM_tokPos, $3, $6);}

assign :    lvalue ASSIGN exp {$$ = A_AssignStm(EM_tokPos, $1, $3, FALSE);}
            | lvalue PLUS PLUS {$$ = A_AssignStm(EM_tokPos, $1, A_OpExp(EM_tokPos, A_plusOp, A_VarExp(EM_tokPos, $1), A_IntExp(EM_tokPos, 1)), FALSE);}
            | lvalue MINUS MINUS {$$ = A_AssignStm(EM_tokPos, $1, A_OpExp(EM_tokPos, A_minusOp, A_VarExp(EM_tokPos, $1), A_IntExp(EM_tokPos, 1)), FALSE);}
            | lvalue PLUS ASSIGN exp {$$ = A_AssignStm(EM_tokPos, $1, A_OpExp(EM_tokPos, A_plusOp, A_VarExp(EM_tokPos, $1), $4), FALSE);}
            | lvalue MINUS ASSIGN exp {$$ = A_AssignStm(EM_tokPos, $1, A_OpExp(EM_tokPos, A_minusOp, A_VarExp(EM_tokPos, $1), $4), FALSE);}

declare :   type COLON assign SEMICOLON {$$ = A_VarDec(EM_tokPos, $3, $1);}
            | type COLON lvalue SEMICOLON{$$ = A_VarDec(EM_tokPos, A_AssignStm(EM_tokPos, $3, NULL, TRUE), $1);}
            | type COLON lvalue LBRACK RBRACK SEMICOLON {$$ = A_VarDec(EM_tokPos, A_AssignStm(EM_tokPos, $3, NULL, TRUE), $1);}
            | type COLON id LPAREN explist RPAREN SEMICOLON {$$ = A_ObjectDec(EM_tokPos, $1, $3, $5);}
            | type COLON id SEMICOLON {$$ = A_ObjectDec(EM_tokPos, $1, $3, NULL);}

if :        IF LPAREN exp RPAREN THEN stm  %prec LOWER_THAN_ELSE {$$ = A_IfStm(EM_tokPos, $3, $6, NULL);}
            | IF LPAREN exp RPAREN THEN stm ELSE stm {$$ = A_IfStm(EM_tokPos, $3, $6, $8);}

while :     LPAREN exp RPAREN TO stm {$$ = A_WhileStm(EM_tokPos, $2, $5);}

for :       LPAREN stm RPAREN FROM LPAREN exp RPAREN TO LPAREN stm RPAREN stm {$$ = A_ForStm(EM_tokPos, $2, $6, $10, $12);}
            | LPAREN exp RPAREN REPEAT stm {$$ = A_ForStm(EM_tokPos, A_DeclarationStm(EM_tokPos, A_VarDec(EM_tokPos, A_AssignStm(EM_tokPos, A_SimpleVar(EM_tokPos, S_Symbol("カウンタ")), A_IntExp(EM_tokPos, 0), TRUE), A_NameTy(EM_tokPos, S_Symbol("int")))), A_OpExp(EM_tokPos, A_eqOp, A_VarExp(EM_tokPos, A_SimpleVar(EM_tokPos, S_Symbol("カウンタ"))), $2), A_AssignStm(EM_tokPos, A_SimpleVar(EM_tokPos, S_Symbol("カウンタ")), A_OpExp(EM_tokPos, A_plusOp, A_VarExp(EM_tokPos, A_SimpleVar(EM_tokPos, S_Symbol("カウンタ"))), A_IntExp(EM_tokPos, 1)), FALSE), $5);} 
            /* | LPAREN INT RPAREN FROM LPAREN INT RPAREN INT TO stm {$$ = A_ForStm} */

loop :      LOOP stm {$$ = A_LoopStm(EM_tokPos, $2); }

explist:    exp COMMA explist {$$ = A_ExpList($1, $3);}
            | exp { $$ = A_ExpList($1, 0); }
            | {$$ = NULL;}
            
id :        ID { $$ = S_Symbol($1); }

lvalue :    id {$$ = A_SimpleVar(EM_tokPos, $1);}
            | lvalue DOT id {$$ = A_FieldVar(EM_tokPos, $1, $3);}
            | TIMES lvalue {$$ = A_DerefVar(EM_tokPos, $2);}
            | lvalue LBRACK exp RBRACK { $$ = A_SubscriptVar(EM_tokPos, $1, $3); }
            | lvalue RIGHTARROW id {$$ = A_ArrowFieldVar(EM_tokPos, $1, $3);}
 