CFLAGS = -DWIN

lesson1.exe : lesson.yac 
   yacc -v -d lesson.yac -o ytab.c
   bcc32 -elesson ytab.c
   

clean :
 if exist *.obj          erase *.obj
 if exist *.exe          erase *.exe
 if exist *.tds          erase *.tds
 if exist *.tr2          erase *.tr2
 if exist *.td2          erase *.td2
 if exist *.out          erase *.out
 if exist ytab.*         erase ytab.*
 if exist lexyy.c        erase lexyy.c
 if exist brief.rst      erase brief.rst
