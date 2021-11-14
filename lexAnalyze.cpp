#include <stdio.h>
#include <memory>
#include <deque>
#include <iostream>
#include <unordered_map>
#include "util.h"
#include "errormsg.h"
#include "symbol.h"
#include "absyn.h"
#include "parse.h"
#include "printtree.h"
#include "ppsemant.hpp"
#include "toJson.h"
#include "lexer.hpp"
#include "parser.hpp"
#include "converter.hpp"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// extern YYSTYPE yylval;
extern int yydebug;
extern int funcs;
extern int memorySize;

extern "C" int yylex(void);
int prelex(void);

// extern "C" int yyparse(void);
extern "C" int preparse(void);

// void parseTest(string fname)
// {
//     EM_reset(fname);
//     if(yyparse() == 0)
//     {
//         fprintf(stderr, "Parsing Successful\n");
//     }
//     else
//     {
//         fprintf(stderr, "Parsing Failed\n");
//     }
// }

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
    L_tokenList list = L_Lexer(tempFileName, "jalang.json");
    A_decList absyn_root = P_parse(list, "jalang.json");
    std::cout << "Finished Parsing..." << std::endl;
    string resultJsonFile = concat(tempFileName, ".json");
    // A_decList absyn_root = parse(tempFileName);
    jobj jsonAST = JS_toJson(absyn_root);
    fileContent(resultJsonFile, (string)json_object_to_json_string(jsonAST));
    CON_convert(resultJsonFile, "enlang.json");
    Pr_printTree(SEM_transProg(absyn_root), resultFilename);
    printf("Finshed Compiling.\n");
    //printf("%d memorysize\n", memorySize);
    return 0;
}