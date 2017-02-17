/**    
    @file grainmixer.h
    @brief
    @author John Williamson
    
    Copyright (c) 2011 All rights reserved.
    Licensed under the BSD 3 clause license. See COPYING.
    
    This file is part of the OpenGrain distribution.
    http://opengrain.sourceforge.net   
*/              

#ifndef __GRAINMIXER_H__
#define __GRAINMIXER_H__
#include "audio.h"
#include "reverb.h"
#include "random_reverb.h"
#include "grain_stream.h"
#include "eq.h"
#include "widener.h"
#include "compressor.h"


/** @def Reverb mode bit flag for enabling the standard Dattoro reverb */
#define MIXER_REVERB_DATTORO 1

/** @def Reverb mode bit flag for enabling the randomized Dattoro reverb */
#define MIXER_REVERB_RANDOM 2


/** @struct GrainMixer A grain mixer object which holds a number of streams
    and effects to apply to them. */
typedef struct GrainMixer
{    
    int n_streams;    
    StereoEQ *eq;
        
    RandomReverb *random_reverb;
    Widener *widener;
    StereoCompressor *compressor;
    float reverb_level;
    float again;
    float again_target;
    float again_coeff;
    
    float dB_gain;
    list_t *stream_list;
    
    int reverb_mode;
    int compressor_enabled;
    int widener_enabled;
    int reverb_enabled;
    int eq_enabled;
    int test_tone_enabled;    
    
    Biquad *diffuse_lowpass;
    Buffer *temp_left, *temp_right, *aux, *temp_aux;
    
} GrainMixer;



void test_tone(Buffer *left, Buffer *right);
GrainMixer *create_mixer();
void destroy_mixer(GrainMixer *mixer);

void add_stream(GrainMixer *mixer, GrainStream *stream);
void remove_stream(GrainMixer *mixer, GrainStream *stream);
void set_reverb_level_mixer(GrainMixer *mixer, float wet);
void set_gain_mixer(GrainMixer *mixer, double dBgain);
void fade_gain_mixer(GrainMixer *mixer, float dBgain, float time);

RandomReverb *get_random_reverb_mixer(GrainMixer *mixer);

StereoEQ *get_eq_mixer(GrainMixer *mixer);

void grain_mix(GrainMixer *mixer, Buffer *left, Buffer *right);

void enable_test_tone_mixer(GrainMixer *mixer);
void disable_test_tone_mixer(GrainMixer *mixer);

void enable_eq_mixer(GrainMixer *mixer);
void disable_eq_mixer(GrainMixer *mixer);

void enable_reverb_mixer(GrainMixer *mixer);
void disable_reverb_mixer(GrainMixer *mixer);

void enable_random_reverb_mixer(GrainMixer *mixer);
void disable_random_reverb_mixer(GrainMixer *mixer);

Widener *get_widener_mixer(GrainMixer *mixer);

void enable_widener_mixer(GrainMixer *mixer);
void disable_widener_mixer(GrainMixer *mixer);

StereoCompressor *get_compressor_mixer(GrainMixer *mixer);
void enable_compressor_mixer(GrainMixer *mixer);
void disable_compressor_mixer(GrainMixer *mixer);
#endif