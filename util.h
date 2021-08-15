#pragma once
#include <assert.h>
#include <stdio.h>
#include <stddef.h>
#include <uchar.h>
#include <wchar.h>
#include <locale.h>

struct Number
{
    enum {INTEGER, FLOAT} type;
    union 
    {
        float fval;
        int ival;
    };
};
typedef struct Number num;
typedef char32_t *jpstring;
typedef char *string;
typedef int bool;

#define TRUE 1
#define FALSE 0

void *checked_malloc(int);
string String(char *);
string concat(char *, char *);
bool copyFileContent(string, string);
int getFileSize(FILE *file);

typedef struct U_boolList_ *U_boolList;
struct U_boolList_ {bool head; U_boolList tail;};
U_boolList U_BoolList(bool head, U_boolList tail);
bool toByte(string fname, string tempFileName);