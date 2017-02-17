/**    
    @file multisinegrain.c
    @brief
    @author John Williamson
    
    Copyright (c) 2011 All rights reserved.
    Licensed under the BSD 3 clause license. See COPYING.
    
    This file is part of the OpenGrain distribution.
    http://opengrain.sourceforge.net   
*/              

#include "multisinegrain.h"






// create a multisine grain parameter structure
MultiSineGrainParameters *create_multisine_parameters(GrainSource *source)
{
    MultiSineGrainParameters *multisinegrain;
    multisinegrain = malloc(sizeof(*multisinegrain));
    multisinegrain->frequency = create_distribution();
    multisinegrain->decay = create_distribution();
    multisinegrain->amplitude = create_distribution();
    multisinegrain->n_sines = create_distribution();
    
    
    // defaults
    set_constant_distribution(multisinegrain->frequency, 440.0);
    set_constant_distribution(multisinegrain->amplitude, 0.0);
    set_constant_distribution(multisinegrain->decay, 1.0);
    set_constant_distribution(multisinegrain->n_sines, 1.0);
    multisinegrain->time_mode = TIME_MODE_RELATIVE;    
        
    return multisinegrain;
}


// set the decay mode timing
void set_decay_mode(MultiSineGrainParameters *multisinegrain, int mode)
{
    multisinegrain->time_mode = mode;    
}


// destroy a multisinegrain parameter object
void destroy_multisine_parameters(MultiSineGrainParameters *multisinegrain)
{
    destroy_distribution(multisinegrain->frequency);
    free(multisinegrain);
}


// get the frequency distribution object of a MultiSineGrain parameter object
Distribution * get_frequency_distribution_multisine_parameters(MultiSineGrainParameters *multisinegrain)
{
    return multisinegrain->frequency;

}

// get the amplitude distribution (in dB)
Distribution * get_amplitude_distribution_multisine_parameters(MultiSineGrainParameters *multisinegrain)
{
    return multisinegrain->amplitude;
}

// get the decay distribution (in seconds or multiples of duration)
Distribution * get_decay_distribution_multisine_parameters(MultiSineGrainParameters *multisinegrain)
{
        return multisinegrain->decay;
}

// get the distribution of number of sine waves
Distribution * get_n_sines_distribution_multisine_parameters(MultiSineGrainParameters *multisinegrain)
{
    return multisinegrain->n_sines;
}

void init_multisinegrain(void *multisinegrain, void *source, Grain *grain)
{   
    int i;
    MultiSineGrain *active;
    MultiSineGrainParameters *parent;
    float time;
    active = (MultiSineGrain *) multisinegrain;
    parent = (MultiSineGrainParameters *)source;
        
    
    active->n_sines = MIN((int)sample_from_distribution(parent->n_sines),MAX_SINES-1);
    for(i=0;i<active->n_sines;i++)
    {
        // sample frequencies
        active->frequencies[i] = sample_from_distribution(parent->frequency);
        active->phases[i] = 0.0;
        
        // sample levels and decays
        active->levels[i] = dB_to_gain(sample_from_distribution(parent->amplitude));
        
        // relative or absolute timing
        if(parent->time_mode==TIME_MODE_ABSOLUTE)
            time = sample_from_distribution(parent->decay);
        else
            time = sample_from_distribution(parent->decay) * grain->duration;
  
        active->decay_coeffs[i] = exp(-1.0 / (time*GLOBAL_STATE.sample_rate) / 2.1);
        
        active->phase_increments[i] = (2*M_PI*active->frequencies[i]) / (double)GLOBAL_STATE.sample_rate;     
    }
    
}


// create a new grain from a MultiSineGrain parent object
void *create_multisinegrain(void *source)
{
    MultiSineGrain *active;
    active = malloc(sizeof(*active));
    
    // allocate memory for the sine components
    active->frequencies = malloc(sizeof(*active->frequencies) * MAX_SINES);
    active->phases = malloc(sizeof(*active->phases) * MAX_SINES);
    active->phase_increments = malloc(sizeof(*active->phase_increments) * MAX_SINES);
    active->levels = malloc(sizeof(*active->levels) * MAX_SINES);
    active->decay_coeffs = malloc(sizeof(*active->decay_coeffs) * MAX_SINES);
    
    return (void*)active;
}



// fill a buffer with a multisinewave at a fixed frequency
void fill_multisinegrain(void *multisinegrain, Buffer *buffer)
{
    int i,j;
    MultiSineGrain *active;
    active = (MultiSineGrain *)multisinegrain;
    
    
    for(i=0;i<buffer->n_samples;i++)
    {        
        buffer->x[i] = 0.0;
        for(j=0;j<active->n_sines;j++)
        {
            buffer->x[i] += sin(active->phases[j]) * active->levels[j];
            active->levels[j] = active->decay_coeffs[j] * active->levels[j];
            active->phases[j] += active->phase_increments[j];    
        }
        
    }

}

// destroy a multisine grain object
void destroy_multisinegrain(void *multisinegrain)
{
    free((MultiSineGrain*)multisinegrain);

}