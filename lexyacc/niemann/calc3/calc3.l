%{
#include <stdlib.h>
#include "calc3.h"
#include "y.tab.h"
void yyerror(char *);

char buffer[1000];
%}


HEX         0x[0-9azAZ]+
BIN         0b[01]+
STR         ["][^"]*["]

%x CCOM CPPCOM

%%



[a-z]       { 
                yylval.sIndex = *yytext - 'a';
                return VARIABLE;
            }

[0-9]+      {   yylval.iValue = atoi(yytext);                return INTEGER; } 
{HEX}       {   yylval.iValue = strtol(yytext+2, NULL, 16);  return INTEGER; }
{BIN}       {   yylval.iValue = strtol(yytext+2, NULL,  2);  return INTEGER; }
{STR}       {   memcpy(buffer, &yytext[1], strlen(yytext)-2); buffer[strlen(yytext)-2]=0; yylval.sStr=buffer; return STRING; }

[-()<>=+*/;{}.] {
                return *yytext;
             }

">="            return GE;
"<="            return LE;
"=="            return EQ;
"!="            return NE;
"while"         return WHILE;
"if"            return IF;
"else"          return ELSE;
"print"         return PRINT;
"++"            return PP;

"/*"            {BEGIN CCOM; }
<CCOM>{
.               {/* ignore */ }
\n              {/* ignore */ }
"*/"            {BEGIN 0;    }
}

"//"            {BEGIN CPPCOM;}
<CPPCOM>{
.               {/* ignore */ }
\n              {BEGIN 0;     }
}

[ \t\n]+        ;       /* ignore whitespace */

.               yyerror("Unknown character");
%%
int yywrap(void) {
    return 1;
}
