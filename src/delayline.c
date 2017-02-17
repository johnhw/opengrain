/**    
    @file delayline.c
    @brief
    @author John Williamson
    
    Copyright (c) 2011 All rights reserved.
    Licensed under the BSD 3 clause license. See COPYING.
    
    This file is part of the OpenGrain distribution.
    http://opengrain.sourceforge.net   
*/              

#include "delayline.h"

#define INIT_DELAY_MAX 1024

// Create a delay line with a given maximum length
// Delay will start out with a delay equal to the maximum
DelayLine *create_delay()
{
    DelayLine *delay = malloc(sizeof(*delay));
    delay->max_n_samples = INIT_DELAY_MAX;
    delay->n_samples = 0; 
    delay->read_head = 1;
    delay->write_head = 0;
    delay->samples = calloc(sizeof(*delay->samples), INIT_DELAY_MAX);
    return delay;
}

// Destroy a delay line object
void destroy_delay(DelayLine *delay)
{
    free(delay->samples);
    free(delay);
}


// Delay a sample
float delay(DelayLine *delay, float sample)
{
    float r;
    delay->samples[delay->write_head++] = sample;
    r = delay->samples[delay->read_head++];
    if(delay->read_head>=delay->n_samples)
        delay->read_head = 0;
    if(delay->write_head>=delay->n_samples)
        delay->write_head = 0;
    return r;
}

// Take a new sample into the delay line
void delay_in(DelayLine *delay, float sample)
{
    delay->samples[delay->write_head++] = sample;
    delay->read_head++;
    if(delay->read_head>=delay->n_samples)
        delay->read_head = 0;
    if(delay->write_head>=delay->n_samples)
        delay->write_head = 0;
}


// Get the sample at (write_head - index)
float get_delay(DelayLine *delay, int index)
{
    int rindex = delay->write_head - index;
    while(rindex<0)
        rindex += delay->n_samples;
    return delay->samples[rindex];
}

// Return the current output of the delay line 
// Does not adjust the delay line state
float delay_out(DelayLine *delay)
{
    return delay->samples[delay->read_head];
}


// expand the size of the delay line to fit
void expand_delay(DelayLine *delay,int samples)
{
    int i, old_length;
    old_length = delay->max_n_samples;
    
    delay->max_n_samples = samples;
    delay->samples = realloc(delay->samples, sizeof(*delay->samples)*delay->max_n_samples);
    
    // zero out new part
    for(i=old_length;i<delay->max_n_samples;i++)
    {
        delay->samples[i] = 0.0;    
    }
}

// Set the delay line length
void set_delay(DelayLine *delay, int delay_length)
{

    while(delay_length >= delay->max_n_samples)
    {
        expand_delay(delay, delay_length+1000);    
    }
    
    if(delay_length > 2 && delay_length <= delay->max_n_samples)
    {
        delay->n_samples = delay_length;
    }
    
   
    
    if(delay->read_head>=delay->n_samples)
        delay->read_head = 0;
    if(delay->write_head>=delay->n_samples)
        delay->write_head = 0;
        
        
    // stop pointers from overlapping
    if(delay->read_head==delay->write_head)
    {
        delay->read_head = 1;
    }

}

/*
DelayLine *create_mdelay(int max_samples, float modulation_extent, float modulation_frequency);
void destroy_mdelay(ModDelayLine *delay);
float mdelay(ModDelayLine *delay, float sample);
void mdelay_in(ModDelayLine *delay, float sample);
float mdelay_out(ModDelayLine *delay);
void set_mdelay(ModDelayLine *delay, float delay);
*/



