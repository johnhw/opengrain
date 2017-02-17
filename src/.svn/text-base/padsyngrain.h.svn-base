/**    
    @file padsyngrain.h
    @brief
    @author John Williamson
    
    Copyright (c) 2011 All rights reserved.
    Licensed under the BSD 3 clause license. See COPYING.
    
    This file is part of the OpenGrain distribution.
    http://opengrain.sourceforge.net   
*/              

#ifndef __PADSYNGRAIN_H__
#define __PADSYNGRAIN_H__

#include "audio.h"
#include "distributions.h"
#include "envelope.h"
#include "grain.h"
#include "grain_source.h"
#include "wavereader.h"
#include "kiss_fftr.h"
#include "kiss_fft.h"

// each structure always has a parameter structure
// and an active structure representing a specific instance of a grain
typedef struct PadSynGrainParameters
{
    Distribution *pitch_shift; // in semitones    
    Buffer *pad_loop;    
    
} PadSynGrainParameters;

typedef struct PadSyn
{
    Buffer *pad_loop;
    float offset;
    float rate;       
} PadSynGrain;


PadSynGrainParameters *create_padsyn_parameters(GrainSource *source);
void destroy_padsyn_parameters(PadSynGrainParameters *padsyn_parameters);
Distribution * get_pitch_shift_distribution_padsyn_parameters(PadSynGrainParameters *padsyn_parameters);
void set_wave_padsyn_parameters(PadSynGrainParameters *padsyn_parameters, WaveSound *sound, int loop_size, float noise_level);



void *create_padsyngrain(void *source);
void init_padsyngrain(void *padsyngrain, void *source, Grain *grain);
void destroy_padsyngrain(void *padsyngrain);
void fill_padsyngrain(void *padsyngrain, Buffer *buffer);



#endif