/**    
    @file impulsegrain.h
    @brief
    @author John Williamson
    
    Copyright (c) 2011 All rights reserved.
    Licensed under the BSD 3 clause license. See COPYING.
    
    This file is part of the OpenGrain distribution.
    http://opengrain.sourceforge.net   
*/              

#ifndef __IMPULSEGRAIN_H__
#define __IMPULSEGRAIN_H__

#include "audio.h"
#include "distributions.h"
#include "biquad.h"
#include "envelope.h"
#include "grain.h"
#include "grain_source.h"
#include "wavereader.h"


// each structure always has a parameter structure
// and an active structure representing a specific instance of a grain
typedef struct ImpulseGrainParameters
{               
    list_t *sounds;           
    int play_whole;
    int loop;
} ImpulseGrainParameters;

typedef struct ImpulseGrain
{    
    WaveSound *sound;
    int phase; 
    int n_samples;
    int loop;
    Grain *master_grain;
} ImpulseGrain;


ImpulseGrainParameters *create_impulse_parameters(GrainSource *source);
void add_sound_impulse_parameters(ImpulseGrainParameters *impulse_parameters, WaveSound *sound);
void add_sound_bank_impulse_parameters(ImpulseGrainParameters *impulse_parameters, list_t *sound_list);
void destroy_impulse_parameters(ImpulseGrainParameters *impulse_parameters);
void set_loop_impulse_parameters(ImpulseGrainParameters *impulse_parameters, int loop);

void *create_impulsegrain(void *source);
void init_impulsegrain(void *impulsegrain, void *source, Grain *grain);
void destroy_impulsegrain(void *mpulsegrain);
void fill_impulsegrain(void *mpulsegrain, Buffer *buffer);



#endif