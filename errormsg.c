#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "util.h"
#include "errormsg.h"

bool anyErrors = FALSE;

static string fileName = "";
static int lineNum = 0;
int EM_tokPos = 0;
extern FILE *yyin;
extern FILE *prein;

// typedef struct intList { int i; struct intList *rest; } *IntList;

IntList intList(int i, IntList rest)
{
    IntList l = checked_malloc(sizeof *l);
    l->i=i;
    l->rest = rest;
    return l;
}

static IntList linePos = NULL;

void EM_newLine(void)
{
    lineNum++;
    linePos = intList(EM_tokPos, linePos);
}
void EM_error(int pos, char *message, ...)
{
    va_list ap;
    IntList lines = linePos;
    int num = lineNum;

    anyErrors = TRUE;
    while(lines && lines -> i >= pos)
    {
        lines = lines->rest; num++;
    }
    // printf("%s", fileName);
    if(fileName) fprintf(stderr, "%s:", fileName);
    if(lines) fprintf(stderr,  "%d, %d:", num, pos-lines->i);
    va_start(ap, message);
    vfprintf(stderr, message, ap);
    va_end(ap);
    fprintf(stderr, "\n");
    exit(0);
}

void debug(int pos, char *message, ...)
{
    va_list ap;
    IntList lines = linePos;
    int num = lineNum;

    anyErrors = TRUE;
    while(lines && lines -> i >= pos)
    {
        lines = lines->rest; num++;
    }
    // printf("%s", fileName);
    if(fileName) fprintf(stdout, "%s:", fileName);
    if(lines) fprintf(stdout,  "%d, %d:", num, pos-lines->i);
    va_start(ap, message);
    vfprintf(stdout, message, ap);
    va_end(ap);
    fprintf(stdout, "\n");
}

void EM_reset(string fname)
{
    anyErrors = FALSE;
    fileName = fname;
    lineNum = 1;
    linePos = intList(0, NULL);
    yyin = fopen(fname, "r, ccs = UTF16LE");
    if(!yyin)
    {
        EM_error(0, "cannot open");
        exit(1);
    }
}

void EM_prereset(string fname)
{
    anyErrors = FALSE;
    fileName = fname;
    // lineNum = 1;
    // linePos = intList(0, NULL);
    prein = fopen(fname, "r, ccs = UTF16LE");
    if(!prein)
    {
        EM_error(0, "cannot open");
        exit(1);
    }
}