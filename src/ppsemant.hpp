#pragma once
#include "errormsg.hpp"
#include "preprocessor.h"
#include <vector>

void transPPList(Pre_preprocessorList ppList, const char *file);
void includeFile(const char *includeStr, const char *file);
void transPP(Pre_preprocessor pp, const char *file);

struct PP_cursor
{
    std::string fileName = "";
    int fileNum = 0;
    int lineNum = 0;
};
PP_cursor PP_getLinesInFile(int linePos);
const char * PP_getFilename(int index);