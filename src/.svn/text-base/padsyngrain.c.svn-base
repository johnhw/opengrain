/**    
    @file padsyngrain.c
    @brief
    @author John Williamson
    
    Copyright (c) 2011 All rights reserved.
    Licensed under the BSD 3 clause license. See COPYING.
    
    This file is part of the OpenGrain distribution.
    http://opengrain.sourceforge.net   
*/              

#include "padsyngrain.h"
#include "complex_buffer.h"
#include <string.h>


// create an empty impulse grain parameter set
PadSynGrainParameters *create_padsyn_parameters(GrainSource *source)
{

    PadSynGrainParameters *padsyngrain;
    padsyngrain = malloc(sizeof(*padsyngrain));        
    padsyngrain->pad_loop = NULL;
    padsyngrain->pitch_shift = create_distribution();
    return padsyngrain;
}


// return the pitch shift distribution (in semi-tones)
Distribution * get_pitch_shift_distribution_padsyn_parameters(PadSynGrainParameters *padsyn_parameters)
{
    return padsyn_parameters->pitch_shift;
}



// set the wave for this pad synth, and give the loop size in samples (must be power of 2), and an amount of noise
// to add to the spectrum
void set_wave_padsyn_parameters(PadSynGrainParameters *padsyn_parameters, WaveSound *sound, int loop_size, float noise_level)
{
    FFT *fft;
    ComplexBuffer *complex_buffer, *complex_accumulate_buffer;
    
    Buffer *channel;
    Buffer *real_buffer;
    int  i,j, nbuffers;
    
    complex_buffer = create_complex_buffer(loop_size);
    complex_accumulate_buffer = create_complex_buffer(loop_size);
    real_buffer = create_buffer(loop_size);
    
    fft = create_fft(loop_size);
    
    // copy in the input buffer, 
    j = 0;
    nbuffers = 0;
    channel = get_channel_wave_sound(sound,0);
    
    while(j < channel->n_samples)
    {
        // copy in the input                 
        for(i=0;i<real_buffer->n_samples;i++)
        {
            if(j < channel->n_samples)
                real_buffer->x[i] = channel->x[j];
            else
                real_buffer->x[i] = 0;                
            j++;
        }
        
        nbuffers++;
        // fft the buffer
        fft_buffer(fft, real_buffer, complex_buffer);
        
        // accumulate
        accumulate_complex_buffer(complex_accumulate_buffer, complex_buffer);                    
    }
    
    scale_complex_buffer(complex_accumulate_buffer, 1.0/(nbuffers*loop_size/2));
    
    // randomize the phase, and convolve the amplitude with a blurring function
    randomize_complex_buffer(complex_accumulate_buffer, noise_level);
    
    
    // IFFT
    ifft_buffer(fft, complex_accumulate_buffer, real_buffer);
    
        
    // free the temporary complex data
    destroy_complex_buffer(complex_accumulate_buffer);
    destroy_complex_buffer(complex_buffer);
    destroy_fft(fft);
       
    
    padsyn_parameters->pad_loop = real_buffer;
    
}


// destroy an impulse parameter set
void destroy_padsyn_parameters(PadSynGrainParameters *padsyn_parameters)
{

    if(padsyn_parameters->pad_loop)
        destroy_buffer(padsyn_parameters->pad_loop);              
    free(padsyn_parameters);
}


// create an empty impulse grain
void *create_padsyngrain(void *source)
{
    PadSynGrainParameters *padsyn_parameters;
    PadSynGrain *impulse;
    padsyn_parameters = (PadSynGrainParameters*)source;    
    impulse = malloc(sizeof(*impulse));        
    return impulse;
}


// create a new pan synth grain which will play the loop
// starting from a random position each time
void init_padsyngrain(void *padsyngrain, void *source, Grain *grain)
{

    PadSynGrainParameters *padsyn_parameters;
    PadSynGrain *impulse;

    padsyn_parameters = (PadSynGrainParameters*)source;    
    impulse = (PadSynGrain*)padsyngrain;
    
    impulse->pad_loop = padsyn_parameters->pad_loop;
    impulse->offset = uniform_double() * impulse->pad_loop->n_samples;
    
    // set the pitch
    impulse->rate = SEMITONES_TO_RATE(sample_from_distribution(padsyn_parameters->pitch_shift));           
}


// destroy an impulse grain (don't free the sounds!)
void destroy_padsyngrain(void *padsyngrain)
{
    free((PadSynGrain*)padsyngrain);
}


// copy a block from an impulse into a buffer
void fill_padsyngrain(void *padsyngrain, Buffer *buffer)
{
    int i,j, k;
    float t;
    PadSynGrain *impulse;
    impulse = (PadSynGrain *) padsyngrain;
    
    for(i=0;i<buffer->n_samples;i++)
    {
    
        // length of loop must be power of 2!        
        if(impulse->offset >= impulse->pad_loop->n_samples)
            impulse->offset -= impulse->pad_loop->n_samples;
        
        j = (int)impulse->offset + 1;
        k = (int)impulse->offset;
        if(j >= impulse->pad_loop->n_samples)
            j -= impulse->pad_loop->n_samples;
        
        
        t = impulse->offset - floor(impulse->offset);
        
        // no interpolation version
        buffer->x[i] = (1-t) * impulse->pad_loop->x[k] + t * impulse->pad_loop->x[j];
        impulse->offset += impulse->rate;
    }
               
}