#include <cstdio>
#include <cstdlib>
#include <cstdarg>
#include <string>
#include <nlohmann/json.hpp>
#include "util.h"
#include "errormsg.hpp"

bool anyErrors = FALSE;

char *fileName = "";

int EM_tokPos = 0;
extern FILE *yyin;
extern FILE *prein;

json errorJson;

void EM_setErrorJson(const char *filename){
    std::ifstream input(filename);
    json j;
    input >> j;
    json error;
    errorJson = j["errors"];
}

void EM_error(int pos, const char *message, ...)
{
    std::va_list ap;

    anyErrors = TRUE;
    // printf("%s", fileName);
    L_errorPos errorPos = L_getErrorPos(pos);

    std::string fileName = errorPos.fileName;
    if(errorPos.fileName.find('/') != errorPos.fileName.npos){
        fileName = errorPos.fileName.substr(errorPos.fileName.find_last_of("/") + 1);
    }
    std::fprintf(stderr, "%s:", fileName.substr(2).c_str());
    std::fprintf(stderr,  "%d%s, %d%s:", errorPos.lineNum, errorJson["keywords"]["lineNum"].get<std::string>().c_str(), errorPos.columnNum, errorJson["keywords"]["charNum"].get<std::string>().c_str());
    char errorMes[1024] = "";
    std::stringstream errorInput(errorMes);
    va_start(ap, message);
    std::vsprintf(errorMes, message, ap);
    errorInput << errorMes;
    // std::vfprintf(stderr, message, ap);
    va_end(ap);
    std::string errorCode = "";
    errorInput >> errorCode;
    std::string errorArg;
    std::vector<std::string> errorArgs;
    while(errorInput >> errorArg){
        errorArgs.push_back(errorArg);
    }
    std::string parent = errorCode.substr(0, errorCode.find('.'));
    std::string child = errorCode.substr(errorCode.find('.') + 1);
    std::string errorOutput = errorCode;
    if(errorJson[parent][child].is_string()){
        errorOutput = errorJson[parent][child].get<std::string>();
    }
    for(int i = 0; i < errorArgs.size(); i++){
        errorOutput = std::regex_replace(errorOutput, std::regex("\\$" + std::to_string(i + 1)), errorArgs.at(i));
    }
    std::cerr << errorOutput << std::endl;
    std::exit(0);
}

void debug(int pos, const char *message, ...)
{
    va_list ap;
    anyErrors = TRUE;
    // printf("%s", fileName);
    L_errorPos errorPos = L_getErrorPos(pos);
    fprintf(stdout, "%s:", errorPos.fileName.c_str());
    fprintf(stdout,  "%d, %d:", errorPos.lineNum, errorPos.columnNum);
    va_start(ap, message);
    vfprintf(stdout, message, ap);
    va_end(ap);
    fprintf(stdout, "\n");
}

void EM_reset(const string fname)
{
    anyErrors = FALSE;
    // fileName = fname;
    // lineNum = 1;
    // linePos = intList(0, NULL);
    FILE *yyin = fopen(fname, "r, ccs = UTF16LE");
    // if(!yyin)
    // {
    //     EM_error(0, "cannot open");
    //     exit(1);
    // }
}

void EM_prereset(const char *fname)
{
    anyErrors = FALSE;
    // fileName = fname;
    // lineNum = 1;
    // linePos = intList(0, NULL);
    prein = fopen(fname, "r, ccs = UTF16LE");
    if(!prein)
    {
        EM_error(0, "cannot open");
        exit(1);
    }
}