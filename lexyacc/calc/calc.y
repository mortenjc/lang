%{
   #include <stdio.h>
   int yylex(void);
   void yyerror(char* );
   int sym[26];
   int syminit[26];

   union 
   {
       int    ival;
       float  fval;
   } types;
%}

%union { double dval; int ival; }
%token <dval> INTEGER 
%token <dval> FLOAT
%token <ival> VARIABLE 
%type <dval> statement
%type <dval> expr
%left '+' '-'
%left '*' '/'

%%

program: 
    program statement '\n'
    |
    ;

statement:
     expr               { printf("%f\n", $1); }
   | VARIABLE '=' expr  { sym[$1] = $3; syminit[$1]=1;}

expr: 
      INTEGER        
    | FLOAT          
    | VARIABLE       { if (syminit[$1] != 1)
                          yyerror("uninitialized variable");
                       else
                          $$ = sym[$1]; }
    | expr '+' expr  { $$ = $1 + $3; }
    | expr '-' expr  { $$ = $1 - $3; }
    | expr '*' expr  { $$ = $1 * $3; }
    | expr '/' expr  { $$ = $1 / $3; }
    | '(' expr ')'   { $$ = $2;      }
    ;

%%

void yyerror(char * s)
{
    fprintf(stderr, "%s\n", s);
}

int main()
{
    yyparse();
    return 0;
}
