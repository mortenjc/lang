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
 
  fs.h: Fourier series functions include file

*/

void fft(float *datam1,int nn,int isign);
void find_harm(float input[], float mag[],float pitch,int num_harm,int length);
int findmax(float input[], int npts);
void idft_real(float real[], float signal[], int length);
