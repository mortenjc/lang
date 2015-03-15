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

  pit_lib.c: pitch analysis subroutines 

*/

#include <stdio.h>
#include <math.h>
#include "spbstd.h"
#include "mat.h"
#include "melp_sub.h"
#include "dsp_sub.h"
#include "pit.h"

/*                                                                  */
/*  double_chk.c: check for pitch doubling                          */
/*  and also verify pitch multiple for short pitches.               */
/*                                                                  */
#define NUM_MULT 8

float double_chk(float sig_in[], float *pcorr, float pitch, float pdouble, int pmin, int pmax, int lmin)

{

    int mult;
    float corr,thresh;
    float temp_pit;

    pitch = frac_pch(sig_in,pcorr,pitch,0,pmin,pmax,lmin);
    thresh = pdouble * (*pcorr);

    /* Check pitch submultiples from shortest to longest */
    for (mult = NUM_MULT; mult >= 2; mult--) {
	temp_pit = pitch / mult;
	if (temp_pit >= pmin) {
	    temp_pit = frac_pch(sig_in,&corr,temp_pit,0,pmin,pmax,lmin);
	    double_ver(sig_in,&corr,temp_pit,pmin,pmax,lmin);
	    
	    /* stop if submultiple greater than threshold */
	    if (corr > thresh) {

		/* refine estimate one more time since previous window */
		/* may be off center slightly and temp_pit has moved */
		pitch = frac_pch(sig_in,pcorr,temp_pit,0,pmin,pmax,lmin);
		break;
	    }
	}
    }

    /* Verify pitch multiples for short pitches */
    double_ver(sig_in,pcorr,pitch,pmin,pmax,lmin);

    /* Return full floating point pitch value and correlation*/
    return(pitch);

}
#undef NUM_MULT


/*                                                                  */
/*  double_ver.c: verify pitch multiple for short pitches.          */
/*                                                                  */
#define SHORT_PITCH 30

void double_ver(float sig_in[], float *pcorr, float pitch, int pmin, int pmax, int lmin)

{

    int mult;
    float corr,temp_pit;

    /* Verify pitch multiples for short pitches */
    mult = 1;
    while (pitch*mult < SHORT_PITCH)
      mult++;

    if (mult > 1) {
	temp_pit = pitch * mult;
	temp_pit = frac_pch(sig_in,&corr,temp_pit,0,pmin,pmax,lmin);
    
	/* use smaller of two correlation values */
	if (corr < *pcorr)
	  *pcorr = corr;
    }

}
#undef SHORT_PITCH

/*                                                                  */
/*  find_pitch.c: Determine pitch value.                            */
/*                                                                  */

float find_pitch(float sig_in[], float *pcorr, int lower, int upper, 
		 int length)

{

    int i,cbegin,ipitch,even_flag;
    float corr,maxcorr;
    float c0_0,cT_T;

    /* Find beginning of correlation window centered on signal */
    ipitch = lower;
    maxcorr = 0.0;
    even_flag = 1;
    cbegin = - ((length+upper)/2);
    c0_0 = v_magsq(&sig_in[cbegin],length);
    cT_T = v_magsq(&sig_in[cbegin+upper],length);

    for (i = upper; i >= lower; i--) {
	/* calculate normalized crosscorrelation */
	corr  = v_inner(&sig_in[cbegin],&sig_in[cbegin+i],length);
	if (corr > 0.01)
	  corr = (corr*corr) / (c0_0*cT_T);

	/* check if current maximum value */
	if (corr > maxcorr) {
	    maxcorr = corr;
	    ipitch = i;
	}

	/* update for next iteration */
	if (even_flag) {
	    even_flag = 0;
	    c0_0 += (sig_in[cbegin+length]*sig_in[cbegin+length]);
	    c0_0 -= (sig_in[cbegin]*sig_in[cbegin]);
	    cbegin++;
	}
	else {
	    even_flag = 1;
	    cT_T += (sig_in[cbegin+i-1]*sig_in[cbegin+i-1]);
	    cT_T -= (sig_in[cbegin+i-1+length]*sig_in[cbegin+i-1+length]);
	}
	    
    }

    /* Return full floating point pitch value and correlation*/
    *pcorr = sqrt(maxcorr);
    return((float) ipitch);

}

/*
    Name: frac_pch.c
    Description: Determine fractional pitch.
    Inputs:
      sig_in - input signal
      fpitch - initial floating point pitch estimate
      range - range for local integer pitch search (0=none)
      pmin - minimum allowed pitch value
      pmax - maximum allowed pitch value
      lmin - minimum correlation length
    Outputs:
      pcorr - correlation at fractional pitch value
    Returns: fpitch - fractional pitch value

    Copyright (c) 1995 by Texas Instruments, Inc.  All rights reserved.
*/

#define MAXFRAC 2.0
#define MINFRAC -1.0

float frac_pch(float sig_in[], float *pcorr, float fpitch, int range, int pmin, int pmax, int lmin)

{

    int length,cbegin,lower,upper,ipitch;
    float c0_0,c0_T,c0_T1,cT_T,cT_T1,cT1_T1,c0_Tm1;
    float frac,frac1;
    float corr,denom;

    /* Perform local integer pitch search for better fpitch estimate */
    if (range > 0) {
	ipitch = fpitch + 0.5;
	lower = ipitch - range;
	upper = ipitch + range;
	if (upper > pmax)
	  upper = pmax;
	if (lower < pmin)
	  lower = pmin;
	if (lower < 0.75*ipitch)
	  lower = 0.75*ipitch;
	length = ipitch;
	if (length < lmin)
	  length = lmin;
	fpitch = find_pitch(sig_in,&corr,lower,upper,length);
    }

    /* Estimate needed crosscorrelations */
    ipitch = fpitch + 0.5;
    if (ipitch >= pmax)
      ipitch = pmax - 1;
    length = ipitch;
    if (length < lmin)
      length = lmin;
    cbegin = - ((length+ipitch)/2);
    c0_T = v_inner(&sig_in[cbegin],&sig_in[cbegin+ipitch],length);
    c0_T1 = v_inner(&sig_in[cbegin],&sig_in[cbegin+ipitch+1],length);
    c0_Tm1 = v_inner(&sig_in[cbegin],&sig_in[cbegin+ipitch-1],length);
    if (c0_Tm1 > c0_T1) {
	/* fractional component should be less than 1, so decrement pitch */
	c0_T1 = c0_T;
	c0_T = c0_Tm1;
	ipitch-- ;
    }
    cT_T1 = v_inner(&sig_in[cbegin+ipitch],&sig_in[cbegin+ipitch+1],length);
    c0_0 = v_inner(&sig_in[cbegin],&sig_in[cbegin],length);
    cT_T = v_inner(&sig_in[cbegin+ipitch],&sig_in[cbegin+ipitch],length);
    cT1_T1 = v_inner(&sig_in[cbegin+ipitch+1],&sig_in[cbegin+ipitch+1],length);

    /* Find fractional component of pitch within integer range */
    denom = c0_T1*(cT_T - cT_T1) + c0_T*(cT1_T1 - cT_T1);
    if (fabs(denom) > 0.01) 
      frac = (c0_T1*cT_T - c0_T*cT_T1) / denom;
    else
      frac = 0.5;
    if (frac > MAXFRAC)
      frac = MAXFRAC;
    if (frac < MINFRAC)
      frac = MINFRAC;

    /* Make sure pitch is still within range */
    fpitch = ipitch + frac;
    if (fpitch > pmax)
      fpitch = pmax;
    if (fpitch < pmin)
      fpitch = pmin;
    frac = fpitch - ipitch;    

    /* Calculate interpolated correlation strength */
    frac1 = 1.0 - frac;
    denom = c0_0*(frac1*frac1*cT_T + 2*frac*frac1*cT_T1 + frac*frac*cT1_T1);
    denom = sqrt(denom);
    if (fabs(denom) > 0.01)
      *pcorr = (frac1*c0_T + frac*c0_T1) / denom;
    else
      *pcorr = 0.0;

    /* Return full floating point pitch value */
    return(fpitch);

}
#undef MAXFRAC
#undef MINFRAC

/*
    Name: p_avg_update.c
    Description: Update pitch average value.
    Inputs:
      pitch - current pitch value
      pcorr - correlation strength at current pitch value
      pthresh - pitch correlation threshold
    Returns: pitch_avg - updated average pitch value

    Copyright (c) 1995 by Texas Instruments, Inc.  All rights reserved.
*/

/* Static constants */
static float PDECAY;
static float DEFAULT_PITCH;
static int NUM_GOOD;

/* Static data */
static float *good_pitch;

float p_avg_update(float pitch, float pcorr, float pthresh)

{
    int i;
    float pitch_avg;

    /* Strong correlation: update good pitch array */
    if (pcorr > pthresh) {
	for (i = NUM_GOOD-1; i >= 1; i--)
	  good_pitch[i] = good_pitch[i-1];
	good_pitch[0] = pitch;
    }
    
    /* Otherwise decay good pitch array to default value */
    else {
	for (i = 0; i < NUM_GOOD; i++)
	  good_pitch[i] = (PDECAY*good_pitch[i]) +((1.0-PDECAY)*DEFAULT_PITCH);
    }
    
    /* Pitch_avg = median of pitch values */
    pitch_avg = median(good_pitch,NUM_GOOD);

    return(pitch_avg);
}

void p_avg_init(float pdecay, float default_pitch, int num_good)

{

    /* Initialize constants */
    PDECAY = pdecay;
    DEFAULT_PITCH = default_pitch;
    NUM_GOOD = num_good;

    /* Allocate and initialize good pitch array */
    MEM_ALLOC(malloc,good_pitch,NUM_GOOD,float);
    fill(good_pitch,DEFAULT_PITCH,NUM_GOOD);

}

/*
    Name: pitch_ana.c
    Description: Pitch analysis
    Inputs:
      speech[] - input speech signal
      resid[] - LPC residual signal
      pitch_est - initial (floating point) pitch estimate
      pitch_avg - average pitch value
    Outputs: 
      pcorr2 - pitch correlation strength
    Returns: pitch - pitch estimate

    Copyright (c) 1995 by Texas Instruments, Inc.  All rights reserved.
*/

/* Compiler constants */
#define UVMAX 0.55
#define PDOUBLE1 0.75
#define PDOUBLE2 0.5
#define PDOUBLE3 0.9
#define PDOUBLE4 0.7
#define LONG_PITCH 100.0

/* Static constants */
static int PITCHMAX;
static int PITCHMIN;
static int FRAME;
static int LPF_ORD;
static int PITCH_FR;
static int LMIN;

/* External variables */
extern float lpf_num[], lpf_den[];

/* Static data */
static float *lpres_del;
static float *sigbuf;

float pitch_ana(float speech[], float resid[], float pitch_est, float pitch_avg, float *pcorr2)

{

    float temp, temp2, pcorr, pitch;

    /* Lowpass filter residual signal */
    v_equ(&sigbuf[LPF_ORD],&resid[-PITCHMAX],PITCH_FR);
    v_equ(sigbuf,lpres_del,LPF_ORD);
    polflt(&sigbuf[LPF_ORD],lpf_den,&sigbuf[LPF_ORD],LPF_ORD,PITCH_FR);
    v_equ(lpres_del,&sigbuf[FRAME],LPF_ORD);
    zerflt(&sigbuf[LPF_ORD],lpf_num,&sigbuf[LPF_ORD],LPF_ORD,PITCH_FR);

    /* Perform local search around pitch estimate */
    temp = frac_pch(&sigbuf[LPF_ORD+(PITCH_FR/2)],&pcorr,
		    pitch_est,5,PITCHMIN,PITCHMAX,LMIN);
    
    if (pcorr < 0.6) {

	/* If correlation is too low, try speech signal instead */
	v_equ(&sigbuf[LPF_ORD],&speech[-PITCHMAX],PITCH_FR);
	temp = frac_pch(&sigbuf[LPF_ORD+(PITCH_FR/2)],&pcorr,
			pitch_est,0,PITCHMIN,PITCHMAX,LMIN);

	if (pcorr < UVMAX)
		
	  /* If correlation still too low, use average pitch */
	  pitch = pitch_avg;

	else {
		
	    /* Else check for pitch doubling (speech thresholds) */
	    temp2 = PDOUBLE3;
	    if (temp > LONG_PITCH)
	      /* longer pitches are more likely to be doubles */
	      temp2 = PDOUBLE4;
	    pitch = double_chk(&sigbuf[LPF_ORD+(PITCH_FR/2)],&pcorr,
				    temp,temp2,PITCHMIN,PITCHMAX,LMIN);
	}
    }
    
    else {

	/* Else check for pitch doubling (residual thresholds) */
	temp2 = PDOUBLE1;
	if (temp > LONG_PITCH)
	
	  /* longer pitches are more likely to be doubles */
	  temp2 = PDOUBLE2;

	pitch = double_chk(&sigbuf[LPF_ORD+(PITCH_FR/2)],&pcorr,
				temp,temp2,PITCHMIN,PITCHMAX,LMIN);
    }
    
    if (pcorr < UVMAX) {
	
	/* If correlation still too low, use average pitch */
	pitch = pitch_avg;
    }
	
    /* Return pitch and set correlation strength */
    *pcorr2 = pcorr;
    return(pitch);

}

void pitch_ana_init(int pmin, int pmax, int fr, int lpf_ord, int lmin)

{

    /* Initialize constants */
    FRAME = fr;
    PITCHMIN = pmin;
    PITCHMAX = pmax;
    LPF_ORD = lpf_ord;
    LMIN = lmin;
    PITCH_FR = ((2*PITCHMAX)+1);

    /* Allocate and initialize delay memory */
    MEM_ALLOC(malloc,lpres_del,LPF_ORD,float);
    v_zap(lpres_del,LPF_ORD);

    /* Allocate scratch buffer */
    MEM_ALLOC(malloc,sigbuf,LPF_ORD+PITCH_FR,float);
}
