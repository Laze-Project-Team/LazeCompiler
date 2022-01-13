#include <stdio.h>
#include <memory>
#include <deque>
#include <iostream>
#include <unordered_map>
#include "util.h"
#include "errormsg.hpp"
#include "symbol.h"
#include "absyn.h"
#include "parse.h"
#include "printtree.hpp"
#include "ppsemant.hpp"
#include "toJson.hpp"
#include "lexer.hpp"
#include "parser.hpp"
#include "converter.hpp"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <chrono>

// extern YYSTYPE yylval;
extern int yydebug;
extern int funcs;
extern int memorySize;

// extern "C" int yylex(void);
int prelex(void);

// extern "C" int yyparse(void);
extern "C" int preparse(void);

A_decList getAST(char *fname, char *directory, char *currentDirectory, char *parseJsonName, char *mode, char *parserFileName){
    chdir(directory);
    string tempFileName = concat(".", fname);
    string temptempFileName = concat("..", fname);
    FILE *temp = fopen(tempFileName, "w");
    fclose(temp);
    temp = fopen(temptempFileName, "w");
    fclose(temp);
    copyFileContent(fname, temptempFileName);
    transPPList(preprocess(temptempFileName), temptempFileName);
    while(!toByte(temptempFileName, tempFileName));
    L_tokenList list = L_Lexer(tempFileName, parseJsonName, mode);
    remove(tempFileName);
    remove(temptempFileName);
    A_decList absyn_root = NULL;
    if(parserFileName){
        absyn_root = P_parseWithFile(list, parseJsonName, parserFileName);
    }else{
        absyn_root = P_parse(list, parseJsonName);
    }
    chdir(currentDirectory);
    return absyn_root;
}

int main(int argc, char **argv)
{
    auto start = std::chrono::steady_clock::now();
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
    getcwd(directory, 256);
    string parseJsonName = "";
    string convertJsonName = "";
    string mode = "compile";
    string convertOutput = "";
    string parserOutput = "";
    string parserFileName = NULL;
    char *linkFile = NULL;
    char *convertLinkFile = NULL;
    char *convertDirectory = NULL;
    for(int i = 1; i < argc; i++){
        if(argv[i][0] == '-'){
            if(argv[i][1] == 'c'){
                i++;
                chdir(argv[i]);
                strcpy(directory, argv[i]);
            }
            if(strcmp(argv[i], "--link") == 0){
                i++;

                linkFile = argv[i];
            }
            if(strcmp(argv[i], "--parse-json") == 0){
                i++;
                parseJsonName = argv[i];
            }
            if(strcmp(argv[i], "--convert-json") == 0){
                i++;
                convertJsonName = argv[i];
            }
            if(strcmp(argv[i], "--convert-output") == 0){
                i++;
                convertOutput = argv[i];
            }
            if(strcmp(argv[i], "--parser-output") == 0){
                i++;
                parserOutput = argv[i];
            }
            if(strcmp(argv[i], "--mode") == 0){
                i++;
                mode = argv[i];
            }
            if(strcmp(argv[i], "--parser-opt") == 0){
                i++;
                parserFileName = argv[i];
            }
            if(strcmp(argv[i], "--convert-link") == 0){
                i++;
                convertLinkFile = argv[i];
            }
            if(strcmp(argv[i], "--convert-dir") == 0){
                i++;
                convertDirectory = argv[i];
            }
        }
    }
    if(strcmp(mode, "parserload") == 0){
        std::vector<std::pair<std::string, std::wregex>> regexMap = L_genTokenNames(parseJsonName);
        P_generateParseFile(parseJsonName, parserOutput);
        std::cout << "Finished Generating Parser." << std::endl;
        return 0;
    }
    fname = argv[1];
    string tempFileName = concat(".", fname);
    string temptempFileName = concat("..", fname);
    string resultFilename = concat(tempFileName, ".wat");
    FILE *temp = fopen(tempFileName, "w");
    fclose(temp);
    temp = fopen(temptempFileName, "w");
    fclose(temp);
    copyFileContent(fname, temptempFileName);
    EM_setErrorJson(parseJsonName);
    if(linkFile){
        includeFile(linkFile, temptempFileName);
    }
    transPPList(preprocess(temptempFileName), temptempFileName);
    while(!toByte(temptempFileName, tempFileName));
    L_tokenList list = L_Lexer(tempFileName, parseJsonName, mode);
    remove(tempFileName);
    remove(temptempFileName);
    A_decList absyn_root = NULL;
    if(parserFileName){
        absyn_root = P_parseWithFile(list, parseJsonName, parserFileName);
    }else{
        absyn_root = P_parse(list, parseJsonName);
    }
    if(strcmp(mode, "convert") == 0){
        A_decList linkFileAST = getAST(convertLinkFile, convertDirectory, directory, parseJsonName, mode, parserFileName);
        string convertResultJsonFile = concat(convertLinkFile, ".json");
        jobj convertJsonAST = JS_toJson(linkFileAST);
        fileContent(convertResultJsonFile, (string)json_object_to_json_string(convertJsonAST));
        CON_convert(convertResultJsonFile, convertJsonName, convertOutput);
        remove(convertResultJsonFile);
        string resultJsonFile = concat(tempFileName, ".json");
        jobj jsonAST = JS_toJson(absyn_root);
        fileContent(resultJsonFile, (string)json_object_to_json_string(jsonAST));
        CON_convert(resultJsonFile, convertJsonName, convertOutput);
        // std::cout << "Finished Converting." << std::endl;
    }
    else if(strcmp(mode, "compile") == 0){
        Pr_printTree(SEM_transProg(absyn_root), resultFilename);
        // printf("Finished Compiling.\n");
    }
    auto end = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "Execution Time:" << duration.count() << "ms" << std::endl;
    return 0;
}