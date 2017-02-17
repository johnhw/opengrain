/**    
    @file acoustics.c
    @brief
    @author John Williamson
    
    Copyright (c) 2011 All rights reserved.
    Licensed under the BSD 3 clause license. See COPYING.
    
    This file is part of the OpenGrain distribution.
    http://opengrain.sourceforge.net   
*/              

#include "acoustics.h"


// create an acoustics channel
AcousticsChannel *create_acoustics_channel(int n_channels)
{
    AcousticsChannel *channel = malloc(sizeof(*channel));
    channel->n_channels = n_channels;
    channel->reverb = create_random_reverb(n_channels);     
    return channel;
}


// destroy a reverb channel
void destroy_acoustics_channel(AcousticsChannel *channel)
{
    destroy_random_reverb(channel->reverb);
    free(channel);
}

// process one channel of reverb
void process_acoustics_channel(AcousticsChannel *channel, Buffer *in, Buffer **out)
{
    Buffer *in_buffers[1];
    in_buffers[0] = in;
    process_random_reverb(channel->reverb, &in_buffers[0], 1, out);    
}


// Create an acoustics object with two internal reverb processors which can be smoothly
// interpolated together
Acoustics *create_acoustics(int n_channels)
{
    int i;
    Acoustics *acoustics = malloc(sizeof(*acoustics));
    acoustics->n_channels = n_channels;
    acoustics->channel_a = create_acoustics_channel(n_channels);    
    acoustics->channel_b = create_acoustics_channel(n_channels);    
    set_interpolate_time_acoustics(acoustics, 0.05);    
    acoustics->interpolate = 0.0;
    acoustics->active_channel = acoustics->channel_a+;
    acoustics->dry = 1.0;
    acoustics->wet = 0.0;
    acoustics->mix_buffer = create_buffer(GLOBAL_STATE.frames_per_buffer);
    acoustics->channel_a_buffers = malloc(sizeof(*acoustics->channel_a_buffers)*n_channels);
    acoustics->channel_b_buffers = malloc(sizeof(*acoustics->channel_b_buffers)*n_channels);
    for(i=0;i<n_channels;i++)
    {    
        acoustics->channel_a_buffers[i] = create_buffer(GLOBAL_STATE.frames_per_buffer);
        acoustics->channel_b_buffers[i] = create_buffer(GLOBAL_STATE.frames_per_buffer);
    }
    
    acoustics->aux_filter = create_biquad();
    
    
    biquad_lowpass(acoustics->aux_filter, 5000.0, 1.0);
    return acoustics;

}

// Destroy an acoustics object
void destroy_acoustics(Acoustics *acoustics)
{
    int i;
    destroy_buffer(acoustics->mix_buffer);
    for(i=0;i<n_channels;i++)    
    {
        destroy_buffer(acoustics->channel_a_buffers[i]);    
        destroy_buffer(acoustics->channel_b_buffers[i]);    
    }    
    free(acoustics->channel_a_buffers);
    free(acoustics->channel_b_buffers);
    destroy_acoustics_channel(acoustics->channel_a);
    destroy_acoustics_channel(acoustics->channel_b);    
    free(acoustics);
}


// switch the active acoustics channel
void switch_active_acoustics(Acoustics *acoustics)
{
    AcousticsChannel *temp;
    temp = acoustics->channel_a;
    acoustics->channel_a = acoustics->channel_b;
    acoustics->channel_b = temp;       
    acoustics->interpolate = 0.0;       
}

// Set the time taken to switch from one acoustics channel to the other
void set_interpolate_time_acoustics(Acoustics *acoustics, float time)
{
    acoustics->interpolate_time = time;
    acoustics->interpolate_rate = 1.0 / (time*GLOBAL_STATE.sample_rate);
}


// set the overall reverb levels
void set_reverb_levels_acoustics(Acoustics *acoustics, float dry, float wet)
{
    acoustics->dry = dB_to_gain(dry);
    acoustics->wet = dB_to_gain(wet);
}

// set the level of the aux channel (representing distant sound)
void set_aux_level_acoustics(Acoustics *acoustics, float aux_level)
{
    acoustics->aux_level = dB_to_gain(aux_level);
}


// set the cutoff frequency of the aux input filter in Hz
// i.e. how much to deaden distant sounds
void set_aux_cutoff_acoustics(Acoustics *acoustics, float aux_cutoff)
{
    biquad_lowpass(acoustics->aux_filter, aux_cutoff, 1.0);
}


// set the impulse response of the acoustics
void set_impulse_acoustics(Acoustics *acoustics, Buffer *impulse)
{
    switch_active_acoustics(acoustics);    
    set_impulse_convolver(acoustics->channel_b->convolver, impulse);
}


// set the size of the "room". 1.0 = default
void set_size_acoustics(Acoustics *acoustics, float size)
{
    switch_active_acoustics(acoustics); 
    
    set_size_random_reverb(acoustics->channel_b->random_reverb);
}


void set_impulse_envelope_acoustics(Acoustics *acoustics, float decay_time)
{

}



// set the diffusion of the reverb
void set_input_diffusion_acoustics(Acoustics *acoustics, float diffusion)
{
    switch_active_acoustics(acoustics); 
    set_input_diffusion_1_random_reverb(acoustics->channel_b->random_reverb, diffusion);
    set_input_diffusion_2_random_reverb(acoustics->channel_b->random_reverb, diffusion);    
}


// set the decay diffusion for the reverb
void set_decay_diffusion_acoustics(Acoustics *acoustics, float diffusion)
{
    switch_active_acoustics(acoustics); 
    set_decay_diffusion_1_random_reverb(acoustics->channel_b->random_reverb, diffusion);
    set_decay_diffusion_2_random_reverb(acoustics->channel_b->random_reverb, diffusion);    
}

// set the decay time of the reverb
void set_decay_acoustics(Acoustics *acoustics, float decay_time)
{
    switch_active_acoustics(acoustics); 
    set_decay_random_reverb(acoustics->channel_b->random_reverb, rc_time(decay_time));    
}


// set the bandwidth of the reverb
void set_bandwidth_acoustics(Acoustics *acoustics, float bandwidth_hz)
{
    switch_active_acoustics(acoustics); 
    set_bandwidth_random_reverb(acoustics->channel_b->random_reverb, rc_freq(bandwidth_hz));    
}


// set the predelay
void set_predelay_acoustics(Acoustics *acoustics, float predelay)
{
    switch_active_acoustics(acoustics); 
    set_predelay_random_reverb(acoustics->channel_b->random_reverb, predelay);    
}


// process a series of buffers through the reverb unit, into a number of output channels
void process_acoustics(Acoustics *acoustics, Buffer **in_channels, int n_n_channels, Buffer *aux, Buffer **out_channels)
{
    int i,j,k;
    
     
    // mix down buffers    
    for(i=0;i<in_channels[0]->n_samples;i++)    
        mix_buffer[i] = acoustics->aux_level * biquad_lowpass(acoustics->aux_filter,aux->x[i]);
        
    
    for(j=0;j<n_in_channels;j++)        
        mix_buffer(acoustics->mix_buffer, in_channels[i], 1.0/n_in_channels);
        
    // process the channels
    process_acoustics_channel(acoustics->channel_a, acoustics->mix_buffer, acoustics->channel_a_buffers);
    process_acoustics_channel(acoustics->channel_b, acoustics->mix_buffer, acoustics->channel_b_buffers);
    
    // crossfade them together
    for(i=0;i<in_channels[0]->n_samples;i++)    
    {
        for(j=0;j<acoustics->n_channels;j++)                
        {
            out_channels[j][i] = acoustics->channel_a_buffers[j][i] * (1.0-acoustics->interpolate); 
            out_channels[j][i] += acoustics->channel_b_buffers[j][i] * (acoustics->interpolate); 
        }
        acoustics->interpolate += acoustics->interpolate_rate;                
        
        if(acoustics->interpolate>1.0)
            acoustics->interpolate = 1.0;
            
    }
    
    
    
    
    


}