
%{

#include "..\..\stubyac.c"

%}

// *************************************************************************
// lesson: infix notation with yacc precedence and associativity 

//   1+2*3 = 7
//   8/2*3 = 12

%left       '+' '-'
%left       '*' '/'
%right      '|'
%nonassoc   UNARY

%%

sum:     expr                    { printf(" = %d\n", $1);   }
         ;

expr:    digit                   { $$ = $1;           }
         | expr '+' expr         { $$ = $1 + $3;      }
         | expr '-' expr         { $$ = $1 - $3;      }
         | expr '*' expr         { $$ = $1 * $3;      }
         | expr '/' expr         { $$ = $1 / $3;      }
         | expr '|' expr         { $$ = pow($1,$3);   }
         | '-' expr %prec UNARY  { $$ = -$1;          }
         | '(' expr ')'          { $$ = $2            }
         ;

digit:   '0'                     { $$ = 0; }
         | '1'                   { $$ = 1; }
         | '2'                   { $$ = 2; }
         | '3'                   { $$ = 3; }
         | '4'                   { $$ = 4; }
         | '5'                   { $$ = 5; }
         | '6'                   { $$ = 6; }
         | '7'                   { $$ = 7; }
         | '8'                   { $$ = 8; }
         | '9'                   { $$ = 9; }
         ;



%%

// *************************************************************************

void main(void)
{
  yyparse();
}

