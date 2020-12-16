

PROGS = refs1 trace unaligned

all: $(PROGS)

refs1: refs1.cpp
	g++ --std=c++11 $< -o $@

trace: trace.cpp
	g++ --std=c++11 $< -o $@

unaligned: unaligned.cpp
	g++ --std=c++11 $< -o $@

clean:
	rm -f $(PROGS) a.out
