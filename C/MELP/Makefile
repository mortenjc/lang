#
#   Commands:  
#      make OSTYPE=xxx  [opt]         Create optimized executable melp
#      make OSTYPE=xxx  debug         Create debugging executable melp
#      make clean         	      Remove existing object files

OBJS = melp.o melp_ana.o melp_syn.o melp_chn.o coeff.o
OBJS+=  fsvq_cb.o msvq_cb.o fec_code.o dsp_sub.o melp_sub.o
OBJS+= mat_lib.o lpc_lib.o vq_lib.o fs_lib.o pit_lib.o

EXE     =       melp

CC         = gcc 
CFLAGS     = -O3 -D$(OSTYPE) -Wall
LIBS	   = -lm
COMPILE.c  = $(CC) $(CFLAGS) -c
LINK.c     = $(CC) $(CFLAGS) 

debug := CFLAGS = -g -D$(OSTYPE) -Wall

opt debug:  $(OBJS)
	$(LINK.c) -o $(EXE) $(OBJS) $(LIBS)

purify:  $(OBJS)
	purify $(LINK.c) -o $(EXE) $(OBJS) $(LIBS)

clean:
	rm -f $(OBJS) $(EXE)
