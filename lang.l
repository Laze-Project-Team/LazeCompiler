%{
    #ifdef __cplusplus
    extern "C" {
    #endif
    #include <stdlib.h>
    #include <stdio.h>
    #include "util.h"
    #include "symbol.h"
    #include "absyn.h"
    #include "y.tab.h"
    #include "errormsg.h"

    int charPos = 1;
    int yylex(void);
    int yyparse(void);

    int yywrap(void)
    {
        charPos = 1;
        return 1;
    }

    void adjust(void)
    {
        EM_tokPos = charPos;
        string temp = String(yytext);
        int yytextLeng = 0;
        while(*temp != '\0'){
            if(*temp < 0){
                temp += 3;
            }
            else if(*temp > 0){
                temp++;
            }
            yytextLeng++;
        }
        charPos += yytextLeng;
    }

    void charAdjust(void)
    {
        EM_tokPos = charPos;
        char *temp = String(yytext);
        int yytextLeng = 0;
        while(*temp != '\0'){
            if(*temp < 0){
                temp += 3;
            }
            else if(*temp > 0){
                temp++;
            }
            yytextLeng++;
        }
        charPos += yytextLeng;
    }
    #ifdef __cplusplus
    }
    #endif
%}
digits [0-9]+
char [㐀-龯ぁ-んァ-ヶa-zA-Z_]
charAndNum [㐀-龯ぁ-んァ-ヶa-zA-Z0-9ー_]*
%%
\/\/.*$ {adjust(); continue;}
#.* {adjust(); continue;}

" " {adjust(); continue;}
"   " {adjust(); continue;}
\n {adjust(); EM_newLine(); continue;}
"," {adjust(); return COMMA;}
\. {adjust(); return DOT;}
{digits} {adjust(); yylval.ival = atoll(yytext); return INT;}
0x[0-9a-fA-F]+ {adjust(); /*yylval.ival = strtol(yytext, NULL, 10);*/ sscanf(yytext, "%llx", &(yylval.ival)); return INT;}
{digits}\.{digits}f? {adjust(); yylval.fval = atof(yytext); return REAL;}
'(...|..|.)' {adjust(); strcpy(yylval.cval, strndup(yytext+1, strlen(yytext)-2)); return CHAR;}
\"([^\\\"]|\\.)*\"  {adjust(); yylval.sval = strndup(yytext+1, strlen(yytext)-2); return STRING;}
: {adjust(); return COLON;}
; {adjust(); return SEMICOLON;}
\( {adjust(); return LPAREN;}
\) {adjust(); return RPAREN;}
\[ {adjust(); return LBRACK;}
\] {adjust(); return RBRACK;}
\{ {adjust(); return LBRACE;}
\} {adjust(); return RBRACE;}
\. {adjust(); return DOT;}
\+ {adjust(); return PLUS;}
\- {adjust(); return MINUS;}
\* {adjust(); return TIMES;}
\/ {adjust(); return DIVIDE;}
& {adjust(); return AMPERSAND;}
% {adjust(); return MOD;}
=\> {adjust(); return ARROW;}
\<\- {adjust(); return LEFTARROW;}
\-\> {adjust(); return RIGHTARROW;}
== {adjust(); return EQ;}
!= {adjust(); return NEQ;}
\<= {adjust(); return LE;}
\>= {adjust(); return GE;}
\< {adjust(); return LT;}
\> {adjust(); return GT;}
&& {adjust(); return AND;}
\|\| {adjust(); return OR;}
= {adjust(); return ASSIGN;}

もし {adjust(); return IF;}
ならば {adjust(); return THEN;}
でなければ {adjust(); return ELSE;}
から {adjust(); return FROM;}
まで {adjust(); return TO;}
抜ける {adjust(); return BREAK;}
次へ {adjust(); return CONTINUE;}
関数 {adjust(); return FUNCTION;}
無限ループ {adjust(); return LOOP;}
整数 {adjust(); return INTTYPE;}
整数32 {adjust(); return SHORTTYPE;}
実数 {adjust(); return REALTYPE;}
文字 {adjust(); return CHARTYPE;}
終了 {adjust(); return RETURN;}
型 {adjust(); return TYPE;}
無 {adjust(); return VOID;}
空 {adjust(); return NUL;}
真偽 {adjust(); return BOOLEAN;}
真 {adjust(); return TRUEE;}
偽 {adjust(); return FALSEE;}
js読み込み {adjust(); return JSLOAD;}
js書き出し {adjust(); return JSEXPORT;}
バイト数 {adjust(); return SIZEOF;}
クラス {adjust(); return CLASS;}
公開 {adjust(); return PUBLIC;}
非公開 {adjust(); return PRIVATE;}
保護 {adjust(); return PROTECTED;}
回繰り返す {adjust(); return REPEAT;}

{char}{charAndNum} {adjust(); yylval.sval = strdup(yytext); return ID;}
.	 {adjust(); EM_error(EM_tokPos,"illegal token");}

