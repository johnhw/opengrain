/**    
    @file impulsetrigger.h
    @brief
    @author John Williamson
    
    Copyright (c) 2011 All rights reserved.
    Licensed under the BSD 3 clause license. See COPYING.
    
    This file is part of the OpenGrain distribution.
    http://opengrain.sourceforge.net   
*/              

#ifndef __IMPULSETRIGGER_H__
#define __IMPULSETRIGGER_H__
#include "audio.h"
#include "biquad.h"
#include "grain_stream.h"
#include "rms.h"

#include <stdlib.h>
#include <math.h>


typedef struct ImpulseTrigger
{
    Biquad *bpf;
    float threshold; // in dB
    float deriv_threshold; // in dB/millisecond    
    float inhibit_time; // in seconds
    float inhibit_coeff;
    RMS *power_tracker;
    RMS *deriv_tracker;    
    float last_power;
    float inhibit;
    int copy_amplitude;
    float copy_boost;
} ImpulseTrigger;

ImpulseTrigger *create_impulse_trigger(void);

void set_copy_amplitude_impulse_trigger(ImpulseTrigger *trigger, int copy_amplitude, float copy_boost);
void destroy_impulse_trigger(ImpulseTrigger *trigger);
void set_filter_impulse_trigger(ImpulseTrigger *trigger, float freq, float q);
void set_inhibit_impulse_trigger(ImpulseTrigger *trigger, float time);
void set_threshold_impulse_trigger(ImpulseTrigger *trigger, float threshold, float speed);
void set_deriv_threshold_impulse_trigger(ImpulseTrigger *trigger, float deriv_threshold, float deriv_speed);
void process_impulse_trigger(void *impulse_trigger, Buffer *input, GrainStream *stream);


#endif