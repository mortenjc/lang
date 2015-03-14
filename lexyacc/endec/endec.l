%{
#include <stdlib.h>
void yyerror(char *);
#include "y.tab.h"

%}

%%

[ \t\n]   ;  /* ignore whitespace  */

integer { return INTEGER; }
float   { return FLOAT;   }
message { return MESSAGE; }


[a-z][_a-z0-9]* { yylval.sval = strdup(yytext); 
                 return NAME; }

[{};]  return *yytext;

.  { yyerror("invalid character"); }

%%

int yywrap(void) 
{
    return 1;
}
