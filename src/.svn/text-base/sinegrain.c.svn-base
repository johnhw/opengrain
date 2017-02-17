/**    
    @file sinegrain.c
    @brief
    @author John Williamson
    
    Copyright (c) 2011 All rights reserved.
    Licensed under the BSD 3 clause license. See COPYING.
    
    This file is part of the OpenGrain distribution.
    http://opengrain.sourceforge.net   
*/              

#include "sinegrain.h"






// create a sine grain parameter structure
SineGrainParameters *create_sine_parameters(GrainSource *source)
{
    SineGrainParameters *sinegrain;
    sinegrain = malloc(sizeof(*sinegrain));
    sinegrain->frequency = create_distribution();
    
    
    // defaults
    set_constant_distribution(sinegrain->frequency, 440.0);
    
    
    return sinegrain;
}



// destroy a sinegrain parameter object
void destroy_sine_parameters(SineGrainParameters *sinegrain)
{
    destroy_distribution(sinegrain->frequency);
    free(sinegrain);
}


// get the frequency distribution object of a SineGrain parameter object
Distribution * get_frequency_distribution_sine_parameters(SineGrainParameters *sinegrain)
{
    return sinegrain->frequency;

}


void init_sinegrain(void *sinegrain, void *source, Grain *grain)
{
    SineGrain *active;
    SineGrainParameters *parent;
    active = (SineGrain *) sinegrain;
    parent = (SineGrainParameters *)source;
    
    active->frequency = sample_from_distribution(parent->frequency);
    active->phase = 0.0;
    active->phase_increment = (2*M_PI*active->frequency) / (double)GLOBAL_STATE.sample_rate;     
    
}


// create a new grain from a SineGrain parent object
void *create_sinegrain(void *source)
{
    SineGrain *active;
    active = malloc(sizeof(*active));
    return (void*)active;
}



// fill a buffer with a sinewave at a fixed frequency
void fill_sinegrain(void *sinegrain, Buffer *buffer)
{
    int i;
    SineGrain *active;
    active = (SineGrain *)sinegrain;
    
    for(i=0;i<buffer->n_samples;i++)
    {        
        buffer->x[i] = sin(active->phase);
        active->phase += active->phase_increment;    
        
    }

}

// destroy a sine grain object
void destroy_sinegrain(void *sinegrain)
{
    free((SineGrain*)sinegrain);

}