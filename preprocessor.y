%{
    #include "preprocessor.h"
    #include "errormsg.hpp"
    Pre_preprocessorList ppListRoot;

    int prelex(void);

    void preerror(const char *s)
    {
        EM_error(0, "%s", s);
    }
%}

%union
{
    string sval;
    Pre_preprocessor ppVal;
    Pre_preprocessorList ppListVal;
    Pre_argument argVal;
}

%token <sval> PRE_ID PRE_STRING

%type <ppVal> pp
%type <ppListVal> ppList
%type <argVal> arg

%token
    PRE_HASHTAG PRE_RIGHTANG PRE_LEFTANG PRE_INCLUDE

%start program

%%

program: ppList {ppListRoot = $1;}

ppList: pp {$$ = Pre_PreprocessorList($1, NULL);}
            | ppList pp {$$ = Pre_PreprocessorList($2, $1);}
            | {$$ = NULL;}

pp: PRE_HASHTAG PRE_INCLUDE arg {$$ = Pre_Include($3);}

arg: PRE_LEFTANG PRE_ID PRE_RIGHTANG {$$ = Pre_Module($2);}
   | PRE_STRING {$$ = Pre_String($1);}