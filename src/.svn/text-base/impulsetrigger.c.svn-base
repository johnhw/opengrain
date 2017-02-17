/**    
    @file impulsetrigger.c
    @brief
    @author John Williamson
    
    Copyright (c) 2011 All rights reserved.
    Licensed under the BSD 3 clause license. See COPYING.
    
    This file is part of the OpenGrain distribution.
    http://opengrain.sourceforge.net   
*/              

#include "impulsetrigger.h"

#define E_RECIPROCAL 0.367879441

// create an uninitialized trigger object
ImpulseTrigger *create_impulse_trigger(void)
{
    ImpulseTrigger *trigger;
    trigger = malloc(sizeof(*trigger));
    trigger->power_tracker = create_RMS();
    trigger->deriv_tracker = create_RMS();
    trigger->bpf = create_biquad();    
    trigger->inhibit = 0;
    trigger->last_power = 0.0;
    set_inhibit_impulse_trigger(trigger, 0.05);
    trigger->copy_amplitude = 1;
    trigger->copy_boost = 0.0;
    return trigger;
}

// set whether or not incoming levels are used for the amplitude
void set_copy_amplitude_impulse_trigger(ImpulseTrigger *trigger, int copy_amplitude, float copy_boost)
{
    trigger->copy_amplitude = copy_amplitude;
    trigger->copy_boost = copy_boost;
}

// free an impulse trigger
void destroy_impulse_trigger(ImpulseTrigger *trigger)
{
    destroy_biquad(trigger->bpf);
    destroy_RMS(trigger->power_tracker);
    destroy_RMS(trigger->deriv_tracker);
    free(trigger);
}


// set the filter for the impulse trigger (bandpass filter)
void set_filter_impulse_trigger(ImpulseTrigger *trigger, float freq, float q)
{
    biquad_bandpass(trigger->bpf, freq, q);
}


// set the time in which another event cannot occur
void set_inhibit_impulse_trigger(ImpulseTrigger *trigger, float time)
{
    trigger->inhibit_time = time;
    trigger->inhibit_coeff = exp(-1.0 / (time * GLOBAL_STATE.sample_rate));
}


// set the threshold and the speed of threshold tracking
void set_threshold_impulse_trigger(ImpulseTrigger *trigger, float threshold, float speed)
{
    trigger->threshold = threshold;
    set_time_RMS(trigger->power_tracker, speed);
}

// set the threshold and the speed of derivative tracking
void set_deriv_threshold_impulse_trigger(ImpulseTrigger *trigger, float deriv_threshold, float deriv_speed)
{
    trigger->deriv_threshold = deriv_threshold;
    set_time_RMS(trigger->deriv_tracker, deriv_speed);
}


// process a buffer, triggering grains as they occur
void process_impulse_trigger(void *impulse_trigger, Buffer *input, GrainStream *stream)
{
    int i;
    float v, power, dBrate, smooth_rate;
    double level;
    ImpulseTrigger *trigger;
    trigger = (ImpulseTrigger *)impulse_trigger;
    for(i=0;i<input->n_samples;i++)
    {
        // filter and get envelope
        v = process_biquad(trigger->bpf, input->x[i]);
        update_RMS(trigger->power_tracker, v);
        power = compute_RMS(trigger->power_tracker);
        
        // compute dB/second
        dBrate = gain_to_dB(power/(trigger->last_power+1e-6)) * GLOBAL_STATE.sample_rate / 1000.0;
        if(dBrate<0)
            dBrate = 0;
            
        update_RMS(trigger->deriv_tracker, dBrate);
        smooth_rate = compute_RMS(trigger->deriv_tracker);
        trigger->last_power = power;
        
        level = gain_to_dB(power);
        
        // decay inhibit indicator
        trigger->inhibit = trigger->inhibit * trigger->inhibit_coeff;
        
        // trigger!
        if(level > trigger->threshold && smooth_rate > trigger->deriv_threshold && trigger->inhibit < E_RECIPROCAL)
        {
            
            trigger->inhibit = 1.0;
            if(trigger->copy_amplitude)
            {
                level += trigger->copy_boost; // allow cutting/amplifying incoming levels
                
                add_temporary_sequence_distribution(stream->model->amplitude, &level,  1);
            }
            trigger_single_grain_stream(stream, i/(double)GLOBAL_STATE.sample_rate);
        }    
    }    
}

