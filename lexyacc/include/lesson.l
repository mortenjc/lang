%{

// scanning with file includsion
// nu med lex-macro og union
  
// *************************************************************************
// Start of copied C-code                                               
// *************************************************************************
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

// *************************************************************************

typedef union token
{
   char        ident[40];
   int         token;
   int         integer;
   float       real;
   char       *string;
} yystype;

#define YYSTYPE yystype

YYSTYPE yylval;

#define IF         258
#define ELSE       259
#define WHILE      260
#define IDENT      261
#define INTNUM     262
#define REAL       263
#define STRING     264

static char buffer[1000];

/* ----------------------------------------------------------------------- */
/* the #include file stack                                                 */
/* ----------------------------------------------------------------------- */

#define MAX_INCLUDE_DEPTH 10

YY_BUFFER_STATE include_stack[MAX_INCLUDE_DEPTH];

static int include_sp;

static char incfile[260];

void init_include_stack(void)
{
   include_sp = 0;   
}

%}


IDENT          [a-zA-Z][0-9a-zA-Z_.$]* 
DEC            [0-9]+
HEX            [0][xX][0-9a-fA-F]+
BIN            [0][b][01]+
REAL           [0-9]*[.][0-9]+
STR            ["][^"]*["]
WS             [ \t]
INCFILE        [<][a-z0-9_$.^>]*[>]

%x             CCOMNT PCOMNT INCL

%%


   /* *********************************** */
   /* macro: skip C comments              */
   /* *********************************** */

"/*"           { BEGIN CCOMNT;                                    }
<CCOMNT>{                                                         

"*/"           { BEGIN 0;                                         }
.              { /* ignore */                                     }
\n             { /* ignore */                                     }
}

   /* *********************************** */
   /* macro: skip C++ comments            */
   /* *********************************** */

"//"           { BEGIN PCOMNT;                                    }
<PCOMNT>{

.              /* ignore */
\n             { BEGIN 0;                                         }
}

   /* *********************************** */
   /* macro: open #include file           */
   /* *********************************** */

"#include"     { BEGIN INCL;                      }
<INCL>{

{INCFILE}      {
                  /* switch the scanner input * to include a <file> */  
                  FILE *fp;

                  strcpy(incfile, &yytext[1]);    
                  incfile[strlen(incfile) - 1] = 0;

                  if (include_sp >= MAX_INCLUDE_DEPTH)
                  {
                     printf("Includes nested too deeply at level %d\n",include_sp);
                  }
                  else
                  {
                     include_stack[include_sp++] = YY_CURRENT_BUFFER;
   
                     /* printf("include %s\n", incfile); */
   
                     fp = fopen(incfile, "r");
                     if (fp == 0)
                     {
                        printf("Include file '%s' not found\n", incfile);
                        yyterminate();
                     }
                     else
                     {
                        yyin = fp;
                        yy_switch_to_buffer(yy_create_buffer(yyin, YY_BUF_SIZE));
                     }
                  }

                  BEGIN 0;
               }

.              { }

\n             {  BEGIN 0; }

}

   /* *********************************** */
   /* EOF: return from included file or   */
   /* exit if no more files               */
   /* *********************************** */

<<EOF>>        {
                  if (--include_sp < 0)
                  {
                      include_sp = 0;
                      yyterminate();
                  }
                  else
                  {
                     fclose(yyin);
                     yy_delete_buffer(YY_CURRENT_BUFFER);
                     yy_switch_to_buffer(include_stack[include_sp]);
                  }
               }

   /* *********************************** */
   /* tokens                              */
   /* *********************************** */

if             { return IF;                                       }
else           { return ELSE;                                     }
while          { return WHILE;                                    }
{WS}           { /* ignore spaces */                              }
{IDENT}        { strcpy(yylval.ident, yytext);   return IDENT;    }
{DEC}          { yylval.integer = atoi(yytext);  return INTNUM;   }
{HEX}          { yylval.integer = atoi(yytext);  return INTNUM;   }
{BIN}          { yylval.integer = strtol(&yytext[2], NULL, 2); return INTNUM; }
{REAL}         { yylval.real    = atof(yytext);  return REAL;     }
{STR}          { strcpy(buffer, &yytext[1]); 
                 yylval.string = buffer; 
                 yylval.string[yyleng - 2] = 0;
                 return STRING;   
               }
.              { /* all other except newline */  return yytext[0];}
\n             { /* the newline */                                }

%%

// C++ comments allowed again

// *************************************************************************
// More C-code, now in LEX code scope
// *************************************************************************

int yywrap(void)
{
   return 1;
}

// -------------------------------------------------------------------------

int main(int argc, char **argv)
{
   int i = 1;

   if (argc != 2)
   {
      printf("Filename missing\n");
      return;
   }

   yyin = fopen(argv[1], "r");
   if (yyin == 0) 
   {
      printf("Could not open infile\n");
      return;
   }

   while (i)
   {
      i = yylex();

      switch (i)
      {
         case IF      : printf("token   IF   \n"  , yylval.ident  ); break;
         case ELSE    : printf("token   ELSE \n"  , yylval.ident  ); break;
         case WHILE   : printf("token   WHILE\n"  , yylval.ident  ); break;
         case IDENT   : printf("ident   %s   \n"  , yylval.ident  ); break;
         case INTNUM  : printf("number  %d   \n"  , yylval.integer); break;
         case REAL    : printf("real    %f   \n"  , yylval.real   ); break;
         case STRING  : printf("string '%s'  \n"  , yylval.string ); break;
         default:
         {
             printf("token   %c\n", i);
         }
      }
   }
   printf("\n");

   fclose(yyin);
   return 0;
}
