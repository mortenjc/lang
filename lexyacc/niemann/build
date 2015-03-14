# build environment for cygwin

# bison -d calc1.y    produces calc1.tab.c
# bison -y -d calc1.y produces y.tab.c (the standard yacc output)

# for calc1 and calc2, you may have to
# include the following in the lex ".l" file:
#    extern int yylval;

# calc1
bison -y -d calc1.y
flex calc1.l
gcc -c y.tab.c lex.yy.c
gcc y.tab.o lex.yy.o -o calc1.exe

# calc2
bison -y -d calc2.y
flex calc2.l
gcc -c y.tab.c lex.yy.c
gcc y.tab.o lex.yy.o -o calc2.exe

# calc3
bison -y -d calc3.y
flex calc3.l
gcc -c y.tab.c lex.yy.c
gcc y.tab.o lex.yy.o calc3a.c -o calc3a.exe
gcc y.tab.o lex.yy.o calc3b.c -o calc3b.exe
gcc y.tab.o lex.yy.o calc3g.c -o calc3g.exe
