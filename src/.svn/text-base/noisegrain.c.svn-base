/**    
    @file noisegrain.c
    @brief
    @author John Williamson
    
    Copyright (c) 2011 All rights reserved.
    Licensed under the BSD 3 clause license. See COPYING.
    
    This file is part of the OpenGrain distribution.
    http://opengrain.sourceforge.net   
*/              

#include "noisegrain.h"


// create a noise grain parameters object
NoiseGrainParameters *create_noise_parameters(GrainSource *source)
{
    NoiseGrainParameters *noisegrain;
    noisegrain = malloc(sizeof(*noisegrain));
    noisegrain->frequency = create_distribution();
    noisegrain->q = create_distribution();
    noisegrain->filtered = 1;
    
    
    // defaults
    set_constant_distribution(noisegrain->frequency, 100.0);
    set_constant_distribution(noisegrain->q, 10.0);
    
    return noisegrain;
}

// free a noise grain parameters object
void destroy_noise_parameters(NoiseGrainParameters *noisegrain)
{
    destroy_distribution(noisegrain->frequency);
    destroy_distribution(noisegrain->q);
    free(noisegrain);
}


// set the filtering property
void set_filtered_noisegrain(NoiseGrainParameters *noisegrain, int filtered)
{
    noisegrain->filtered = filtered;
}


// return the frequency distribution of the noise grain
Distribution * get_frequency_distribution_noise_parameters(NoiseGrainParameters *noisegrain)
{
    return noisegrain->frequency;
}


// return the resonance distribution of the noisegrain
Distribution * get_q_distribution_noise_parameters(NoiseGrainParameters *noisegrain)
{
    return noisegrain->q;
}

// create an empty noisegrain object
void *create_noisegrain(void *source)
{
    NoiseGrain *grain;
    NoiseGrainParameters *parameters;
    parameters = (NoiseGrainParameters *)source;
    grain = malloc(sizeof(*grain));
    
    grain->resonator = create_biquad();
    
    return (void*) grain;

}

// sample the parameters of a noise grain, and configure the filter
void init_noisegrain(void *ngrain, void *source, Grain *ggrain)
{
    NoiseGrain *grain;
    NoiseGrainParameters *parameters;
    parameters = (NoiseGrainParameters *)source;
    grain = (NoiseGrain *)ngrain;
    grain->q = sample_from_distribution(parameters->q);
    grain->frequency = sample_from_distribution(parameters->frequency);  
    if(!parameters->filtered)
        grain->frequency = 0.0;
    biquad_bandpass(grain->resonator, grain->frequency, grain->q);    
    reset_biquad(grain->resonator);

}

// destroy a noisegrain object
void destroy_noisegrain(void *ngrain)
{
    NoiseGrain *grain;
    grain = (NoiseGrain *)ngrain;
    
    destroy_biquad(grain->resonator);
    free(grain);    
}


// fill a grain with filtered noise
void fill_noisegrain(void *ngrain, Buffer *buffer)
{
    int i;
    float q;
    NoiseGrain *grain = (NoiseGrain *)ngrain;
    
    if(grain->frequency>0.0)
    {
        for(i=0;i<buffer->n_samples;i++)
        {
            q = uniform_double() - 0.5;
            q = process_biquad(grain->resonator, 2*q);
            buffer->x[i] = q;
        }
    }
    else
    {
        // unfiltered
        for(i=0;i<buffer->n_samples;i++)
        {
            buffer->x[i] = 2*(uniform_double() - 0.5);        
        }
           
        
    
    }

}
