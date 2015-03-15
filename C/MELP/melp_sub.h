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

  melp_sub.h: include file for MELP subroutines

*/

void dc_rmv(float sigin[], float sigout[], float dcdel[], int frame);
void bpvc_ana(float speech[], float fpitch[], float bpvc[], float sub_pitch[]);
void bpvc_ana_init(int fr, int pmin, int pmax, int nbands, int num_p, int lmin);
float gain_ana(float sigin[], float pitch, int minlength, int maxlength);
void q_gain(float *gain,int *gain_index,float qlow,float qup,float qlev);
void q_gain_dec(float *gain,int *gain_index,float qlow,float qup,float qlev);
int q_bpvc(float *bpvc,int *bpvc_index,float bpthresh,int num_bands);
void q_bpvc_dec(float *bpvc,int *bpvc_index,int uv_flag,int num_bands);
void scale_adj(float *speech, float gain, float *prev_scale, int length, int sc_over);
float lin_int_bnd(float x,float xmin,float xmax,float ymin,float ymax);
void noise_est(float gain,float *noise_gain,float up,float down,float min,float max);
void noise_sup(float *gain,float noise_gain,float max_noise,float max_atten,float nfact);

