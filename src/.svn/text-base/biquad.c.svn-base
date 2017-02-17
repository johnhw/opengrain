/**    
    @file biquad.c
    @brief
    @author John Williamson
    
    Copyright (c) 2011 All rights reserved.
    Licensed under the BSD 3 clause license. See COPYING.
    
    This file is part of the OpenGrain distribution.
    http://opengrain.sourceforge.net   
*/              

#include "biquad.h"
#include <math.h>

// Create an empty biquad filter.
// MUST initialised with one of the filter design
// functions below before use!
Biquad *create_biquad()
{
    Biquad *biquad = malloc(sizeof(*biquad));
    
    biquad->x1 = 0;
    biquad->x2 = 0;
    biquad->y1 = 0;
    biquad->y2 = 0;
    return biquad;
}


// Destroy a biquad object
void destroy_biquad(Biquad *biquad)
{
    free(biquad);
}


// reset a biquad object (eliminate transients!)
void reset_biquad(Biquad *biquad)
{
    biquad->x1 = 0;
    biquad->x2 = 0;
    biquad->y1 = 0;
    biquad->y2 = 0; 
}

// set a biquad, using the type given
void set_biquad(Biquad *biquad, float freq, float q, int type)
{
    if(type==BIQUAD_LOWPASS)
        biquad_lowpass(biquad, freq, q);
    if(type==BIQUAD_HIGHPASS)
        biquad_highpass(biquad, freq, q);
    if(type==BIQUAD_BANDPASS)
        biquad_bandpass(biquad, freq, q);
    if(type==BIQUAD_ALLPASS)
        biquad_allpass(biquad, freq, q);
    if(type==BIQUAD_NOTCH)
        biquad_notch(biquad, freq, q);
}

// Pass a sample through a biquad filter
float process_biquad(Biquad *biquad, float x)
{
    float y;    
    
    y = (biquad->b0 * x + biquad->b1 * biquad->x1 + biquad->b2 * biquad->x2 - biquad->a1 * biquad->y1 - biquad->a2 * biquad->y2)/biquad->a0;
    biquad->x2 = biquad->x1;
    biquad->x1 = x;
    biquad->y2 = biquad->y1;
    biquad->y1 = y;
    return y;
}


/*
 Biquad filter design based on RBJ's filter cookbook
 Provides lowpass, highpass, bandpass, allpass, peaking, lowshelf and highshelf filters
 Each function takes a biquad object, filter parameters, and fills in the biquad coefficients
*/
void biquad_lowpass(Biquad *biquad,  float freq, float q)
{
    float cw0, sw0, alpha;
    float w0 = 2*M_PI*(freq/GLOBAL_STATE.sample_rate);
    cw0 = cos(w0); 
    sw0 = sin(w0);
    alpha = sw0 / (2*q);
    
    biquad->b0 = (1-cw0)/2;
    biquad->b1 = (1-cw0);
    biquad->b2 = (1-cw0)/2;
    
    biquad->a0 = 1 + alpha;
    biquad->a1 = -2*cw0;
    biquad->a2 = 1-alpha;
    
   
}


void biquad_highpass(Biquad *biquad, float freq, float q)
{
    float cw0, sw0, alpha;
    float w0 = 2*M_PI*(freq/GLOBAL_STATE.sample_rate);
    cw0 = cos(w0); 
    sw0 = sin(w0);
    alpha = sw0 / (2*q);
    
    biquad->b0 = (1+cw0)/2;
    biquad->b1 = -(1+cw0);
    biquad->b2 = (1+cw0)/2;

    biquad->a0 = 1 + alpha;
    biquad->a1 = -2*cw0;
    biquad->a2 = 1-alpha;
    
}


void biquad_bandpass(Biquad *biquad, float freq, float q)
{
    float cw0, sw0, alpha;
    float w0 = 2*M_PI*(freq/GLOBAL_STATE.sample_rate);
    cw0 = cos(w0); 
    sw0 = sin(w0);
    alpha = sw0 / (2*q);
    
    biquad->b0 = q*alpha;
    biquad->b1 = 0;
    biquad->b2 = -q*alpha;

    
    biquad->a0 = 1 + alpha;
    biquad->a1 = -2*cw0;
    biquad->a2 = 1-alpha;

}



// bandpass filter, bandwitdh in Hz
void biquad_bandpass_bw(Biquad *biquad, float freq, float bw)
{
    float cw0, sw0, alpha;
    float w0 = 2*M_PI*(freq/GLOBAL_STATE.sample_rate);
       
    bw = rate_to_octaves((freq+bw)/freq)/2;
    

    cw0 = cos(w0); 
    sw0 = sin(w0);
    alpha = sin(w0)*sinh(log(2.0)/2.0 * bw * w0/sin(w0));
    
    biquad->b0 = sw0/2;
    biquad->b1 = 0;
    biquad->b2 = -sw0/2;

    
    biquad->a0 = 1 + alpha;
    biquad->a1 = -2*cw0;
    biquad->a2 = 1-alpha; 
}


void biquad_bandpass_unity(Biquad *biquad, float freq, float q)
{
    float cw0, sw0, alpha;
    float w0 = 2*M_PI*(freq/GLOBAL_STATE.sample_rate);
    cw0 = cos(w0); 
    sw0 = sin(w0);
    alpha = sw0 / (2*q);
    biquad->b0 = alpha;
    biquad->b1 = 0;
    biquad->b2 = -alpha;
    
    biquad->a0 = 1 + alpha;
    biquad->a1 = -2*cw0;
    biquad->a2 = 1-alpha;
    
}


void biquad_notch(Biquad *biquad, float freq, float q)
{
    float cw0, sw0, alpha;
    float w0 = 2*M_PI*(freq/GLOBAL_STATE.sample_rate);
    cw0 = cos(w0); 
    sw0 = sin(w0);
    alpha = sw0 / (2*q);
    
    biquad->b0 = 1;
    biquad->b1 = -2*cw0;
    biquad->b2 = 1;
    
    biquad->a0 = 1 + alpha;
    biquad->a1 = -2*cw0;
    biquad->a2 = 1-alpha;
    
}


void biquad_allpass(Biquad *biquad, float freq, float q)
{
    float cw0, sw0, alpha;
    float w0 = 2*M_PI*(freq/GLOBAL_STATE.sample_rate);
    cw0 = cos(w0); 
    sw0 = sin(w0);
    alpha = sw0 / (2*q);
    biquad->b0 = 1-alpha;
    biquad->b1 = -2*cw0;
    biquad->b2 = 1+alpha;
    
    biquad->a0 = 1 + alpha;
    biquad->a1 = -2*cw0;
    biquad->a2 = 1-alpha;
    
}


void biquad_peaking(Biquad *biquad, float freq, float shelf, float octaves)
{
    float cw0, sw0, alpha;
    float A;
    A = pow(10, shelf/40.0);
    float w0 = 2*M_PI*(freq/GLOBAL_STATE.sample_rate);
    cw0 = cos(w0); 
    sw0 = sin(w0);
    alpha = sw0 * sinh(M_LN2 / 2 * octaves * w0/sw0);
    
    biquad->a0 = 1 + alpha/A;
    biquad->a1 = -2*cw0;
    biquad->a2 = 1-alpha/A;
    
    biquad->b0 = 1+alpha*A;
    biquad->b1 = -2*cw0;
    biquad->b2 = 1-alpha*A;
}


void biquad_lowshelf(Biquad *biquad, float freq, float shelf, float octaves)
{
    float cw0, sw0, alpha, beta;
    float A;    
    A = pow(10, shelf/40.0);
    float w0 = 2*M_PI*(freq/GLOBAL_STATE.sample_rate);
    cw0 = cos(w0); 
    sw0 = sin(w0);
    
    alpha = sw0 * sinh(M_LN2 / 2 * octaves * w0/sw0);
    beta = sqrt(A)/octaves;
    
    
    biquad->a0 = (A+1) + (A-1)*cw0 + beta*sw0;
    biquad->a1 = -2*((A-1)+(A+1)*cw0);
    biquad->a2 = (A+1) + (A-1) * cw0 - beta*sw0;
    
    biquad->b0 = A*((A+1) - (A-1)*cw0 + beta*sw0);
    biquad->b1 = 2*A*((A-1) - (A+1)*cw0);
    biquad->b2 = A*((A+1) - (A-1) * cw0 - beta*sw0);
}

void biquad_highshelf(Biquad *biquad, float freq, float shelf, float octaves)
{
    float cw0, sw0, alpha, beta;
    float A;
    A = pow(10.0, shelf/40.0);
    float w0 = 2*M_PI*(freq/GLOBAL_STATE.sample_rate);
    cw0 = cos(w0); 
    sw0 = sin(w0);
    alpha = sw0 * sinh(M_LN2 / 2 * octaves * w0/sw0);
    beta = sqrt(A)/octaves;
    
    biquad->a0 = (A+1) - (A-1)*cw0 + beta*sw0;
    biquad->a1 = 2*((A-1)-(A+1)*cw0);
    biquad->a2 = (A+1) - (A-1) * cw0 - beta*sw0;
    
    biquad->b0 = A*((A+1) + (A-1)*cw0 + beta*sw0);
    biquad->b1 = -2*A*((A-1)+(A+1)*cw0);
    biquad->b2 = A*((A+1) + (A-1) * cw0 - beta*sw0);
        
}



// Unity gain narrowband resonator
void biquad_formant(Biquad *biquad, float freq, float q)
{
    q = 1.0-1.0/q;
    
    
    biquad->b0 = 1.0-q;
    biquad->b1 = 0.0;
    biquad->b2 = -q*(1.0-q);
    
    biquad->a0 = 1.0;
       
    biquad->a1 = -2 * q * cos((2*M_PI*freq)/GLOBAL_STATE.sample_rate);
    biquad->a2 = q * q;    
}




