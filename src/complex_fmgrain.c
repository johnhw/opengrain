/**    
    @file complex_fmgrain.c
    @brief
    @author John Williamson
    
    Copyright (c) 2011 All rights reserved.
    Licensed under the BSD 3 clause license. See COPYING.
    
    This file is part of the OpenGrain distribution.
    http://opengrain.sourceforge.net   
*/              

#include "fmgrain.h"






// create a sine grain parameter structure
FMGrainParameters *create_fm_parameters(GrainSource *source)
{
    FMGrainParameters *fmgrain;
    fmgrain = malloc(sizeof(*fmgrain));
    fmgrain->frequency = create_distribution();
    
    
    // defaults
    set_constant_distribution(fmgrain->frequency, 440.0);
    
    
    return fmgrain;
}



// destroy a fmgrain parameter object
void destroy_fm_parameters(FMGrainParameters *fmgrain)
{
    destroy_distribution(fmgrain->frequency);
    free(fmgrain);
}


// get the frequency distribution object of a FMGrain parameter object
Distribution * get_frequency_distribution_fm_parameters(FMGrainParameters *fmgrain)
{
    return fmgrain->frequency;

}


void init_fmgrain(void *fmgrain, void *source, Grain *grain)
{
    FMGrain *active;
    FMGrainParameters *parent;
    active = (FMGrain *) fmgrain;
    parent = (FMGrainParameters *)source;
    
    active->frequency = sample_from_distribution(parent->frequency);
    active->phase = 0.0;
    active->phase_increment = (2*M_PI*active->frequency) / (double)GLOBAL_STATE.sample_rate;     
    
}


// create a new grain from a FMGrain parent object
void *create_fmgrain(void *source)
{
    FMGrain *active;
    active = malloc(sizeof(*active));
    return (void*)active;
}



// fill a buffer with a sinewave at a fixed frequency
void fill_fmgrain(void *fmgrain, Buffer *buffer)
{
    int i;
    FMGrain *active;
    active = (FMGrain *)fmgrain;
    
    for(i=0;i<buffer->n_samples;i++)
    {        
        buffer->x[i] = sin(active->phase);
        active->phase += active->phase_increment;    
        
    }

}

// destroy a sine grain object
void destroy_fmgrain(void *fmgrain)
{
    free((FMGrain*)fmgrain);

}