/**    
    @file eq.h
    @brief
    @author John Williamson
    
    Copyright (c) 2011 All rights reserved.
    Licensed under the BSD 3 clause license. See COPYING.
    
    This file is part of the OpenGrain distribution.
    http://opengrain.sourceforge.net   
*/              

#ifndef __EQ_H__
#define __EQ_H__
#include "audio.h"
#include "biquad.h"
#include <math.h>

typedef struct StereoEQ
{
    Biquad *left_low, *right_low;
    Biquad *left_high, *right_high;
    Biquad *left_peak_1, *right_peak_1;
    Biquad *left_peak_2, *right_peak_2;
    Biquad *left_peak_3, *right_peak_3;
    
} StereoEQ;

StereoEQ *create_eq();
void destroy_eq(StereoEQ *eq);
void compute_eq(StereoEQ *eq, float l, float r, float *out_l, float *out_r);
void set_low_eq(StereoEQ *eq, float freq, float boostdB);
void set_high_eq(StereoEQ *eq, float freq, float boostdB);


void set_peak_eq_1(StereoEQ *eq, float freq, float boostdB);
void set_peak_eq_2(StereoEQ *eq, float freq, float boostdB);
void set_peak_eq_3(StereoEQ *eq, float freq, float boostdB);

#endif