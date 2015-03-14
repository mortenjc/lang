%{
   #include <stdio.h>
   int yylex(void);
   void yyerror(char* );

%}

%error-verbose 

%union { int ival; float fval; char * sval;}

%token <ival>INT <ival>CHAR <ival>STRUCT
%token <ival>INTEGER
%token <sval> NAME

%type <ival> expr

%left '+' '-'
%left '*' '/'
%left ','
%left '[' ']'

%%

declares:   /*nothing*/
          | declare
          | declares declare
          ;

declare:    field ';'
          | field '=' expr ';'
          ;

type:       INT
          | CHAR
          | STRUCT '{' fields '}'
          | STRUCT NAME '{' fields '}'
          ;

fields:     field ';'
          | fields field ';'
          ;

varlist:   NAME
          | varlist '[' INTEGER ']'
	  | varlist ',' varlist
	  ;
	  	  
field:      type varlist
	  ;
	 
expr:       INTEGER         
          | '('  expr  ')'  {$$ = $2;     }
          |  expr '+' expr  {$$ = $1 + $3;}
          |  expr '-' expr  {$$ = $1 - $3;}
          |  expr '*' expr  {$$ = $1 * $3;}
          |  expr '/' expr  {$$ = $1 / $3;}
          ;

%%

void yyerror(char * s) { fprintf(stderr, "%s \n", s); }

int main()
{
    yyparse();
    return 0;
}

