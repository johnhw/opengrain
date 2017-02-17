/**    
    @file noisegrain.h
    @brief
    @author John Williamson
    
    Copyright (c) 2011 All rights reserved.
    Licensed under the BSD 3 clause license. See COPYING.
    
    This file is part of the OpenGrain distribution.
    http://opengrain.sourceforge.net   
*/              

#ifndef __NOISEGRAIN_H__
#define __NOISEGRAIN_H__

#include "audio.h"
#include "distributions.h"
#include "biquad.h"
#include "envelope.h"
#include "grain.h"
#include "grain_source.h"


// each structure always has a parameter structure
// and an active structure representing a specific instance of a grain
typedef struct NoiseGrainParameters
{
    void *parent; // parent source data    
    Distribution *frequency;
    Distribution *q;
    int filtered;    
} NoiseGrainParameters;

typedef struct NoiseGrain
{
    float frequency;
    float q;
    Biquad *resonator;
    
} NoiseGrain;



NoiseGrainParameters *create_noise_parameters(GrainSource *source);
void destroy_noise_parameters(NoiseGrainParameters *noisegrain);
Distribution * get_frequency_distribution_noise_parameters(NoiseGrainParameters *noisegrain);
Distribution * get_q_distribution_noise_parameters(NoiseGrainParameters *noisegrain);
void set_filtered_noisegrain(NoiseGrainParameters *noisegrain, int filtered);

void *create_noisegrain(void *source);
void init_noisegrain(void *sinegrain, void *source, Grain *grain);
void destroy_noisegrain(void *sinegrain);
void fill_noisegrain(void *sinegrain, Buffer *buffer);



#endif