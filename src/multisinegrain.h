/**    
    @file multisinegrain.h
    @brief
    @author John Williamson
    
    Copyright (c) 2011 All rights reserved.
    Licensed under the BSD 3 clause license. See COPYING.
    
    This file is part of the OpenGrain distribution.
    http://opengrain.sourceforge.net   
*/              

#ifndef __MULTISINEGRAIN_H__
#define __MULTISINEGRAIN_H__

#include "audio.h"
#include "distributions.h"
#include "envelope.h"
#include "grain.h"
#include "grain_source.h"


#define MULTISINEGRAIN_MODE_DECAY_RELATIVE

#define MAX_SINES 32

// each structure always has a parameter structure
// and an active structure representing a specific instance of a grain
typedef struct MultiSineGrainParameters
{    
    Distribution *frequency;
    Distribution *amplitude;
    Distribution *decay;
    Distribution *n_sines;
    int time_mode;
} MultiSineGrainParameters;

typedef struct MultiSineGrain
{

    float *frequencies;
    float *phases;
    float *phase_increments;
    float *levels;
    float *decay_coeffs;
    int n_sines;
} MultiSineGrain;



MultiSineGrainParameters *create_multisine_parameters(GrainSource *source);
void destroy_multisine_parameters(MultiSineGrainParameters *multisinegrain);
Distribution * get_frequency_distribution_multisine_parameters(MultiSineGrainParameters *multisinegrain);
Distribution * get_amplitude_distribution_multisine_parameters(MultiSineGrainParameters *multisinegrain);
Distribution * get_decay_distribution_multisine_parameters(MultiSineGrainParameters *multisinegrain);
Distribution * get_n_sines_distribution_multisine_parameters(MultiSineGrainParameters *multisinegrain);
void set_decay_mode(MultiSineGrainParameters *multisinegrain, int mode);


void *create_multisinegrain(void *source);
void init_multisinegrain(void *multisinegrain, void *source, Grain *grain);
void destroy_multisinegrain(void *multisinegrain);
void fill_multisinegrain(void *multisinegrain, Buffer *buffer);



#endif