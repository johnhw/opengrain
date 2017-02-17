/**    
    @file wavegrain.h
    @brief
    @author John Williamson
    
    Copyright (c) 2011 All rights reserved.
    Licensed under the BSD 3 clause license. See COPYING.
    
    This file is part of the OpenGrain distribution.
    http://opengrain.sourceforge.net   
*/              

#ifndef __WAVEGRAIN_H__
#define __WAVEGRAIN_H__

#include "audio.h"
#include "distributions.h"
#include "biquad.h"
#include "envelope.h"
#include "grain.h"
#include "grain_source.h"
#include "wavereader.h"


#define WAVEGRAIN_PHASE_REALTIME 0
#define WAVEGRAIN_PHASE_GRAINTIME 1


// each structure always has a parameter structure
// and an active structure representing a specific instance of a grain
typedef struct WaveGrainParameters
{       
    Distribution *phase; // 0.0--1.0 normalized
    
    Distribution *pitch_shift; // in semitones
    WaveSound *sound;
    int interpolate;
    float phase_rate; // 1.0 == original speed
    int phase_offset;    
    int phase_mode;
    
    float grain_phase;
    
} WaveGrainParameters;

typedef struct WaveGrain
{
    float rate;
    WaveSound *sound;
    float phase;
    int interpolate;
    
    
} WaveGrain;



WaveGrainParameters *create_wave_parameters(GrainSource *source);
void destroy_wave_parameters(WaveGrainParameters *wavegrain);


void restart_wave_parameters(WaveGrainParameters *wavegrain);
void set_rate_wave_parameters(WaveGrainParameters *wavegrain, float rate);
void set_phase_mode_wave_parameters(WaveGrainParameters *wavegrain, int mode);
void set_interpolation_wave_parameters(WaveGrainParameters *wavegrain, int interpolate);
Distribution * get_phase_distribution_wave_parameters(WaveGrainParameters *wavegrain);

Distribution * get_pitch_shift_distribution_wave_parameters(WaveGrainParameters *wavegrain);
void set_rate_wave_parameters(WaveGrainParameters *wavegrain, float rate);
void restart_wave_parameters(WaveGrainParameters *wavegrain);

void set_source_wave_parameters(WaveGrainParameters *wavegrain, WaveSound *sound);

void *create_wavegrain(void *source);
void init_wavegrain(void *wavegrain, void *source, Grain *grain);
void destroy_wavegrain(void *wavegrain);
void fill_wavegrain(void *sinegrain, Buffer *buffer);



#endif