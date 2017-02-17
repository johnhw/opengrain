/**    
    @file glissgrain.h
    @brief
    @author John Williamson
    
    Copyright (c) 2011 All rights reserved.
    Licensed under the BSD 3 clause license. See COPYING.
    
    This file is part of the OpenGrain distribution.
    http://opengrain.sourceforge.net   
*/              

#ifndef __GLISSGRAIN_H__
#define __GLISSGRAIN_H__

#include "audio.h"
#include "distributions.h"
#include "envelope.h"
#include "grain.h"
#include "grain_source.h"

#define GLISS_MODE_RELATIVE_FREQUENCY 0
#define GLISS_MODE_ABSOLUTE_FREQUENCY 1


// each structure always has a parameter structure
// and an active structure representing a specific instance of a grain
typedef struct GlissGrainParameters
{
    
    Distribution *frequency;
    Distribution *frequency_target;
    Distribution *time;
    Distribution *noise;
    Distribution *noise_bandwidth;
    Distribution *noise_target;
    int time_mode;
    int frequency_mode;            
} GlissGrainParameters;

typedef struct GlissGrain
{
    float frequency, frequency_target;
    float frequency_coeff;
    float phase;
    float noise;
    float noise_target;
    float noise_prev, noise_int;
    float noise_bandwidth_coeff;
} GlissGrain;



GlissGrainParameters *create_gliss_parameters(GrainSource *source);
void destroy_gliss_parameters(GlissGrainParameters *glissgrain);
Distribution * get_frequency_distribution_gliss_parameters(GlissGrainParameters *glissgrain);
Distribution * get_frequency_target_distribution_gliss_parameters(GlissGrainParameters *glissgrain);
Distribution * get_time_distribution_gliss_parameters(GlissGrainParameters *glissgrain);
Distribution * get_noise_distribution_gliss_parameters(GlissGrainParameters *glissgrain);
Distribution * get_noise_bandwidth_distribution_gliss_parameters(GlissGrainParameters *glissgrain);
Distribution * get_noise_target_distribution_gliss_parameters(GlissGrainParameters *glissgrain);

void set_frequency_mode_gliss(GlissGrainParameters *glissgrain, int freq_mode);
void set_time_mode_gliss(GlissGrainParameters *glissgrain, int time_mode);

void *create_glissgrain(void *source);
void init_glissgrain(void *glissgrain, void *source, Grain *grain);
void destroy_glissgrain(void *glissgrain);
void fill_glissgrain(void *glissgrain, Buffer *buffer);



#endif