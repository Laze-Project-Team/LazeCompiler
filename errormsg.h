#pragma once
#include "util.h"
typedef struct intList { int i; struct intList *rest; } *IntList;
IntList intList(int i, IntList rest);
extern bool EM_anyErrors;

void EM_newLine(void);

extern int EM_tokPos;

void EM_error(int, string, ...);
void debug(int, string, ...);
void EM_impossible(string, ...);
void EM_reset(string filename);
void EM_prereset(string filename);