/**    
    @file sinegrain.h
    @brief
    @author John Williamson
    
    Copyright (c) 2011 All rights reserved.
    Licensed under the BSD 3 clause license. See COPYING.
    
    This file is part of the OpenGrain distribution.
    http://opengrain.sourceforge.net   
*/              

#ifndef __SINEGRAIN_H__
#define __SINEGRAIN_H__

#include "audio.h"
#include "distributions.h"
#include "envelope.h"
#include "grain.h"
#include "grain_source.h"


// each structure always has a parameter structure
// and an active structure representing a specific instance of a grain
typedef struct SineGrainParameters
{
    void *parent; // parent source data
    Distribution *frequency;
} SineGrainParameters;

typedef struct SineGrain
{
    float frequency;
    float phase;
    float phase_increment;
} SineGrain;



SineGrainParameters *create_sine_parameters(GrainSource *source);
void destroy_sine_parameters(SineGrainParameters *sinegrain);
Distribution * get_frequency_distribution_sine_parameters(SineGrainParameters *sinegrain);

void *create_sinegrain(void *source);
void init_sinegrain(void *sinegrain, void *source, Grain *grain);
void destroy_sinegrain(void *sinegrain);
void fill_sinegrain(void *sinegrain, Buffer *buffer);



#endif