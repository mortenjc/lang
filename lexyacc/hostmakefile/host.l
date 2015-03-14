%{

#include <stdio.h>
//#include "y.tab.h"
void yyerror(char *);
char * psubst(char * i);


#define MJC 300


%}


pname       \[[-.A-Z]+\]

%%


{pname}      {printf("Path: %s", psubst(yytext)); }

^[ ]*"!".*    { /* ignore */ }


\n              { printf("%c", *yytext); }
.               { printf("%c", *yytext); }


%%

int yywrap(void) {return 1; }


char * psubst(char * t)
{
    int i;
    for (i=0; i< strlen(t); i++)
    {
        switch (t[i])
        {
        case '[':
        case ']':
           t[i]=' ';
           break;
        case '.':
           t[i]='/';
        break;
        default:
            break;
        }
    }
    return t;
}


int main()
{
    yylex();
    return 0;
}
