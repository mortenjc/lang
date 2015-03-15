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
   mat.h     Matrix include file.
             (Low level matrix and vector functions.)  

   Copyright (c) 1995 by Texas Instruments, Inc.  All rights reserved.
*/

float v_inner(float *v1,float *v2,int n);
float v_magsq(float *v,int n);
float *v_zap(float *v,int n);
float *v_equ(float *v1,float *v2,int n);
float *v_sub(float *v1,float *v2,int n);
float *v_add(float *v1,float *v2,int n);
float *v_scale(float *v,float scale,int n);
int *v_zap_int(int *v,int n);
int *v_equ_int(int *v1,int *v2,int n);

