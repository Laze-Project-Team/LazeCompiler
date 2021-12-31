#pragma once
#include "lexer.hpp"
#ifdef __cplusplus
extern "C" {
#endif
#include "util.h"
extern bool EM_anyErrors;

void EM_newLine(void);

extern int EM_tokPos;

void EM_error(int, const char *, ...);
void debug(int, const char *, ...);
void EM_impossible(const char *, ...);
void EM_reset(const char * filename);
void EM_prereset(const char * filename);
void EM_setErrorJson(const char *filename);
#ifdef __cplusplus
}
#endif