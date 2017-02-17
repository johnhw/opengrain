/**    
    @file glissgrain.c
    @brief
    @author John Williamson
    
    Copyright (c) 2011 All rights reserved.
    Licensed under the BSD 3 clause license. See COPYING.
    
    This file is part of the OpenGrain distribution.
    http://opengrain.sourceforge.net   
*/              

#include "glissgrain.h"
#include <math.h>






// create a sine grain parameter structure
GlissGrainParameters *create_gliss_parameters(GrainSource *source)
{
    GlissGrainParameters *glissgrain;
    glissgrain = malloc(sizeof(*glissgrain));
    glissgrain->frequency = create_distribution();
    glissgrain->frequency_target = create_distribution();
    glissgrain->noise = create_distribution();
    glissgrain->noise_target = create_distribution();
    glissgrain->time = create_distribution();
    glissgrain->noise_bandwidth = create_distribution();
        
    // defaults
    set_constant_distribution(glissgrain->frequency, 440.0);
    set_constant_distribution(glissgrain->frequency_target, 440.0);
    set_constant_distribution(glissgrain->noise, 0);
    set_constant_distribution(glissgrain->noise_target, 0.0);
    set_constant_distribution(glissgrain->time, 1.0);
    set_constant_distribution(glissgrain->noise_bandwidth, 100.0);
    
    glissgrain->time_mode = TIME_MODE_RELATIVE;
    glissgrain->frequency_mode = GLISS_MODE_ABSOLUTE_FREQUENCY;
    
    return glissgrain;
}



// destroy a glissgrain parameter object
void destroy_gliss_parameters(GlissGrainParameters *glissgrain)
{
    destroy_distribution(glissgrain->frequency);
    destroy_distribution(glissgrain->frequency_target);
    destroy_distribution(glissgrain->time);
    destroy_distribution(glissgrain->noise);
    destroy_distribution(glissgrain->noise_target);
    destroy_distribution(glissgrain->noise_bandwidth);
    free(glissgrain);
    
}

// set the frequency mode for this parameter set (relative or absolute)
void set_frequency_mode_gliss(GlissGrainParameters *glissgrain, int freq_mode)
{
    glissgrain->frequency_mode = freq_mode;
}   


// set the time mode for this parameter set (relative or absolute)
void set_time_mode_gliss(GlissGrainParameters *glissgrain, int time_mode)
{
    glissgrain->time_mode = time_mode;
}



// get the frequency distribution object of a GlissGrain parameter object
Distribution * get_frequency_distribution_gliss_parameters(GlissGrainParameters *glissgrain)
{
    return glissgrain->frequency;

}


Distribution * get_frequency_target_distribution_gliss_parameters(GlissGrainParameters *glissgrain)
{
    return glissgrain->frequency_target;
}

Distribution * get_time_distribution_gliss_parameters(GlissGrainParameters *glissgrain)
{
    return glissgrain->time;
}


Distribution * get_noise_distribution_gliss_parameters(GlissGrainParameters *glissgrain)
{
    return glissgrain->noise;
}


Distribution * get_noise_target_distribution_gliss_parameters(GlissGrainParameters *glissgrain)
{
    return glissgrain->noise_target;
}

Distribution * get_noise_bandwidth_distribution_gliss_parameters(GlissGrainParameters *glissgrain)
{
       return glissgrain->noise_bandwidth;
}


void init_glissgrain(void *glissgrain, void *source, Grain *grain)
{
    GlissGrain *active;
    GlissGrainParameters *parent;
    float time;
    active = (GlissGrain *) glissgrain;
    parent = (GlissGrainParameters *)source;
    
    // set grain initial frequency
    active->frequency = sample_from_distribution(parent->frequency);
    
    // set the target frequency, depending on whether it is absolute or relative
    if(parent->frequency_mode==GLISS_MODE_ABSOLUTE_FREQUENCY)    
        active->frequency_target = sample_from_distribution(parent->frequency_target);    
    else    
        active->frequency_target = active->frequency * sample_from_distribution(parent->frequency_target);
    
    // time selection
    if(parent->time_mode==TIME_MODE_ABSOLUTE)    
        time = sample_from_distribution(parent->time);    
    else    
        time = grain->duration * sample_from_distribution(parent->time);    
    
    // coefficient for frequency sweep
    active->frequency_coeff = exp(-1.0 / ((time*GLOBAL_STATE.sample_rate) * 1.5));
    
    // noise parameters
    active->noise = sample_from_distribution(parent->noise);
    active->noise_target = sample_from_distribution(parent->noise_target);
    
    active->noise_prev =  0.0;
    active->noise_int = 0.0;
    active->noise_bandwidth_coeff = exp(-2 * M_PI * sample_from_distribution(parent->noise_bandwidth)/GLOBAL_STATE.sample_rate);
    
    
    
    active->phase = 0.0;
        
}


// create a new grain from a GlissGrain parent object
void *create_glissgrain(void *source)
{
    GlissGrain *active;
    active = malloc(sizeof(*active));
    return (void*)active;
}



// fill a buffer with a sinewave at a fixed frequency
void fill_glissgrain(void *glissgrain, Buffer *buffer)
{
    int i;
    GlissGrain *active;
    active = (GlissGrain *)glissgrain;
    float phase_increment;
    
    
    for(i=0;i<buffer->n_samples;i++)
    {  
        phase_increment = (2*M_PI*active->frequency) / (double)GLOBAL_STATE.sample_rate;     
        
        // apply slide
        active->frequency = active->frequency_coeff * active->frequency + (1-active->frequency_coeff) * active->frequency_target;
        active->noise = active->frequency_coeff * active->noise + (1-active->frequency_coeff) * active->noise_target;
        
        // compute lowpassed brownian noise
        active->noise_prev = active->noise_bandwidth_coeff * active->noise_prev + (1-active->noise_bandwidth_coeff) * (uniform_double()-0.5);    
        active->noise_int += active->noise_prev * active->noise;
       
    
          
        buffer->x[i] = sin(active->phase);        
        // increment phase, with some phase noise
        active->phase += phase_increment + active->noise_prev * active->noise;    
        
    }

}

// destroy a sine grain object
void destroy_glissgrain(void *glissgrain)
{
    free((GlissGrain*)glissgrain);

}