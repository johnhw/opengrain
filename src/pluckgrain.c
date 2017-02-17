/**    
    @file pluckgrain.c
    @brief
    @author John Williamson
    
    Copyright (c) 2011 All rights reserved.
    Licensed under the BSD 3 clause license. See COPYING.
    
    This file is part of the OpenGrain distribution.
    http://opengrain.sourceforge.net   
*/              

#include "pluckgrain.h"






// create a sine grain parameter structure
PluckGrainParameters *create_pluck_parameters(GrainSource *source)
{
    PluckGrainParameters *pluckgrain;
    pluckgrain = malloc(sizeof(*pluckgrain));
    pluckgrain->frequency = create_distribution();
    pluckgrain->duration = create_distribution();
    pluckgrain->damping = create_distribution();
    pluckgrain->impulse = NULL;
    
    
    
    // defaults
    set_constant_distribution(pluckgrain->frequency, 440.0);
    set_constant_distribution(pluckgrain->duration, 1.0);
    set_constant_distribution(pluckgrain->damping, 13500.0);
    pluckgrain->duration_mode = TIME_MODE_RELATIVE;    
    
    return pluckgrain;
}



// destroy a pluckgrain parameter object
void destroy_pluck_parameters(PluckGrainParameters *pluckgrain)
{
    destroy_distribution(pluckgrain->frequency);
    free(pluckgrain);
}


// get the frequency distribution object of a PluckGrain parameter object
Distribution * get_frequency_distribution_pluck_parameters(PluckGrainParameters *pluckgrain)
{
    return pluckgrain->frequency;

}

// get the duration distribution
Distribution * get_duration_distribution_pluck_parameters(PluckGrainParameters *pluckgrain)
{
    return pluckgrain->duration;
}

// get the damping distribution (in Hz)
Distribution * get_damping_distribution_pluck_parameters(PluckGrainParameters *pluckgrain)
{
   return pluckgrain->damping;
}

// set the duration mode (either relative to the grain duration, or absolute time)
void set_duration_mode_pluck_parameters(PluckGrainParameters *pluckgrain, int mode)
{
    pluckgrain->duration_mode = mode;
}

// set the impulse for the plucking, or NULL for white noise pluck
void set_impulse_pluck_parameters(PluckGrainParameters *pluckgrain, WaveSound *impulse)
{
    pluckgrain->impulse = impulse;
}

void init_pluckgrain(void *pluckgrain, void *source, Grain *grain)
{
    PluckGrain *active;
    PluckGrainParameters *parent;
    float delay, duration;
    active = (PluckGrain *) pluckgrain;
    parent = (PluckGrainParameters *)source;
    
    // compute delay line length
    delay = 1.0 / sample_from_distribution(parent->frequency);
    
    // set the one-pole damping
    active->filter_coeff = exp((-2.0 * M_PI * sample_from_distribution(parent->damping)) / GLOBAL_STATE.sample_rate);
    
    // get the duration
    if(parent->duration_mode == TIME_MODE_RELATIVE)
        duration = sample_from_distribution(parent->duration) * grain->duration;
    else
        duration = sample_from_distribution(parent->duration);
        
    // set the feedback
    
    active->feedback = pow(1e-7, delay/duration);
    active->last_y = 0.0;
    
    if(parent->impulse)
        active->impulse = get_channel_wave_sound(parent->impulse, 0);
    else
        active->impulse = NULL;
        
    active->impulse_phase = 0;
            
            
    active->last_y = 0;
    set_thiran_allpass_delay(active->delay, delay);           
}


// create a new grain from a PluckGrain parent object
void *create_pluckgrain(void *source)
{
    PluckGrain *active;
    active = malloc(sizeof(*active));
    active->delay = create_thiran_allpass();
    return (void*)active;
}



// fill a buffer with a karplus-strong plucked string
void fill_pluckgrain(void *pluckgrain, Buffer *buffer)
{
    int i;
    float y;
    PluckGrain *active;
    active = (PluckGrain *)pluckgrain;
    
    // fill the line initially
    if(active->impulse_phase==0)
    {
        if(active->impulse==NULL)
        {
            for(i=0;i<active->delay->delay;i++)                       
                insert_thiran_allpass(active->delay, uniform_double() - 0.5);            
            active->impulse_phase = active->delay->delay;
        }
        else
        {
            for(i=0;i<active->delay->delay;i++)
            {
                if(active->impulse_phase < active->impulse->n_samples)
                    insert_thiran_allpass(active->delay, active->impulse->x[active->impulse_phase++]);
                else
                    insert_thiran_allpass(active->delay, 0);
            }
        }
    }
    
    
    for(i=0;i<buffer->n_samples;i++)
    {                   
    
        
        // delay and filter the sample
        y = get_thiran_allpass(active->delay);
        
        // add any remaining impulse
        
        // write out the value
        buffer->x[i] = y;
        
        if(active->impulse!=NULL && active->impulse_phase < active->impulse->n_samples)
            y += active->impulse->x[active->impulse_phase++];
        
        
        // damp and feedback 
        y = active->filter_coeff * active->last_y + (1-active->filter_coeff) * y;
        active->last_y = y;
        insert_thiran_allpass(active->delay, y*active->feedback);                       
    }

}

// destroy a sine grain object
void destroy_pluckgrain(void *pluckgrain)
{
    free((PluckGrain*)pluckgrain);

}