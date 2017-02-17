/**    
    @file chorus.c
    @brief Simple chorus/flanger with a modulating delay line.
    Based on the chorus design given by Jon Dattoro in 
    Dattoro, J. "Effect Design Part 2: Delay-Line Modulation and Chorus"  J. Audio Eng Sot., 1997 45:10
    
    @author John Williamson
    
    Copyright (c) 2011 All rights reserved.
    Licensed under the BSD 3 clause license. See COPYING.
    
    This file is part of the OpenGrain distribution.
    http://opengrain.sourceforge.net   
*/              

#include "chorus.h"

// create aan empty chorus object
Chorus *create_chorus(void)
{
    Chorus *chorus;
    chorus = malloc(sizeof(*chorus));
    chorus->delay = create_mdelay();
    set_interpolation_mode_mdelay(chorus->delay, MODDELAY_INTERPOLATION_ALLPASS);
    set_chorus(chorus, 0.002, 1.0, 0.8, 1.0, 0.0, 1.0);
    
    
    return chorus;
}

// set all parameters of the chorus
void set_chorus(Chorus *chorus, float delay, float mod_freq, float mod_depth, float blend, float feedback, float feedforward)
{
    set_chorus_feedback(chorus, feedback);
    set_chorus_blend(chorus, blend);
    set_chorus_feedforward(chorus, feedforward);    
    set_chorus_delay(chorus,delay);
    set_chorus_modulation(chorus, mod_freq, mod_depth);    
}


// set the feedback (dry) scaling factor of the chorus. Should be < 1.0
void set_chorus_feedback(Chorus *chorus, float feedback)
{
    chorus->feedback = feedback;
}

// set the blend (dry) scaling factor of the chorus. Should be < 1.0
void set_chorus_blend(Chorus *chorus, float blend)
{
    chorus->blend = blend;
}


// set the feedforward scaling factor of the chorus. Should be < 1.0
void set_chorus_feedforward(Chorus *chorus, float feedforward)
{
   chorus->feedforward = feedforward;
}


// set the delay time of the chorus, in seconds
void set_chorus_delay(Chorus *chorus, float delay)
{
    set_mdelay(chorus->delay, delay * GLOBAL_STATE.sample_rate);  
}


// set the chorus modulation. Frequency is in Hz, mod_extent is a fraction
// from 0.0 -> 1.0
void set_chorus_modulation(Chorus *chorus, float mod_freq, float mod_extent)
{
     set_modulation_mdelay(chorus->delay, mod_extent * chorus->delay->read_offset, mod_freq);
}


// destroy a chorus object
void destroy_chorus(Chorus *chorus)
{
    free(chorus->delay);
    free(chorus);
    
}


// process a sample through the chorus
float process_chorus(Chorus *chorus, float x)
{
    float y;
    
    y = chorus->blend * x;
    mdelay_in(chorus->delay, x);
    y += chorus->feedforward * mdelay_out(chorus->delay);
    y -= chorus->feedback * unmodulated_mdelay_out(chorus->delay);
    return y;
}

// Create a mulitchannel chorus that applies chorus to all channels
MultichannelChorus *create_multichannel_chorus(int channels)
{
    int i;
    MultichannelChorus *chorus;
    chorus = malloc(sizeof(*chorus));
    chorus->choruses = malloc(sizeof(*chorus->choruses)*channels);
    chorus->channels = channels;
    for(i=0;i<chorus->channels;i++)
        chorus->choruses[i] = create_chorus();
    return chorus;
}

// destroy a multichannel chorus
void destroy_multichannel_chorus(MultichannelChorus *chorus)
{   
    int i;
    for(i=0;i<chorus->channels;i++)
        free(chorus->choruses[i]);
    free(chorus->choruses);
    free(chorus);
}

// set a multichannel chorus parameter set
void set_multichannel_chorus(MultichannelChorus *chorus, float delay, float mod_freq, float mod_depth, float blend, float feedback, float feedforward)
{
    int i;
    for(i=0;i<chorus->channels;i++)
        set_chorus(chorus->choruses[i], delay, mod_freq, mod_depth, blend, feedback, feedforward);
}


// set the feedback term for all channels
void set_multichannel_chorus_feedback(MultichannelChorus *chorus, float feedback)
{
    int i;
    for(i=0;i<chorus->channels;i++)
        set_chorus_feedback(chorus->choruses[i],  feedback);
}

// set the blend term for all channels
void set_multichannel_chorus_blend(MultichannelChorus *chorus, float blend)
{
    int i;
    for(i=0;i<chorus->channels;i++)
        set_chorus_blend(chorus->choruses[i], blend);
}

// set the feedforward (dry) term for all channels
void set_multichannel_chorus_feedforward(MultichannelChorus *chorus, float feedforward)
{
    int i;
    for(i=0;i<chorus->channels;i++)
        set_chorus_feedforward(chorus->choruses[i], feedforward);
}

// set the nominal delay for all channels (in seconds)
void set_multichannel_chorus_delay(MultichannelChorus *chorus, float delay)
{
    int i;
    for(i=0;i<chorus->channels;i++)
        set_chorus_delay(chorus->choruses[i], delay);
}

// set the modulation depth and frequency (Hz and 0.0->1.0 respectively)
void set_multichannel_chorus_modulation(MultichannelChorus *chorus, float mod_freq, float mod_extent)
{
    int i;
    for(i=0;i<chorus->channels;i++)
        set_chorus_modulation(chorus->choruses[i], mod_freq, mod_extent);

}

// process a mulitchannel buffer through a chorus
void process_multichannel_chorus(MultichannelChorus *chorus, Buffer **ins, Buffer **outs)
{
    int i, j;
    for(j=0;j<chorus->channels;j++)   
        for(i=0;i<ins[j]->n_samples;i++)        
            outs[j]->x[i] = process_chorus(chorus->choruses[j], ins[j]->x[i]);        
            
}


