#pragma once
#include "preprocessor.h"

void transPPList(Pre_preprocessorList ppList, string file);
void includeFile(string includeStr, string file);
void transPP(Pre_preprocessor pp, string file);