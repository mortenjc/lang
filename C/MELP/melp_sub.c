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

  melp_sub.c: MELP-specific subroutines

*/

#include <stdio.h>
#include <math.h>
#include "spbstd.h"
#include "mat.h"
#include "melp_sub.h"
#include "dsp_sub.h"
#include "pit.h"

/*
    Name: bpvc_ana.c
    Description: Bandpass voicing analysis
    Inputs:
      speech[] - input speech signal
      fpitch[] - initial (floating point) pitch estimates
    Outputs: 
      bpvc[] - bandpass voicing decisions
      pitch[] - frame pitch estimates
    Returns: void 

    Copyright (c) 1995 by Texas Instruments, Inc.  All rights reserved.
*/

/* Filter orders */
#define ENV_ORD 2
#define BPF_ORD 6

/* Constants */
static int PITCHMAX;
static int PITCHMIN;
static int FRAME;
static int NUM_BANDS;
static int PIT_BEG;
static int PIT_P_FR;
static int PIT_FR_BEG;
static int FIRST_CNTR;
static int BPF_BEG;
static int NUM_PITCHES;
static int LMIN;

/* Static memory */
static float **bpfdel;
static float **envdel;
static float *envdel2;
static float *sigbuf;

/* External variables */
extern float bpf_num[], bpf_den[];

void bpvc_ana(float speech[], float fpitch[], float bpvc[], float pitch[])
{

    float pcorr, temp;
    int j;
	
    /* Filter lowest band and estimate pitch */
    v_equ(&sigbuf[PIT_BEG-BPF_ORD],&bpfdel[0][0],BPF_ORD);
    polflt(&speech[PIT_FR_BEG],&bpf_den[0],&sigbuf[PIT_BEG],
	   BPF_ORD,PIT_P_FR);
    v_equ(&bpfdel[0][0],&sigbuf[PIT_BEG+FRAME-BPF_ORD],BPF_ORD);
    zerflt(&sigbuf[PIT_BEG],&bpf_num[0],&sigbuf[PIT_BEG],
	   BPF_ORD,PIT_P_FR);
    
    *pitch = frac_pch(&sigbuf[FIRST_CNTR],
				&bpvc[0],fpitch[0],5,PITCHMIN,PITCHMAX,LMIN);
    
    for (j = 1; j < NUM_PITCHES; j++) {
	temp = frac_pch(&sigbuf[FIRST_CNTR],
			&pcorr,fpitch[j],5,PITCHMIN,PITCHMAX,LMIN);
	
	/* choose largest correlation value */
	if (pcorr > bpvc[0]) {
	    *pitch = temp;
	    bpvc[0] = pcorr; 
	}
	
    }
    
    /* Calculate bandpass voicing for frames */
    
    for (j = 1; j < NUM_BANDS; j++) {

	/* Bandpass filter input speech */
	v_equ(&sigbuf[PIT_BEG-BPF_ORD],&bpfdel[j][0],BPF_ORD);
	polflt(&speech[PIT_FR_BEG],&bpf_den[j*(BPF_ORD+1)],&sigbuf[PIT_BEG],
	       BPF_ORD,PIT_P_FR);
	v_equ(&bpfdel[j][0],&sigbuf[PIT_BEG+FRAME-BPF_ORD],BPF_ORD);
	zerflt(&sigbuf[PIT_BEG],&bpf_num[j*(BPF_ORD+1)],&sigbuf[PIT_BEG],
	       BPF_ORD,PIT_P_FR);
	
	/* Check correlations for each frame */
	temp = frac_pch(&sigbuf[FIRST_CNTR],
			&bpvc[j],*pitch,0,PITCHMIN,PITCHMAX,LMIN);

	/* Calculate envelope of bandpass filtered input speech */
	temp = envdel2[j];
	envdel2[j] = sigbuf[PIT_BEG+FRAME-1];
	v_equ(&sigbuf[PIT_BEG-ENV_ORD],&envdel[j][0],ENV_ORD);
	envelope(&sigbuf[PIT_BEG],temp,&sigbuf[PIT_BEG],PIT_P_FR);
	v_equ(&envdel[j][0],&sigbuf[PIT_BEG+FRAME-ENV_ORD],ENV_ORD);
	
	/* Check correlations for each frame */
	temp = frac_pch(&sigbuf[FIRST_CNTR],&pcorr,
			*pitch,0,PITCHMIN,PITCHMAX,LMIN);
					
	/* reduce envelope correlation */
	pcorr -= 0.1;
	
	if (pcorr > bpvc[j])
	    bpvc[j] = pcorr;
    }
}

void bpvc_ana_init(int fr, int pmin, int pmax, int nbands, int num_p, int lmin)
{

    /* Initialize constants */
    FRAME = fr;
    PITCHMIN = pmin;
    PITCHMAX = pmax;
    NUM_BANDS = nbands;
    NUM_PITCHES = num_p;
    LMIN = lmin;
    PIT_BEG = BPF_ORD;
    PIT_P_FR = ((2*PITCHMAX)+1);
    PIT_FR_BEG = (-PITCHMAX);
    FIRST_CNTR = (PIT_BEG+PITCHMAX);
    BPF_BEG = (PIT_BEG+PIT_P_FR-FRAME);

    /* Allocate memory */
    MEM_2ALLOC(malloc,bpfdel,NUM_BANDS,BPF_ORD,float);
    v_zap(&bpfdel[0][0],NUM_BANDS*BPF_ORD);

    MEM_2ALLOC(malloc,envdel,NUM_BANDS,ENV_ORD,float);
    v_zap(&envdel[0][0],NUM_BANDS*ENV_ORD);
    MEM_ALLOC(malloc,envdel2,NUM_BANDS,float);
    v_zap(envdel2,NUM_BANDS);

    /* Allocate scratch buffer */
    MEM_ALLOC(malloc,sigbuf,PIT_BEG+PIT_P_FR,float);
}

/*
    Name: dc_rmv.c
    Description: remove DC from input signal
    Inputs: 
      sigin[] - input signal
      dcdel[] - filter delay history (size DC_ORD)
      frame - number of samples to filter
    Outputs: 
      sigout[] - output signal
      dcdel[] - updated filter delay history
    Returns: void 
    See_Also:

    Copyright (c) 1995 by Texas Instruments, Inc.  All rights reserved.
*/

/* Filter order */
#define DC_ORD 4

/* DC removal filter */
/* 4th order Chebychev Type II 60 Hz removal filter */
/* cutoff=60 Hz, stop=-30 dB */
static float dc_num[DC_ORD+1] = {    
      0.92692416,
     -3.70563834,
      5.55742893,
     -3.70563834,
      0.92692416};
static float dc_den[DC_ORD+1] = {
       1.00000000,
     -3.84610723,
      5.55209760,
     -3.56516069,
      0.85918839};

void dc_rmv(float sigin[], float sigout[], float dcdel[], int frame)
{
    float *sigbuf;

    /* Allocate scratch buffer */
    MEM_ALLOC(malloc,sigbuf,frame+DC_ORD,float);

    /* Remove DC from input speech */
    v_equ(sigbuf,dcdel,DC_ORD);
    polflt(sigin,dc_den,&sigbuf[DC_ORD],DC_ORD,frame);
    v_equ(dcdel,&sigbuf[frame],DC_ORD);
    zerflt(&sigbuf[DC_ORD],dc_num,sigout,DC_ORD,frame);

    /* Free scratch buffer */
    MEM_FREE(free,sigbuf);

}

/*
    Name: gain_ana.c
    Description: analyze gain level for input signal
    Inputs: 
      sigin[] - input signal
      pitch - pitch value (for pitch synchronous window)
      minlength - minimum window length
      maxlength - maximum window length
    Outputs: 
    Returns: log gain in dB
    See_Also:

    Copyright (c) 1995 by Texas Instruments, Inc.  All rights reserved.
*/

#define MINGAIN 0.0

float gain_ana(float sigin[], float pitch, int minlength, int maxlength)
{
    int length;
    float flength, gain;

    /* Find shortest pitch multiple window length (floating point) */
    flength = pitch;
    while (flength < minlength)
      flength += pitch;

    /* Convert window length to integer and check against maximum */
    length = flength + 0.5;
    if (length > maxlength)
      length = (length/2);
    
    /* Calculate RMS gain in dB */
    gain = 10.0*log10(0.01 + (v_magsq(&sigin[-(length/2)],length) / length));
    if (gain < MINGAIN)
      gain = MINGAIN;

    return(gain);

}

/*
    Name: lin_int_bnd.c
    Description: Linear interpolation within bounds
    Inputs:
      x - input X value
      xmin - minimum X value
      xmax - maximum X value
      ymin - minimum Y value
      ymax - maximum Y value
    Returns: y - interpolated and bounded y value

    Copyright (c) 1995 by Texas Instruments, Inc.  All rights reserved.
*/

float lin_int_bnd(float x,float xmin,float xmax,float ymin,float ymax)

{
    float y;
			
    if (x <= xmin)
      y = ymin;

    else if (x >= xmax)
      y = ymax;

    else 
      y = ymin + (x-xmin)*(ymax-ymin)/(xmax-xmin);

    return(y);
}

/*
    Name: noise_est.c
    Description: Estimate long-term noise floor
    Inputs:
      gain - input gain (in dB)
      noise_gain - current noise gain estimate
      up - maximum up stepsize
      down - maximum down stepsize
      min - minimum allowed gain
      max - maximum allowed gain
    Outputs:
      noise_gain - updated noise gain estimate
    Returns: void

    Copyright (c) 1995 by Texas Instruments, Inc.  All rights reserved.
*/

void noise_est(float gain,float *noise_gain,float up,float down,float min,float max)

{

    /* Update noise_gain */
    if (gain > *noise_gain+up)
      *noise_gain = *noise_gain+up;

    else if (gain < *noise_gain+down)
      *noise_gain = *noise_gain+down;

    else
      *noise_gain = gain;

    /* Constrain total range of noise_gain */
    if (*noise_gain < min)
      *noise_gain = min;
    if (*noise_gain > max)
      *noise_gain = max;
		
}

/*
    Name: noise_sup.c
    Description: Perform noise suppression on speech gain
    Inputs: (all in dB)
      gain - input gain (in dB)
      noise_gain - current noise gain estimate (in dB)
      max_noise - maximum allowed noise gain 
      max_atten - maximum allowed attenuation
      nfact - noise floor boost
    Outputs:
      gain - updated gain 
    Returns: void

    Copyright (c) 1995 by Texas Instruments, Inc.  All rights reserved.
*/

void noise_sup(float *gain,float noise_gain,float max_noise,float max_atten,float nfact)

{
    float gain_lev,suppress;
			
    /* Reduce effect for louder background noise */
    if (noise_gain > max_noise)
      noise_gain = max_noise;

    /* Calculate suppression factor */
    gain_lev = *gain - (noise_gain + nfact);
    if (gain_lev > 0.001) {
	suppress = -10.0*log10(1.0 - pow(10.0,-0.1*gain_lev));
	if (suppress > max_atten)
	  suppress = max_atten;
    }
    else
      suppress = max_atten;

    /* Apply suppression to input gain */
    *gain -= suppress;

}

/*
    Name: q_bpvc.c, q_bpvc_dec.c
    Description: Quantize/decode bandpass voicing
    Inputs:
      bpvc, bpvc_index
      bpthresh - threshold
      NUM_BANDS - number of bands
    Outputs: 
      bpvc, bpvc_index
    Returns: uv_flag - flag if unvoiced

    Copyright (c) 1995 by Texas Instruments, Inc.  All rights reserved.
*/

/* Compile constants */
#define INVALID_BPVC 0001

int q_bpvc(float *bpvc,int *bpvc_index,float bpthresh,int NUM_BANDS)

{
    int j, uv_flag;
	
    uv_flag = 1;

    if (bpvc[0] > bpthresh) {
		
	/* Voiced: pack bandpass voicing */
	uv_flag = 0;
	*bpvc_index = 0;
	bpvc[0] = 1.0;
	
	for (j = 1; j < NUM_BANDS; j++) {
	    *bpvc_index <<= 1; /* left shift */
	    if (bpvc[j] > bpthresh) {
		bpvc[j] = 1.0;
		*bpvc_index |= 1;
	    }
	    else {
		bpvc[j] = 0.0;
		*bpvc_index |= 0;
	    }
	}
	
	/* Don't use invalid code (only top band voiced) */
	if (*bpvc_index == INVALID_BPVC) {
	    bpvc[(NUM_BANDS-1)] = 0.0;
	    *bpvc_index = 0;
	}
    }
    
    else {
	
	/* Unvoiced: force all bands unvoiced */
	*bpvc_index = 0;
	for (j = 0; j < NUM_BANDS; j++)
	    bpvc[j] = 0.0;
    }

    return(uv_flag);
}

void q_bpvc_dec(float *bpvc,int *bpvc_index,int uv_flag,int NUM_BANDS)

{
    int j;

    if (uv_flag) {
	
	/* Unvoiced: set all bpvc to 0 */
	*bpvc_index = 0;
	bpvc[0] = 0.0;
    }
    
    else {
	
	/* Voiced: set bpvc[0] to 1.0 */
	bpvc[0] = 1.0;
    }
    
    if (*bpvc_index == INVALID_BPVC) {

	/* Invalid code received: set higher band voicing to zero */
	*bpvc_index = 0;
    }

    /* Decode remaining bands */
    for (j = NUM_BANDS-1; j > 0; j--) {
	if ((*bpvc_index & 1) == 1)
	    bpvc[j] = 1.0;
	else
	    bpvc[j] = 0.0;
	*bpvc_index >>= 1;
    }
}

/*
    Name: q_gain.c, q_gain_dec.c
    Description: Quantize/decode two gain terms using quasi-differential coding
    Inputs:
      gain[2],gain_index[2]
      GN_QLO,GN_QUP,GN_QLEV for second gain term
    Outputs: 
      gain[2],gain_index[2]
    Returns: void

    Copyright (c) 1995 by Texas Instruments, Inc.  All rights reserved.
*/

/* Compile constants */
#define GAIN_INT_DB 5.0

void q_gain(float *gain,int *gain_index,float GN_QLO,float GN_QUP,float GN_QLEV)

{
    static float prev_gain = 0.0;
    float temp,temp2;

    /* Quantize second gain term with uniform quantizer */
    quant_u(&gain[1],&gain_index[1],GN_QLO,GN_QUP,GN_QLEV);
    
    /* Check for intermediate gain interpolation */
    if (gain[0] < GN_QLO)
      gain[0] = GN_QLO;
    if (gain[0] > GN_QUP)
      gain[0] = GN_QUP;
    if (fabs(gain[1] - prev_gain) < GAIN_INT_DB && 
	fabs(gain[0] - 0.5*(gain[1]+prev_gain)) < 3.0) {

	/* interpolate and set special code */
	gain[0] = 0.5*(gain[1]+prev_gain);
	gain_index[0] = 0;
    }
    else {

	/* Code intermediate gain with 7-levels */
	if (prev_gain < gain[1]) {
	    temp = prev_gain;
	    temp2 = gain[1];
	}
	else {
	    temp = gain[1];
	    temp2 = prev_gain;
	}
	temp -= 6.0;
	temp2 += 6.0;
	if (temp < GN_QLO)
	  temp = GN_QLO;
	if (temp2 > GN_QUP)
	  temp2 = GN_QUP;
	quant_u(&gain[0],&gain_index[0],temp,temp2,7);

	/* Skip all-zero code */
	gain_index[0]++;
    }

    /* Update previous gain for next time */
    prev_gain = gain[1];
    
}

void q_gain_dec(float *gain,int *gain_index,float GN_QLO,float GN_QUP,float GN_QLEV)
{

    static float prev_gain = 0.0;
    static int prev_gain_err = 0;
    float temp,temp2;

    /* Decode second gain term */
    quant_u_dec(gain_index[1],&gain[1],GN_QLO,GN_QUP,GN_QLEV);
    
    if (gain_index[0] == 0) {

	/* interpolation bit code for intermediate gain */
	if (fabs(gain[1] - prev_gain) > GAIN_INT_DB) {
	    /* Invalid received data (bit error) */
	    if (prev_gain_err == 0) {
		/* First time: don't allow gain excursion */
		gain[1] = prev_gain;
	    }
	    prev_gain_err = 1;
	}
	else 
	  prev_gain_err = 0;

	/* Use interpolated gain value */
	gain[0] = 0.5*(gain[1]+prev_gain);
    }

    else {

	/* Decode 7-bit quantizer for first gain term */
	prev_gain_err = 0;
	gain_index[0]--;
	if (prev_gain < gain[1]) {
	    temp = prev_gain;
	    temp2 = gain[1];
	}
	else {
	    temp = gain[1];
	    temp2 = prev_gain;
	}
	temp -= 6.0;
	temp2 += 6.0;
	if (temp < GN_QLO)
	  temp = GN_QLO;
	if (temp2 > GN_QUP)
	  temp2 = GN_QUP;
	quant_u_dec(gain_index[0],&gain[0],temp,temp2,7);
    }

    /* Update previous gain for next time */
    prev_gain = gain[1];
    
}

/*
    Name: scale_adj.c
    Description: Adjust scaling of output speech signal.
    Inputs:
      speech - speech signal
      gain - desired RMS gain
      prev_scale - previous scale factor
      length - number of samples in signal
      SCALEOVER - number of points to interpolate scale factor
    Warning: SCALEOVER is assumed to be less than length.
    Outputs: 
      speech - scaled speech signal
      prev_scale - updated previous scale factor
    Returns: void

    Copyright (c) 1995 by Texas Instruments, Inc.  All rights reserved.
*/

void scale_adj(float *speech, float gain, float *prev_scale, int length, int SCALEOVER)

{
    int i;
    float scale;

    /* Calculate desired scaling factor to match gain level */
    scale = gain / (sqrt(v_magsq(&speech[0],length) / length) + .01);

    /* interpolate scale factors for first SCALEOVER points */
    for (i = 1; i < SCALEOVER; i++) {
	speech[i-1] *= ((scale*i + *prev_scale*(SCALEOVER-i))
			      * (1.0/SCALEOVER) );
    }
    
    /* Scale rest of signal */
    v_scale(&speech[SCALEOVER-1],scale,length-SCALEOVER+1);

    /* Update previous scale factor for next call */
    *prev_scale = scale;
			
}
