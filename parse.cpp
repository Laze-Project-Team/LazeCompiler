/*
 * parse.c - Parse source file.
 */

#include <stdio.h>
#include "util.h"
#include "symbol.h"
#include "absyn.h"
#include "parse.h"

// extern "C" int yyparse(void);
extern int preparse(void);
extern A_decList absyn_root;
extern Pre_preprocessorList ppListRoot;

/* parse source file fname; 
   return abstract syntax data structure */
// A_decList parse(string fname) 
// {
//   EM_reset(fname);
//   if (yyparse() == 0) /* parsing worked */
//     return absyn_root;
//   else return NULL;
// }

Pre_preprocessorList preprocess(string fname)
{
  EM_prereset(fname);
  if(preparse() == 0)
    return ppListRoot;
  else return NULL;
}