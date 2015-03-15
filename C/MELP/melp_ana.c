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
    Name: melp_ana.c
    Description: MELP analysis
    Inputs:
      speech[] - input speech signal
    Outputs: 
      *par - MELP parameter structure
    Returns: void
*/

/* compiler include files */

#include <stdio.h>
#include <math.h>
#include "melp.h"
#include "spbstd.h"
#include "lpc.h"
#include "mat.h"
#include "vq.h"
#include "fs.h"
#include "pit.h"

/* compiler constants */

#define BEGIN 0
#define END 1
#define BWFACT 0.994
#define PDECAY 0.95
#define PEAK_THRESH 1.34
#define PEAK_THR2 1.6
#define SILENCE_DB 30.0
#define MAX_ORD LPF_ORD
#define FRAME_BEG (PITCHMAX-(FRAME/2))
#define FRAME_END (FRAME_BEG+FRAME)
#define PITCH_BEG (FRAME_END-PITCHMAX)
#define PITCH_FR ((2*PITCHMAX)+1)
#define IN_BEG (PITCH_BEG+PITCH_FR-FRAME)
#define SIG_LENGTH (LPF_ORD+PITCH_FR)

/* external memory references */
 
extern float win_cof[LPC_FRAME];
extern float lpf_num[LPF_ORD+1];
extern float lpf_den[LPF_ORD+1];
extern float msvq_cb[];
extern float fsvq_cb[];
extern int fsvq_weighted;

/* memory definitions */
 
static float sigbuf[SIG_LENGTH];
static float speech[IN_BEG+FRAME];
static float dcdel[DC_ORD];
static float lpfsp_del[LPF_ORD];
static float pitch_avg;
static float fpitch[2];
static struct msvq_param vq_par;  /* MSVQ parameters */
static struct msvq_param fs_vq_par;  /* Fourier series VQ parameters */
static float w_fs[NUM_HARM];

	
void melp_ana(float sp_in[],struct melp_param *par)
{

    int i;
    int begin;
    float sub_pitch;
    float temp,pcorr,bpthresh;
    float r[LPC_ORD+1],refc[LPC_ORD+1],lpc[LPC_ORD+1];
    float weights[LPC_ORD];
        
    /* Remove DC from input speech */
    dc_rmv(sp_in,&speech[IN_BEG],dcdel,FRAME);
    
    /* Copy input speech to pitch window and lowpass filter */
    v_equ(&sigbuf[LPF_ORD],&speech[PITCH_BEG],PITCH_FR);
    v_equ(sigbuf,lpfsp_del,LPF_ORD);
    polflt(&sigbuf[LPF_ORD],lpf_den,&sigbuf[LPF_ORD],LPF_ORD,PITCH_FR);
    v_equ(lpfsp_del,&sigbuf[FRAME],LPF_ORD);
    zerflt(&sigbuf[LPF_ORD],lpf_num,&sigbuf[LPF_ORD],LPF_ORD,PITCH_FR);
    
    /* Perform global pitch search at frame end on lowpass speech signal */
    /* Note: avoid short pitches due to formant tracking */
    fpitch[END] = find_pitch(&sigbuf[LPF_ORD+(PITCH_FR/2)],&temp,
			     (2*PITCHMIN),PITCHMAX,PITCHMAX);
    
    /* Perform bandpass voicing analysis for end of frame */
    bpvc_ana(&speech[FRAME_END], fpitch, &par->bpvc[0], &sub_pitch);
    
    /* Force jitter if lowest band voicing strength is weak */    
    if (par->bpvc[0] < VJIT)
	par->jitter = MAX_JITTER;
    else
	par->jitter = 0.0;
    
    /* Calculate LPC for end of frame */
    window(&speech[(FRAME_END-(LPC_FRAME/2))],win_cof,sigbuf,LPC_FRAME);
    autocorr(sigbuf,r,LPC_ORD,LPC_FRAME);
    lpc[0] = 1.0;
    lpc_schur(r,lpc,refc,LPC_ORD);
    lpc_bw_expand(lpc,lpc,BWFACT,LPC_ORD);
    
    /* Calculate LPC residual */
    zerflt(&speech[PITCH_BEG],lpc,&sigbuf[LPF_ORD],LPC_ORD,PITCH_FR);
        
    /* Check peakiness of residual signal */
    begin = (LPF_ORD+(PITCHMAX/2));
    temp = peakiness(&sigbuf[begin],PITCHMAX);
    
    /* Peakiness: force lowest band to be voiced  */
    if (temp > PEAK_THRESH) {
	par->bpvc[0] = 1.0;
    }
    
    /* Extreme peakiness: force second and third bands to be voiced */
    if (temp > PEAK_THR2) {
	par->bpvc[1] = 1.0;
	par->bpvc[2] = 1.0;
    }
		
    /* Calculate overall frame pitch using lowpass filtered residual */
    par->pitch = pitch_ana(&speech[FRAME_END], &sigbuf[LPF_ORD+PITCHMAX], 
			   sub_pitch,pitch_avg,&pcorr);
    bpthresh = BPTHRESH;
    
    /* Calculate gain of input speech for each gain subframe */
    for (i = 0; i < NUM_GAINFR; i++) {
	if (par->bpvc[0] > bpthresh) {

	    /* voiced mode: pitch synchronous window length */
	    temp = sub_pitch;
	    par->gain[i] = gain_ana(&speech[FRAME_BEG+(i+1)*GAINFR],
				    temp,MIN_GAINFR,2*PITCHMAX);
	}
	else {
	    temp = 1.33*GAINFR - 0.5;
	    par->gain[i] = gain_ana(&speech[FRAME_BEG+(i+1)*GAINFR],
				    temp,0,2*PITCHMAX);
	}
    }
    
    /* Update average pitch value */
    if (par->gain[NUM_GAINFR-1] > SILENCE_DB)
      temp = pcorr;
    else
      temp = 0.0;
    pitch_avg = p_avg_update(par->pitch, temp, VMIN);
    
    /* Calculate Line Spectral Frequencies */
    lpc_pred2lsp(lpc,par->lsf,LPC_ORD);
    
    /* Force minimum LSF bandwidth (separation) */
    lpc_clamp(par->lsf,BWMIN,LPC_ORD);
    
    /* Quantize MELP parameters to 2400 bps and generate bitstream */
    
    /* Quantize LSF's with MSVQ */
    vq_lspw(weights, &par->lsf[1], lpc, LPC_ORD);
    msvq_enc(&par->lsf[1], weights, &par->lsf[1], vq_par);
    par->msvq_index = vq_par.indices;
    
    /* Force minimum LSF bandwidth (separation) */
    lpc_clamp(par->lsf,BWMIN,LPC_ORD);
    
    /* Quantize logarithmic pitch period */
    /* Reserve all zero code for completely unvoiced */
    par->pitch = log10(par->pitch);
    quant_u(&par->pitch,&par->pitch_index,PIT_QLO,PIT_QUP,PIT_QLEV);
    par->pitch = pow(10.0,par->pitch);
    
    /* Quantize gain terms with uniform log quantizer	*/
    q_gain(par->gain, par->gain_index,GN_QLO,GN_QUP,GN_QLEV);
    
    /* Quantize jitter and bandpass voicing */
    quant_u(&par->jitter,&par->jit_index,0.0,MAX_JITTER,2);
    par->uv_flag = q_bpvc(&par->bpvc[0],&par->bpvc_index,bpthresh,
			  NUM_BANDS);
    
    /*	Calculate Fourier coefficients of residual signal from quantized LPC */
    fill(par->fs_mag,1.0,NUM_HARM);
    if (par->bpvc[0] > bpthresh) {
	lpc_lsp2pred(par->lsf,lpc,LPC_ORD);
	zerflt(&speech[(FRAME_END-(LPC_FRAME/2))],lpc,sigbuf,
	       LPC_ORD,LPC_FRAME);
	window(sigbuf,win_cof,sigbuf,LPC_FRAME);
	find_harm(sigbuf, par->fs_mag, par->pitch, NUM_HARM, LPC_FRAME);
    }
    
    /* quantize Fourier coefficients */
    /* pre-weight vector, then use Euclidean distance */
    window(&par->fs_mag[0],w_fs,&par->fs_mag[0],NUM_HARM);
    fsvq_enc(&par->fs_mag[0], &par->fs_mag[0], fs_vq_par);
    
    /* Set MELP indeces to point to same array */
    par->fsvq_index = fs_vq_par.indices;

    /* Update MSVQ information */
    par->msvq_stages = vq_par.num_stages;
    par->msvq_bits = vq_par.num_bits;

    /* Write channel bitstream */
    melp_chn_write(par);

    /* Update delay buffers for next frame */
    v_equ(&speech[0],&speech[FRAME],IN_BEG);
    fpitch[BEGIN] = fpitch[END];
}



/* 
 * melp_ana_init: perform initialization 
 */


void melp_ana_init()
{

    int j;

    bpvc_ana_init(FRAME,PITCHMIN,PITCHMAX,NUM_BANDS,2,MINLENGTH);
    pitch_ana_init(PITCHMIN,PITCHMAX,FRAME,LPF_ORD,MINLENGTH);
    p_avg_init(PDECAY,DEFAULT_PITCH,3);

    v_zap(speech,IN_BEG+FRAME);
    pitch_avg=DEFAULT_PITCH;
    fill(fpitch,DEFAULT_PITCH,2);
    v_zap(lpfsp_del,LPF_ORD);
	
    /* Initialize multi-stage vector quantization (read codebook) */
	
    vq_par.num_best = MSVQ_M;
    vq_par.num_stages = 4;
    vq_par.dimension = 10;

    /* 
     * Allocate memory for number of levels per stage and indices
     * and for number of bits per stage 
     */
 
    MEM_ALLOC(MALLOC,vq_par.num_levels,vq_par.num_stages,int);
    MEM_ALLOC(MALLOC,vq_par.indices,vq_par.num_stages,int);
    MEM_ALLOC(MALLOC,vq_par.num_bits,vq_par.num_stages,int);
	
    vq_par.num_levels[0] = 128;
    vq_par.num_levels[1] = 64;
    vq_par.num_levels[2] = 64;
    vq_par.num_levels[3] = 64;
	
    vq_par.num_bits[0] = 7;
    vq_par.num_bits[1] = 6;
    vq_par.num_bits[2] = 6;
    vq_par.num_bits[3] = 6;
	
    vq_par.cb = msvq_cb;
	
    /* Scale codebook to 0 to 1 */
    v_scale(vq_par.cb,(2.0/FSAMP),3200);

    /* Initialize Fourier magnitude vector quantization (read codebook) */
	
    fs_vq_par.num_best = 1;
    fs_vq_par.num_stages = 1;
    fs_vq_par.dimension = NUM_HARM;

    /* 
     * Allocate memory for number of levels per stage and indices
     * and for number of bits per stage 
     */
 
    MEM_ALLOC(MALLOC,fs_vq_par.num_levels,fs_vq_par.num_stages,int);
    MEM_ALLOC(MALLOC,fs_vq_par.indices,fs_vq_par.num_stages,int);
    MEM_ALLOC(MALLOC,fs_vq_par.num_bits,fs_vq_par.num_stages,int);

    fs_vq_par.num_levels[0] = FS_LEVELS;
    fs_vq_par.num_bits[0] = FS_BITS;
    fs_vq_par.cb = fsvq_cb;
	
    /* Initialize fixed MSE weighting and inverse of weighting */
	
    vq_fsw(w_fs, NUM_HARM, 60.0);
	
    /* Pre-weight codebook (assume single stage only) */
	
    if (fsvq_weighted == 0)
      {
	  fsvq_weighted = 1;
	  for (j = 0; j < fs_vq_par.num_levels[0]; j++)
	    window(&fs_vq_par.cb[j*NUM_HARM],w_fs,&fs_vq_par.cb[j*NUM_HARM],
		   NUM_HARM);
      }

}
