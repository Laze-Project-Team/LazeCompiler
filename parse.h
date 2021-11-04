#pragma once
#ifdef __cplusplus
extern "C" {
#endif
#include "preprocessor.h"
/* function prototype from parse.c */
A_decList parse(string fname);
Pre_preprocessorList preprocess(string fname);

#ifdef __cplusplus
}
#endif