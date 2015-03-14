
BUILDDIR=build

OBJECTS=lex.yy.o y.tab.o

lex.yy.o: lex.yy.c y.tab.h
	gcc -c $(BUILDDIR)/$< -o $(BUILDDIR)/$@

y.tab.o: y.tab.c
	gcc -c $(BUILDDIR)/$< -o $(BUILDDIR)/$@

lex.yy.c: cdecl.l
	flex -o $(BUILDDIR)/$@ -l $<

y.tab.h: y.tab.c
	echo nothing

y.tab.c: cdecl.y
	bison -d -y -v $< -o $(BUILDDIR)/$@

clean:
	rm -rf a.out y.* lex.yy.* test.o test.c $(BUILDDIR)

builddir: $(BUILDDIR)

$(BUILDDIR):
	mkdir -p $@


maketest: test
	cp test test.c


all: builddir $(OBJECTS)
	gcc $(BUILDDIR)/*.o 

