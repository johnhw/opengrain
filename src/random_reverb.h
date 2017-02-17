/**    
    @file random_reverb.h
    @brief
    @author John Williamson
    
    Copyright (c) 2011 All rights reserved.
    Licensed under the BSD 3 clause license. See COPYING.
    
    This file is part of the OpenGrain distribution.
    http://opengrain.sourceforge.net   
*/              

#ifndef __RANDOM_REVERB_H__
#define __RANDOM_REVERB_H__
#include "audio.h"
#include "delayline.h"
#include "moddelayline.h"
#include "random.h"



typedef struct RandomReverb
{

    ModDelayLine *pre_delay;
    float bandwidth;
    float damping;
    float decay;
    float decay_diffusion_1;
    float decay_diffusion_2;
    float input_diffusion_1;
    float input_diffusion_2;
    
    float max_excursion_1;
    float max_excursion_2;
    
    
    ModDelayLine *delays[12];
    
    int *channel_taps;
    
    
    int modulations[12];
    float modulation_freqs[12];
    
    float modulation_extent;
    float size;
    float pre_sample;    
    float diffusion_sample_a;
    float diffusion_sample_b;
    float grit;
    int n_channels;
    int random_mode;
    
    
} RandomReverb;

RandomReverb *create_random_reverb(int n_channels);
void set_size_random_reverb(RandomReverb *reverb, double factor);
void set_random_mode_random_reverb(RandomReverb *reverb, int random_mode);

void set_predelay_random_reverb(RandomReverb *reverb, double predelay);
void set_bandwidth_random_reverb(RandomReverb *reverb, double bandwidth);
void set_damping_random_reverb(RandomReverb *reverb, double damping);
void set_decay_random_reverb(RandomReverb *reverb, double decay);
void set_decay_diffusion_1_random_reverb(RandomReverb *reverb, double decay_diffusion_1);
void set_decay_diffusion_2_random_reverb(RandomReverb *reverb, double decay_diffusion_2);
void set_input_diffusion_1_random_reverb(RandomReverb *reverb, double decay_diffusion_1);
void set_input_diffusion_2_random_reverb(RandomReverb *reverb, double decay_diffusion_2);
void set_modulation_random_reverb(RandomReverb *reverb, double modulation);


void destroy_random_reverb(RandomReverb *reverb);
void set_default_random_reverb(RandomReverb *reverb);
void compute_random_reverb(RandomReverb *reverb, Buffer **in_buffers, int n_in_channels, Buffer **out_buffers);


#endif