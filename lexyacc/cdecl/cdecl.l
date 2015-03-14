%{
#include <stdlib.h>
void yyerror(char *);
#include "y.tab.h"

%}

%%

[ \t\n]   ;  /* ignore whitespace  */

int     { return INT; }
char    { return CHAR;   }
struct  { return STRUCT; }


[a-zA-Z][_a-zA-Z0-9]* { yylval.sval = strdup(yytext); 
                   return NAME; }

[0-9]+          {yylval.ival = atoi(yytext);
                 return INTEGER;
                }

[-,+*/=(){};\[\]]  return *yytext;

.  { printf("xx%cxx\n", *yytext); yyerror("invalid character"); }

%%

int yywrap(void) 
{
    return 1;
}
