/**    
    @file voicegrain.h
    @brief 
    @author John Williamson
    
    Copyright (c) 2011 All rights reserved.
    Licensed under the BSD 3 clause license. See COPYING.
    
    This file is part of the OpenGrain distribution.
    http://opengrain.sourceforge.net   
*/              

#ifndef __VOICEGRAIN_H__
#define __VOICEGRAIN_H__

#include "audio.h"
#include "distributions.h"
#include "envelope.h"
#include "grain.h"
#include "biquad.h"
#include "grain_source.h"


// each structure always has a parameter structure
// and an active structure representing a specific instance of a grain
typedef struct VoiceGrainParameters
{
    Distribution *vowel;
    Distribution *noise;
    Distribution *vibrato;
    Distribution *vibrato_frequency;
    Distribution *frequency;
} VoiceGrainParameters;


typedef struct VoiceGrain
{
    Biquad *f[5];   
    Biquad *lowpass;
    float f_gain[5];
    float noise_level;    
    float vibrato_frequency;
    float vibrato;
    float vibrato_phase;    
    float frequency;    
    float phase;    
    float depth;
    
} VoiceGrain;


VoiceGrainParameters *create_voice_parameters(GrainSource *source);
void destroy_voice_parameters(VoiceGrainParameters *voicegrain);

Distribution * get_frequency_distribution_voice_parameters(VoiceGrainParameters *voicegrain);
Distribution * get_noise_distribution_voice_parameters(VoiceGrainParameters *voicegrain);
Distribution * get_vibrato_distribution_voice_parameters(VoiceGrainParameters *voicegrain);
Distribution * get_vibrato_frequency_distribution_voice_parameters(VoiceGrainParameters *voicegrain);
Distribution * get_vowel_distribution_voice_parameters(VoiceGrainParameters *voicegrain);

void *create_voicegrain(void *source);
void init_voicegrain(void *voicegrain, void *source, Grain *grain);
void destroy_voicegrain(void *voicegrain);
void fill_voicegrain(void *voicegrain, Buffer *buffer);



#endif