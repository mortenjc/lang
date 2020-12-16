

PROGS = sim6502 cputest

all: $(PROGS)

gtest:
	./makegtest

cputest: CPUTest.cpp CPU.cpp CPU.h Memory.h
	g++ -I . -I googletest/googletest/include/ -L googletest/build/lib CPUTest.cpp CPU.cpp --std=c++11 -lgtest -o cputest

sim6502: sim6502.cpp CPU.h CPU.cpp Memory.h Instructions.h
	g++ -I . sim6502.cpp CPU.cpp --std=c++11 -o sim6502

clean:
	rm -f $(PROGS) a.out

realclean: clean
	rm -fr googletest
