/**    
    @file compressor.h
    @brief
    @author John Williamson
    
    Copyright (c) 2011 All rights reserved.
    Licensed under the BSD 3 clause license. See COPYING.
    
    This file is part of the OpenGrain distribution.
    http://opengrain.sourceforge.net   
*/              

#ifndef __COMPRESSOR_H__
#define __COMPRESSOR_H__
#include "audio.h"
#include "rms.h"
#include <stdlib.h>
#include <math.h>

typedef struct StereoCompressor
{
    float attack, decay;
    RMS *power_tracker_l, *power_tracker_r;
    float total_power;
    float triggered;
    float gain;
    float ratio, threshold;
    float attack_coeff, decay_coeff;
    float speed;
    float compress_gain;
    
} StereoCompressor;


StereoCompressor *create_compressor(void);
void destroy_compressor(StereoCompressor *compressor);
void set_compressor(StereoCompressor *compressor, float thresholddB, float ratio, float attack, float decay, float gaindB, float speed);

void set_speed_compressor(StereoCompressor *compressor, float speed);

void set_threshold_compressor(StereoCompressor *compressor, float thresholddB);
void set_ratio_compressor(StereoCompressor *compressor, float ratio);
void set_attack_compressor(StereoCompressor *compressor, float attack);
void set_decay_compressor(StereoCompressor *compressor, float decay);
void set_gain_compressor(StereoCompressor *compressor, float gaindB);
void compute_compressor(StereoCompressor *compressor, float l, float r, float *out_l, float *out_r);


#endif