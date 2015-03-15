/*

2.4 kbps MELP Proposed Federal Standard speech coder

version 1.2

Copyright (c) 1996, Texas Instruments, Inc.  

Texas Instruments has intellectual property rights on the MELP
algorithm.  The Texas Instruments contact for licensing issues for
commercial and non-government use is William Gordon, Director,
Government Contracts, Texas Instruments Incorporated, Semiconductor
Group (phone 972 480 7442).


*/

/*
   spbstd.h   SPB standard header file.

   Copyright (c) 1995 by Texas Instruments, Inc.  All rights reserved.
*/

#ifndef _spbstd_h
#define _spbstd_h

/*
** Needed include files.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/* OSTYPE-dependent definitions/macros. */

#ifdef SunOS4

/* some standard C function definitions missing from SunOS4 */
extern int fclose(FILE *stream);
extern int fprintf(FILE *stream, const char *format, ...);
extern size_t fread(void *ptr, size_t size, size_t nobj, FILE *stream);
extern int fseek(FILE *stream, long offset, int origin);
extern size_t fwrite(const void *ptr, size_t size, size_t nobj, FILE *stream);
extern int printf(const char *format, ...);
extern long random(void);
extern int sscanf (char *s, const char *format, ...);
extern void rewind(FILE *stream);

#else

#endif

/*
** Constant definitions.
*/
#ifndef FALSE
#define FALSE           0
#endif
#ifndef M_PI
#define     M_PI    3.14159265358979323846
#endif
#ifndef PI
#define PI              M_PI
#endif
#ifndef TRUE
#define TRUE            1
#endif
#ifndef TWOPI
#define TWOPI 6.28318530717958647692
#endif

/*
** Macros.
*/

#ifndef FREE
#define FREE(v)         if(v)(void)free((void*)(v))
#endif
#ifndef program_abort
#define program_abort(s1,s2,i1,i2) (void)fprintf(stderr,"%s: %s (instance %d, line %d)",s1,s2,i1,i2),exit(1)
#endif
#ifndef SQR
#define SQR(x)          ((x)*(x))
#endif

/* Generic memory allocation/deallocation macros. */

#define MEM_ALLOC(alloc_routine, v, n, type) \
        if(((v) = (type*) alloc_routine((n) * sizeof(type)))!=NULL)\
                ; else program_abort(__FILE__,"MEM_ALLOC",0,__LINE__)
#define MEM_2ALLOC(alloc_routine,v,n,k,type) \
                if((v=(type**)alloc_routine(sizeof(type*)*(n)))!=NULL\
		   &&(v[0]=(type*)alloc_routine(sizeof(type)*(n)*(k)))!=NULL)\
                     {int u__i; for(u__i=1; u__i < n; u__i++)\
                                v[u__i] = &v[u__i-1][k];\
                     }\
                else\
                        program_abort(__FILE__,"MEM_2ALLOC",0,__LINE__)

#define MEM_FREE(free_routine, v) \
    free_routine(v)
#define MEM_2FREE(free_routine, v) \
    if (1) { free_routine((v)[0]); free_routine(v); } else exit(1)

/* lint-dependent macros. */

#ifdef lint
#define MALLOC(n)   (malloc((unsigned)(n)),NULL)
#define VA_ARG(v,type) (v,(type)NULL)
#else
#define MALLOC(n)   malloc((unsigned)(n))
#define VA_ARG(v,type) va_arg(v,type)
#endif

#endif /* #ifndef _spbstd_h */
