/**    
    @file crossdelay.c
    @brief
    @author John Williamson
    
    Copyright (c) 2011 All rights reserved.
    Licensed under the BSD 3 clause license. See COPYING.
    
    This file is part of the OpenGrain distribution.
    http://opengrain.sourceforge.net   
*/              

#include "crossdelay.h"




// Create a feedback delay unit
CrossDelay *create_cross_delay(int channels)
{
    int i;
    
    CrossDelay *cross_delay = malloc(sizeof(*cross_delay)*channels);
    
    cross_delay->filters = malloc(sizeof(*cross_delay->filters)*channels);
    cross_delay->delays = malloc(sizeof(*cross_delay->delays)*channels);
    
    for(i=0;i<channels;i++)
    {
        cross_delay->delays[i] = create_delay();
        set_delay(cross_delay->delays[i], 2);
        
        cross_delay->filters[i] = create_biquad();
        biquad_bandpass(cross_delay->filters[i], 1000.0, 2.0);        
    }
    cross_delay->filtering = 0;
    cross_delay->cross = 0.0;
    cross_delay->feedback = 0.5;
    cross_delay->wet_level = 0.5;
    cross_delay->dry_level = 1.0;
    cross_delay->channels = channels;

    cross_delay->z = malloc(sizeof(*cross_delay->z) * channels);
    
    return cross_delay;
}


// set the overall delay length in seconds
void set_length_cross_delay(CrossDelay *cross_delay, double delay) 
{
    int i;
    for(i=0;i<cross_delay->channels;i++)       
        set_delay(cross_delay->delays[i], delay * GLOBAL_STATE.sample_rate);    
}


// set the feedback and the cross term in the delay network
void set_feedback_cross_delay(CrossDelay *cross_delay, double feedback, double cross)
{
    cross_delay->feedback = dB_to_gain(feedback);
    cross_delay->cross = dB_to_gain(cross);
}

// set the filter for the delay. A frequency of zero disables filtering.
void set_filter_cross_delay(CrossDelay *cross_delay, float freq, float q, int type)
{
    int i;
    
    // disable filtering if freq = 0
    if(freq==0)
        cross_delay->filtering = 0;
    else
    {
        cross_delay->filtering = 1;
        for(i=0;i<cross_delay->channels;i++)           
            set_biquad(cross_delay->filters[i], freq, q, type);            
    }

}


// set the wet and dry mix levels on the output (in decibels)
void set_levels_cross_delay(CrossDelay *cross_delay, double wet, double dry)
{
    cross_delay->wet_level = dB_to_gain(wet);
    cross_delay->dry_level = dB_to_gain(dry);  
}



// Destroy the feedback delay unit
void destroy_cross_delay(CrossDelay *cross_delay)
{
    int i;
    for(i=0;i<cross_delay->channels;i++)  
    {    
        destroy_delay(cross_delay->delays[i]);
        destroy_biquad(cross_delay->filters[i]);
    }

    free(cross_delay->z);        
    free(cross_delay);
}


// Process one sample of the feedback delay unit
void process_cross_delay(CrossDelay *cross_delay, Buffer **ins, Buffer **outs)
{
    
    int i,j,s, buffer_len;
    float sum, input;
    float x,y;
    
    // get size of the buffers from the first input
    buffer_len = ins[0]->n_samples;
    
    for(s=0;s<buffer_len;s++)
    {
        
        for(i=0;i<cross_delay->channels;i++)
        {                   
            x= delay_out(cross_delay->delays[i]);
            if(cross_delay->filtering)
                cross_delay->z[i] = process_biquad(cross_delay->filters[i], x);               
            else
                cross_delay->z[i] = x;
        }
        
        for(i=0;i<cross_delay->channels;i++)
        {
            input = ins[i]->x[s];
            // get sum of other channels
            sum = 0.0;
            for(j=0;j<cross_delay->channels;j++)
                if(j!=i)
                    sum += cross_delay->z[i];
            
            // feedback
            y = cross_delay->z[i] * cross_delay->feedback * (1-cross_delay->cross) + input + sum * cross_delay->feedback * cross_delay->cross;    
            delay_in(cross_delay->delays[i], y);
            
            
            // wet/dry 
            outs[i]->x[s] = input * cross_delay->dry_level + y * cross_delay->wet_level;
        }
    }
        
}



