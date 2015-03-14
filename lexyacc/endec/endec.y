%{
   #define pf printf("%-0.*s", indent, "                         "); printf
   #include <stdio.h>
   int yylex(void);
   void yyerror(char* );

struct name
{
    char  *name;
    struct name * next;
    int    type;
};

struct name names;
int    indent;

void makemessage(char * name);
void endmessage();
void addtype(char * a, char *b);

%}

%error-verbose 

%union { int ival; float fval; char * sval;}

%token INTEGER MESSAGE FLOAT

%token <sval> NAME

%%

interface: 
    interface statement 
    | 
    ;

statement:
    MESSAGE NAME  {makemessage($2);} '{' dec_list '}' { endmessage();}
    ;

dec_list:   
      decl
    | dec_list decl 
    ;


decl:
      INTEGER NAME     { addtype("int", $2); }
    | FLOAT   NAME     { addtype("double", $2); }
    | statement            
    ;

%%

void yyerror(char * s) { fprintf(stderr, "%s \n", s); }

int main()
{
    indent=-4;
    yyparse();
    return 0;
}


void makemessage(char * name)
{
    indent += 4;
    pf("struct msg_%s\n", name);
    pf("{\n");
}

void endmessage()
{
    pf("};\n\n");
    indent -= 4;
}

void addtype(char * type, char * name)
{
    indent +=4;
    pf("%-17s %s;\n", type, name);
    indent -=4;
}

