/**    
    @file fmgrain.c
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
    
    fmgrain->ratio = create_distribution();
    fmgrain->modulation = create_distribution();
    fmgrain->frequency = create_distribution();
    
    
    // defaults
    set_constant_distribution(fmgrain->frequency, 440.0);
    set_constant_distribution(fmgrain->ratio, 0.5);
    set_constant_distribution(fmgrain->modulation, 0.1);  
   
    return fmgrain;
}



// destroy a fmgrain parameter object
void destroy_fm_parameters(FMGrainParameters *fmgrain)
{
    destroy_distribution(fmgrain->frequency);
    destroy_distribution(fmgrain->ratio);
    destroy_distribution(fmgrain->modulation);
    free(fmgrain);
}


// get the ratio distribution of an fm grain
Distribution * get_frequency_distribution_fm_parameters(FMGrainParameters *fmgrain)
{
    return fmgrain->frequency;
}

// get the ratio distribution of an fm grain
Distribution * get_ratio_distribution_fm_parameters(FMGrainParameters *fmgrain)
{
    return fmgrain->ratio;
}

// get the modulation depth distribution 
Distribution * get_modulation_distribution_fm_parameters(FMGrainParameters *fmgrain)
{
    return fmgrain->modulation;
}



void init_fmgrain(void *fmgrain, void *source, Grain *grain)
{
    FMGrain *active;
    FMGrainParameters *parent;
    float frequency, ratio;
    active = (FMGrain *) fmgrain;
    parent = (FMGrainParameters *)source;
    
    frequency = sample_from_distribution(parent->frequency);
    ratio = sample_from_distribution(parent->ratio);
    active->mod_phase = 0.0;
    active->carrier_phase = 0.0;
    active->modulation = sample_from_distribution(parent->modulation);
    active->carrier_phase_increment = (2*M_PI*frequency) / (double)GLOBAL_STATE.sample_rate;     
    active->mod_phase_increment = (2*M_PI*frequency * ratio) / (double)GLOBAL_STATE.sample_rate;     
    
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
        buffer->x[i] = sin(active->carrier_phase + active->modulation*(sin(active->mod_phase)));
        active->carrier_phase += active->carrier_phase_increment;    
        active->mod_phase += active->mod_phase_increment;    
        
    }

}

// destroy a sine grain object
void destroy_fmgrain(void *fmgrain)
{
    free((FMGrain*)fmgrain);

}