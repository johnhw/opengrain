/**    
    @file loopgrain.h
    @brief
    @author John Williamson
    
    Copyright (c) 2011 All rights reserved.
    Licensed under the BSD 3 clause license. See COPYING.
    
    This file is part of the OpenGrain distribution.
    http://opengrain.sourceforge.net   
*/              

#ifndef __LOOPGRAIN_H__
#define __LOOPGRAIN_H__

#include "audio.h"
#include "distributions.h"
#include "biquad.h"
#include "envelope.h"
#include "grain.h"
#include "grain_source.h"
#include "wavereader.h"


// each structure always has a parameter structure
// and an active structure representing a specific instance of a grain
typedef struct LoopGrainParameters
{               
    list_t *attacks;               
    Distribution *crossfade_time;
    Distribution *rate;    
    Distribution *pitch;
    Distribution *source_selector;
} LoopGrainParameters;


typedef struct LoopGrain
{    
    list_t *sounds;
    WaveSound *a, *b;
    Distribution *source_selector;    
    float phase; 
    float phase_increment;        
} LoopGrain;


LoopGrainParameters *create_loop_parameters(GrainSource *source);

void add_sound_loop_parameters(LoopGrainParameters *loop_parameters, WaveSound *sound);
void add_bank_loop_parameters(LoopGrainParameters *loop_parameters, list_t *sound_list);
Distribution *get_crossfade_distribution_loop_parameters(LoopGrainParameters *loop_parameters);
Distribution *get_rate_distribution_loop_parameters(LoopGrainParameters *loop_parameters);
Distribution *get_pitch_distribution(LoopGrainParameters *loop_parameters);

void destroy_loop_parameters(LoopGrainParameters *loop_parameters);


void *create_loopgrain(void *source);
void init_loopgrain(void *loopgrain, void *source, Grain *grain);
void destroy_loopgrain(void *impulsegrain);
void fill_loopgrain(void *impulsegrain, Buffer *buffer);



#endif