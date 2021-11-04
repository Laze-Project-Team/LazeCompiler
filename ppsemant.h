#pragma once
#ifdef __cplusplus
extern "C" {
#endif
#include "preprocessor.h"
#include "errormsg.h"

void transPPList(Pre_preprocessorList ppList, string file);
void includeFile(string includeStr, string file);
void transPP(Pre_preprocessor pp, string file);
#ifdef __cplusplus
}
#endif