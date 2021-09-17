#include <stdio.h>
#include "util.h"
#include "errormsg.h"
#include "symbol.h"
#include "absyn.h"
#include "y.tab.h"
#include "parse.h"
#include "printtree.h"
#include "ppsemant.h"
#include "toJson.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

YYSTYPE yylval;
extern int yydebug;
extern int funcs;
extern int memorySize;

int yylex(void);
int prelex(void);

extern int yyparse(void);
extern int preparse(void);

string toknames[] = 
{
    "ID", "STRING", "INT", "REAL", "COMMA", "COLON", "SEMICOLON", "LPAREN", "RPAREN", "LBRACK", "RBRACK",
    "LBRACE", "RBRACE", "DOT", "PLUS", "MINUS", "TIME", "DIVIDE", "EQ", "NEQ", "LT", "LE",
    "GT", "GE", "AND", "OR", "ASSIGN", "ARRAY", "IF", "THEN", "ELSE", "FROM", "TO", "BREAK",
    "INTTYPE", "CHARTYPE", "STRINGTYPE", "REALTYPE", "CONTINUE"
};

string tokname(tok)
{
    return tok<258 || tok>295 ? "BAD_TOKEN" : toknames[tok-258];
}

void parseTest(string fname)
{
    EM_reset(fname);
    if(yyparse() == 0)
    {
        fprintf(stderr, "Parsing Successful\n");
    }
    else
    {
        fprintf(stderr, "Parsing Failed\n");
    }
}

int main(int argc, char **argv)
{
    // yydebug = 1;
    funcs = 0;
    // memorySize = 512 * 512 * 4;
    string fname;
    char directory[256];
    int tok;
    if(argc == 1)
    {
        fprintf(stderr, "usage: ./a.out filename\n      ./a.out filename directory\nargument count: %d\n", argc);
        exit(1);
    }
    fname = argv[1];
    getcwd(directory, 256);
    string tempFileName = concat(".", fname);
    string temptempFileName = concat("..", fname);
    string resultFilename = concat(tempFileName, ".wat");
    char *linkFile = NULL;
    for(int i = 2; i < argc; i++){
        if(argv[i][0] == '-'){
            if(argv[i][1] == 'c'){
                i++;
                chdir(argv[i]);
                strcpy(directory, argv[i]);
            }
            if(strcmp(argv[i], "-link") == 0){
                i++;

                linkFile = argv[i];
            }
        }
    }
    FILE *temp = fopen(tempFileName, "w");
    fclose(temp);
    temp = fopen(temptempFileName, "w");
    fclose(temp);
    copyFileContent(fname, temptempFileName);
    if(linkFile){
        includeFile(linkFile, temptempFileName);
    }
    transPPList(preprocess(temptempFileName), temptempFileName);
    EM_reset(tempFileName);
    while(!toByte(temptempFileName, tempFileName));
    fprintf(stdout, "\n");
    printf("Finished Preprocessing...\n");
    string resultJsonFile = concat(tempFileName, ".json");
    A_decList absyn_root = parse(tempFileName);
    jobj jsonAST = JS_toJson(absyn_root);
    fileContent(resultJsonFile, json_object_to_json_string(jsonAST));
    Pr_printTree(SEM_transProg(absyn_root), resultFilename);
    printf("Finshed Compiling.\n");
    //printf("%d memorysize\n", memorySize);
    return 0;
}