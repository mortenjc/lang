%{
#include <stdlib.h>
void yyerror(char *);
#include "y.tab.h"

%}

%%

^\n ;

[0-9]+ { yylval.dval = atoi(yytext);
         return INTEGER;
       }

[0-9]+\.[0-9]+ { yylval.dval = atof(yytext);
                 return FLOAT;
       }

[a-z]   { yylval.ival = *yytext -'a';
          return VARIABLE; }

[-+()*/=\n]  return *yytext;

[ \t]   ;

. yyerror("invalid character");

%%

int yywrap(void) 
{
    return 1;
}
