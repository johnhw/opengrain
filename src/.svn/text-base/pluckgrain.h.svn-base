/**    
    @file pluckgrain.h
    @brief
    @author John Williamson
    
    Copyright (c) 2011 All rights reserved.
    Licensed under the BSD 3 clause license. See COPYING.
    
    This file is part of the OpenGrain distribution.
    http://opengrain.sourceforge.net   
*/              

#ifndef __PLUCKGRAIN_H__
#define __PLUCKGRAIN_H__

#include "audio.h"
#include "distributions.h"
#include "envelope.h"
#include "grain.h"
#include "grain_source.h"
#include "wavereader.h"
#include "allpass.h"



// each structure always has a parameter structure
// and an active structure representing a specific instance of a grain
typedef struct PluckGrainParameters
{    
    Distribution *frequency;
    Distribution *damping;
    Distribution *duration;
    int duration_mode;
    WaveSound *impulse;    
} PluckGrainParameters;

typedef struct PluckGrain
{    
    float last_y;
    ThiranAllpass *delay;
    float filter_coeff;
    Buffer *impulse;    
    int impulse_phase;
    float feedback;
} PluckGrain;



PluckGrainParameters *create_pluck_parameters(GrainSource *source);
void destroy_pluck_parameters(PluckGrainParameters *pluckgrain);
Distribution * get_frequency_distribution_pluck_parameters(PluckGrainParameters *pluckgrain);
Distribution * get_duration_distribution_pluck_parameters(PluckGrainParameters *pluckgrain);
Distribution * get_damping_distribution_pluck_parameters(PluckGrainParameters *pluckgrain);
void set_duration_mode_pluck_parameters(PluckGrainParameters *pluckgrain, int mode);
void set_impulse_pluck_parameters(PluckGrainParameters *pluckgrain, WaveSound *impulse);


void *create_pluckgrain(void *source);
void init_pluckgrain(void *pluckgrain, void *source, Grain *grain);
void destroy_pluckgrain(void *pluckgrain);
void fill_pluckgrain(void *pluckgrain, Buffer *buffer);



#endif