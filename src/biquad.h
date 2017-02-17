/**    
    @file biquad.h
    @brief
    @author John Williamson
    
    Copyright (c) 2011 All rights reserved.
    Licensed under the BSD 3 clause license. See COPYING.
    
    This file is part of the OpenGrain distribution.
    http://opengrain.sourceforge.net   
*/              

#ifndef __BIQUAD_H__
#define __BIQUAD_H__
#include "audio.h"
#include <math.h>

#define BIQUAD_LOWPASS 0
#define BIQUAD_HIGHPASS 1
#define BIQUAD_BANDPASS 2
#define BIQUAD_NOTCH 3
#define BIQUAD_ALLPASS 4



typedef struct Biquad
{
    float a0, a1, a2, b0, b1, b2;
    float x1, x2, y1, y2;
} Biquad;

Biquad *create_biquad();
void destroy_biquad(Biquad *biquad);
void reset_biquad(Biquad *biquad);
void set_biquad(Biquad *biquad, float freq, float q, int type);

float process_biquad(Biquad *biquad, float sample);

void biquad_lowpass(Biquad *biquad,  float freq, float q);
void biquad_highpass(Biquad *biquad, float freq, float q);
void biquad_bandpass(Biquad *biquad, float freq, float q);
void biquad_bandpass_unity(Biquad *biquad, float freq, float q);
void biquad_notch(Biquad *biquad, float freq, float q);
void biquad_allpass(Biquad *biquad, float freq, float q);
void biquad_peaking(Biquad *biquad, float freq, float shelf,float octaves);
void biquad_lowshelf(Biquad *biquad, float freq, float shelf,float octaves);
void biquad_highshelf(Biquad *biquad, float freq, float shelf,float octaves);

void biquad_formant(Biquad *biquad, float freq, float q);


void biquad_bandpass_bw(Biquad *biquad, float freq, float bw);




#endif