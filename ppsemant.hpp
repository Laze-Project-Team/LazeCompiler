#pragma once
#include "errormsg.h"
#include "preprocessor.h"
#include <vector>

void transPPList(Pre_preprocessorList ppList, char *file);
void includeFile(char *includeStr, char *file);
void transPP(Pre_preprocessor pp, char *file);

struct PP_cursor
{
    int fileNum;
    int lineNum;
};
PP_cursor PP_getLinesInFile(int linePos);