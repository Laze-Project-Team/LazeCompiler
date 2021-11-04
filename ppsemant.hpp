#include "errormsg.h"
#include "preprocessor.h"

void transPPList(Pre_preprocessorList ppList, char *file);
void includeFile(char *includeStr, char *file);
void transPP(Pre_preprocessor pp, char *file);