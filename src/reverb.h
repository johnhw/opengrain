/**    
    @file reverb.h
    @brief An implementation of Jon Dattoro's reverb algorithm.
    See J. Dattoro, "Effect Design: Part 1: Reverberator and Other Filters" J. Audio Eng. Soc. 45:9 September 1997
    
    @author John Williamson
    
    Copyright (c) 2011 All rights reserved.
    Licensed under the BSD 3 clause license. See COPYING.
    
    This file is part of the OpenGrain distribution.
    http://opengrain.sourceforge.net   
*/              

#ifndef __REVERB_H__
#define __REVERB_H__
#include "audio.h"
#include "delayline.h"
#include "moddelayline.h"


/** @struct DattoroReverb A reverb structure, consisting of a predelay delayline and
    twelve delaylines which form a Dattoro reverb network, two of
    which are modulating, and a set of parameters giving the feedback
    for the various elements of the reverb network */
typedef struct DattoroReverb
{

    DelayLine *pre_delay;
    float bandwidth;
    float damping;
    float decay;
    float decay_diffusion_1;
    float decay_diffusion_2;
    float input_diffusion_1;
    float input_diffusion_2;
    
    float max_excursion_1;
    float max_excursion_2;
        
    DelayLine *delay_142;
    DelayLine *delay_379;
    DelayLine *delay_107;
    DelayLine *delay_277;
    ModDelayLine *delay_672;
    ModDelayLine *delay_908;
    DelayLine *delay_4453;
    DelayLine *delay_4217;
    DelayLine *delay_3720;
    DelayLine *delay_3163;
    DelayLine *delay_1800;
    DelayLine *delay_2656;
    
    float pre_sample;    
    float diffusion_sample_a;
    float diffusion_sample_b;
    
    
    
} DattoroReverb;

DattoroReverb *create_reverb();

void set_size_reverb(DattoroReverb *reverb, double factor);
void set_predelay_reverb(DattoroReverb *reverb, double predelay);
void set_bandwidth_reverb(DattoroReverb *reverb, double bandwidth);
void set_damping_reverb(DattoroReverb *reverb, double damping);
void set_decay_reverb(DattoroReverb *reverb, double decay);
void set_decay_diffusion_1_reverb(DattoroReverb *reverb, double decay_diffusion_1);
void set_decay_diffusion_2_reverb(DattoroReverb *reverb, double decay_diffusion_2);
void set_input_diffusion_1_reverb(DattoroReverb *reverb, double decay_diffusion_1);
void set_input_diffusion_2_reverb(DattoroReverb *reverb, double decay_diffusion_2);
void set_modulation_reverb(DattoroReverb *reverb, double modulation);


void destroy_reverb(DattoroReverb *reverb);
void set_default_reverb(DattoroReverb *reverb);
void compute_reverb(DattoroReverb *reverb, float l, float r, float *out_l, float *out_r);


#endif