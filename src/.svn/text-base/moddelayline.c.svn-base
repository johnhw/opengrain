/**    
    @file moddelayline.c
    @brief
    @author John Williamson
    
    Copyright (c) 2011 All rights reserved.
    Licensed under the BSD 3 clause license. See COPYING.
    
    This file is part of the OpenGrain distribution.
    http://opengrain.sourceforge.net   
*/              

#include "moddelayline.h"
#include "audio.h"
#include <math.h>


#define INIT_DELAY 1024

// Create a delay line with a given maximum length
// Delay will start out with a delay equal to the maximum
ModDelayLine *create_mdelay()
{
    ModDelayLine *delay = malloc(sizeof(*delay));
    
    delay->max_n_samples = INIT_DELAY*2;
    delay->n_samples = INIT_DELAY*2;
    delay->read_offset = INIT_DELAY;
    delay->interpolation_mode = MODDELAY_INTERPOLATION_ALLPASS;
    delay->write_head = 0;
    delay->n_samples = 0;
    delay->modulation_extent = 0.0;
    delay->modulation_frequency = 0.0;
    delay->phase = 0.0;
    delay->read_fraction = 0.0;
    delay->excursion = 0.0;
    delay->samples = calloc(sizeof(*delay->samples), delay->max_n_samples);
    
    
    delay->allpass_a = 0.0;
    delay->modulated = 0;
    
    
    
    return delay;
}

// set the frequency and extent of the modulation on this delay line
// if modulation extent is 0, don't modulate
void set_modulation_mdelay(ModDelayLine *delay,  float modulation_extent, float modulation_frequency)
{
    
    if(delay->modulation_extent>=delay->read_offset)
        delay->modulation_extent = delay->read_offset -1;
        
    
    if(delay->modulation_extent==0.0)
    {
        delay->modulated=0;
        delay->excursion = 0;
    }
    else
        delay->modulated=1;
        
    delay->modulation_extent = modulation_extent;
    delay->modulation_frequency = modulation_frequency;
}
    

// Destroy a delay line object
void destroy_mdelay(ModDelayLine *delay)
{
    free(delay->samples);
    free(delay);
}

// Delay a sample
float mdelay(ModDelayLine *delay, float sample)
{
    
    mdelay_in(delay,sample);
    return mdelay_out(delay);        
}


// return the delayed sample _without_ the modulation
float unmodulated_mdelay_out(ModDelayLine *delay)
{
    int aread;
        
    aread = delay->write_head+delay->read_offset;
    if(aread>=delay->n_samples)
        aread -= delay->n_samples;
    return delay->samples[aread];
}

// Take a new sample into the delay line
void mdelay_in(ModDelayLine *delay, float sample)
{
    double offset;
    delay->samples[delay->write_head++] = sample;
    
    if(delay->modulated)
    {
        // update phase
        delay->phase += (2*M_PI*delay->modulation_frequency)/GLOBAL_STATE.sample_rate;
        offset = sin(delay->phase)*delay->modulation_extent;
    
        delay->excursion = floor(offset);    
        delay->read_fraction = offset - floor(offset);
    }
    
        
            
    if(delay->write_head>=delay->n_samples)
        delay->write_head = 0;
        
}


// set the interpolation mode (linear or allpass)
void set_interpolation_mode_mdelay(ModDelayLine *delay, int mode)
{
    delay->interpolation_mode = mode;
}

// Get the sample at (write_head - index)
float get_mdelay(ModDelayLine *delay, int index)
{
    int rindex = delay->write_head - index;
    while(rindex<0)
        rindex += delay->n_samples;
    return delay->samples[rindex];
}

// Return the current output of the delay line 
// Does not adjust the delay line state
float mdelay_out(ModDelayLine *delay)
{
    
    int aread, bread;
    float an, bn, fr, out;
    
    
    aread = delay->write_head+delay->read_offset+delay->excursion;
    bread = delay->write_head+delay->read_offset+delay->excursion+1;
        
    if(bread>=delay->n_samples)
        bread -= delay->n_samples;
    if(aread>=delay->n_samples)
        aread -= delay->n_samples;
        
    
        
    an = delay->samples[aread];
    bn = delay->samples[bread];
    
    if(!delay->modulated)
        return an;
    
        
        
    if(delay->interpolation_mode == MODDELAY_INTERPOLATION_LINEAR)
    {    
        // read from write_head + delay line length + modulation factor                    
        return (1-delay->read_fraction)*an + (delay->read_fraction)*bn;
    }
    else    
    {
        // allpass coefficient
        fr = (1-(1-delay->read_fraction)) / (1+(1-delay->read_fraction));
        
        
        out = bn * fr + an - fr*delay->allpass_a;
        
        
        delay->allpass_a = out;
        
        return out;
    }
    
}


// Set the delay line length
void set_mdelay(ModDelayLine *delay, int delay_length)
{
    // expand the delay line if the new delay is longer than the current delay line
    // always need 2*delay_length samples
    // read head is centered on write_head + delay_length
    if(delay_length*2>= delay->max_n_samples)
    {
        int i,old_length;
        old_length = delay->max_n_samples;
        delay->max_n_samples = delay_length * 2 + 1;
        delay->samples = realloc(delay->samples, sizeof(*delay->samples)*delay->max_n_samples);
        for(i=old_length;i<delay->max_n_samples;i++)
            delay->samples[i] = 0.0;        
      
    
    }

    if(delay_length > 2)
        delay->read_offset = delay_length;        
        
    delay->n_samples = delay_length * 2;

}

