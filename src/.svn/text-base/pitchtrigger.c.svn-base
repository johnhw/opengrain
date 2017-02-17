/**    
    @file pitchtrigger.c
    @brief
    @author John Williamson
    
    Copyright (c) 2011 All rights reserved.
    Licensed under the BSD 3 clause license. See COPYING.
    
    This file is part of the OpenGrain distribution.
    http://opengrain.sourceforge.net   
*/              

#include "pitchtrigger.h"
#include "complex_buffer.h"

// create an uninitialized trigger object
PitchTrigger *create_pitch_trigger(int window_size)
{
    PitchTrigger *trigger;
    trigger = malloc(sizeof(*trigger));
    trigger->fft = create_fft(window_size);
    
    // create the buffers
    trigger->complex_buffer = create_complex_buffer(window_size);
    trigger->hps_buffer = create_buffer(window_size/2);
    trigger->temp_buffer = create_buffer(window_size/2);    
    trigger->buffer = create_buffer(window_size);
    trigger->window_size = window_size;
    trigger->buffer_ptr = 0;
    
    // current tracking frequency
    trigger->frequency = 0.0;
    trigger->target_frequency = 0.0;    
    set_speed_pitch_trigger(trigger, 0.05);    
    trigger->phase = 0.0;
    
    trigger->power_tracker = create_RMS();
    set_time_RMS(trigger->power_tracker, 0.05);
    
    set_minimum_level_pitch_trigger(trigger, -20.0);
    trigger->copy_amplitude = 1;    
    trigger->copy_boost = 0.0;
    return trigger;
}




// set the minimum level (in dB) at which the trigger will operate
void set_minimum_level_pitch_trigger(PitchTrigger *trigger, float min_level)
{
    trigger->min_level = dB_to_gain(min_level);
}

// set whether or not incoming levels are used for the amplitude
void set_copy_amplitude_pitch_trigger(PitchTrigger *trigger, int copy_amplitude, float copy_boost)
{
    trigger->copy_amplitude = copy_amplitude;
    trigger->copy_boost = copy_boost;
}

// free an impulse trigger
void destroy_pitch_trigger(PitchTrigger *trigger)
{
    free(trigger);
}


// set the time, in seconds, to acquire a new pitch
void set_speed_pitch_trigger(PitchTrigger *trigger, float time)
{
    trigger->frequency_coeff = rc_time(time, -100.0);
}




// process a buffer, triggering grains as they occur
void process_pitch_trigger(void *pitch_trigger, Buffer *input, GrainStream *stream)
{
    int i;
    float frequency;
    int peak_index;
   
    PitchTrigger *trigger;
    trigger = (PitchTrigger *)pitch_trigger;
    for(i=0;i<input->n_samples;i++)
    {    
        // copy in data
        trigger->buffer->x[trigger->buffer_ptr] = input->x[i];                
        trigger->buffer_ptr++;
        
        // if one whole buffer, update pitch info
        if(trigger->buffer_ptr == trigger->window_size)
        {
            // do fft and harmonic product spectrum
            fft_buffer(trigger->fft, trigger->buffer, trigger->complex_buffer);
            
            harmonic_product_spectrum(trigger->hps_buffer, trigger->temp_buffer, trigger->complex_buffer, 4);
            
            // find the big peak
            peak_index = find_absolute_peak(trigger->hps_buffer);
            
            // compute frequency
            frequency = (peak_index / (double)trigger->window_size) * GLOBAL_STATE.sample_rate;
            
            trigger->target_frequency = frequency;
            
            // reset buffer ptr
            trigger->buffer_ptr = 0;
        }
        
        // slide frequency to the target
        trigger->frequency = trigger->frequency_coeff*trigger->frequency + (1-trigger->frequency_coeff)*trigger->target_frequency;                       
        trigger->phase++;
        
        // update RMS power
         update_RMS(trigger->power_tracker, input->x[i]);
        
        if(compute_RMS(trigger->power_tracker) > trigger->min_level && frequency>10.0 && frequency<5000.0)
        {            
            set_constant_distribution(stream->model->rate, trigger->frequency);
        }
        else
        {
          //  set_constant_distribution(stream->rate, 10);
        }
        
        /*
        // trigger grains
        if(trigger->phase >= GLOBAL_STATE.sample_rate / (trigger->frequency))
        {
            trigger->phase = 0;
            
            // generate a grain, if the level is high enough
            if(compute_RMS(trigger->power_tracker) > trigger->min_level)
            {                
                printf("%f\n", trigger->frequency);
                if(trigger->copy_amplitude)
                {
                    level += trigger->copy_boost ; // allow cutting/amplifying incoming levels               
                    add_temporary_sequence_distribution(stream->amplitude, &level,  1);
                }
                //add_grain_stream(stream, i);
                //trigger_single_grain_stream(stream, i/(double)GLOBAL_STATE.sample_rate);
            }
        }
        
        */
    }
}

