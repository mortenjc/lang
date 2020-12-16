

PROGS = sim6502

all: $(PROGS)

sim6502: sim6502.cpp CPU.h CPU.cpp Memory.h
	g++ -I . sim6502.cpp CPU.cpp --std=c++11 -o sim6502

clean:
	rm -f $(PROGS) a.out
