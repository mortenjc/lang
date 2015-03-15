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
   lpc.h     LPC include file.
*/


/* better names */
#define lpc_bw_expand          lpc_bwex
#define lpc_synthesis          lpc_syn
#define lpc_schur              lpc_schr
#define lpc_clamp              lpc_clmp

/* bandwidth expansion function */
int lpc_bwex(float *a, float *aw, float gamma, int p);
/* lpc synthesis filter */
int lpc_syn(float *x,float *y,float *a,int p,int n);

/* sort LSPs and ensure minimum separation */
int lpc_clmp(float *w, float delta, int p);

/* lpc conversion routines */
/* convert predictor parameters to LSPs */
int lpc_pred2lsp(float *a,float *w,int p);
/* convert predictor parameters to reflection coefficients */
int lpc_pred2refl(float *a,float *k,int p);
/* convert LSPs to predictor parameters */
int lpc_lsp2pred(float *w,float *a,int p);
/* convert reflection coefficients to predictor parameters */
int lpc_refl2pred(float *k,float *a,int p);

/* schur recursion */
float lpc_schr(float *r, float *a, float *k_tmp, int p);

/* evaluation of |A(e^jw)|^2 at a single point (using Horner's method) */
float lpc_aejw(float *a,float w,int p);


