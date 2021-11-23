#include <cstdio>
#include <cstdlib>
#include <cstdarg>
#include <string>
#include "util.h"
#include "errormsg.hpp"

bool anyErrors = FALSE;

char *fileName = "";

int EM_tokPos = 0;
extern FILE *yyin;
extern FILE *prein;

void EM_error(int pos, const char *message, ...)
{
    std::va_list ap;

    anyErrors = TRUE;
    // printf("%s", fileName);
    L_errorPos errorPos = L_getErrorPos(pos);
    std::fprintf(stderr, "%s:", errorPos.fileName.c_str());
    std::fprintf(stderr,  "%d, %d:", errorPos.lineNum, errorPos.columnNum);
    va_start(ap, message);
    std::vfprintf(stderr, message, ap);
    va_end(ap);
    std::fprintf(stderr, "\n");
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