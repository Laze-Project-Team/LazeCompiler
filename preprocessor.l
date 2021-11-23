%{
    #include <stdio.h>
    #include <string.h>
    #include "errormsg.hpp"    
    #include "preprocessor.h"
    #include "preprocessor.tab.h"

    extern "C" int prewrap(void)
    {
        return 1;
    }
%}

char [㐀-龯ぁ-んァ-ヶa-zA-Z]
charAndNum [㐀-龯ぁ-んァ-ヶa-zA-Z0-9]*

%%
^(?:[^(#)]|[\(\)]).* {}

\n {}
" " {}
"   " {}
# {return PRE_HASHTAG;}
\".*\" {prelval.sval = strndup(pretext+1, strlen(pretext)-2);return PRE_STRING;}
\< {return PRE_LEFTANG;}
\> {return PRE_RIGHTANG;}

include {return PRE_INCLUDE;}

{char}{charAndNum} {prelval.sval = strdup(pretext); return PRE_ID;}
. {EM_error(0, "Illegal token in preprocessor.");}
