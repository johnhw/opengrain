/**    
    @file mixer_api.h
    @brief Implements the various mixer controls for the OpenGrain system
    @author John Williamson
    
    Copyright (c) 2011 All rights reserved.
    Licensed under the BSD 3 clause license. See COPYING.
    
    This file is part of the OpenGrain distribution.
    http://opengrain.sourceforge.net   
*/             
#include "gr.h"

void init_mixer_api(void);

typedef struct MixerBinding
{
    float reverb_level;
    int reverb_mode;
    float reverb_room_size;
    float reverb_predelay;
    float reverb_diffuse_level;
    float reverb_decay_time;
    float reverb_diffusion;
    float reverb_modulation_frequency;
    float reverb_modulation_depth;
    float reverb_damping;
    float reverb_bandwidth;
    float widener_delay;
    float compressor_threshold;
    float compressor_ratio;
    float compressor_attack;
    float compressor_decay;
    float compressor_gain;
    
    float eq_low_freq, eq_low_level;
    float eq_high_freq, eq_high_level;
    float eq_peak1_freq, eq_peak1_level;
    float eq_peak2_freq, eq_peak2_level;
    float eq_peak3_freq, eq_peak3_level;
} MixerBinding;