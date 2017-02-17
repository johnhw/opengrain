/**    
    @file grainmixer.c
    @brief Mixes a set of GrainStreams together, and applies global effects. 
    Optional effects are reverb (randomized Datorro configuration),
    multiband equalization, dynamic range compression and stereo widening.
    
    @author John Williamson
    
    Copyright (c) 2011 All rights reserved.
    Licensed under the BSD 3 clause license. See COPYING.
    
    This file is part of the OpenGrain distribution.
    http://opengrain.sourceforge.net   
*/              

#include "grainmixer.h"
#include <math.h>



// Create the mixer object
GrainMixer *create_mixer()
{
    GrainMixer *mixer = malloc(sizeof(*mixer));
    
    
    // gain = 0.0 dB by default
    set_gain_mixer(mixer, 0.0);
    
    // effects
    
    mixer->random_reverb = create_random_reverb(2);   
    set_size_random_reverb(mixer->random_reverb, 0.2);
    
    set_modulation_random_reverb(mixer->random_reverb, 1.0);
    // reverb off
    mixer->reverb_level = 0.0;
    mixer->reverb_enabled = 0;
        
    
    mixer->eq = create_eq();
    mixer->widener = create_widener();
    mixer->compressor = create_compressor();
    set_widener(mixer->widener, 0.0003, 0.02);
    
    
    mixer->compressor_enabled = 0;
    mixer->eq_enabled = 1;
    mixer->test_tone_enabled = 0;
    mixer->widener_enabled = 0;
    
    mixer->temp_left = create_buffer(GLOBAL_STATE.frames_per_buffer);
    mixer->temp_right = create_buffer(GLOBAL_STATE.frames_per_buffer);
    mixer->aux = create_buffer(GLOBAL_STATE.frames_per_buffer);
    
    
    mixer->diffuse_lowpass = create_biquad();
    biquad_lowpass(mixer->diffuse_lowpass, 1000.0, 1.0);
    // create the stream list
    mixer->stream_list = malloc(sizeof(*mixer->stream_list));
    list_init(mixer->stream_list);
        
    return mixer;   
}



// Turn on the test tone (verifies audio is working correctly)
void enable_test_tone_mixer(GrainMixer *mixer)
{
    mixer->test_tone_enabled = 1;
}


// Turn off test tone
void disable_test_tone_mixer(GrainMixer *mixer)
{
   mixer->test_tone_enabled = 0;
}

void enable_eq_mixer(GrainMixer *mixer)
{
    mixer->eq_enabled = 1;
}

void disable_eq_mixer(GrainMixer *mixer)
{   
    mixer->eq_enabled = 0;
}

void enable_reverb_mixer(GrainMixer *mixer)
{
    mixer->reverb_enabled = 1;

}


void disable_reverb_mixer(GrainMixer *mixer)
{
    mixer->reverb_enabled = 0;
}


StereoCompressor *get_compressor_mixer(GrainMixer *mixer)
{
    return mixer->compressor;
}

void enable_compressor_mixer(GrainMixer *mixer)
{
    mixer->compressor_enabled = 1;

}



void disable_compressor_mixer(GrainMixer *mixer)
{
    mixer->compressor_enabled = 0;
}


// get the stereo widener
Widener *get_widener_mixer(GrainMixer *mixer)
{
    return mixer->widener;
}


// enable the stereo widener
void enable_widener_mixer(GrainMixer *mixer)
{
    mixer->widener_enabled = 1;
}


// disable the stereo widener
void disable_widener_mixer(GrainMixer *mixer)
{
      mixer->widener_enabled = 0;
}



// Set the overall gain of the output, in decibels
void set_gain_mixer(GrainMixer *mixer, double dBgain)
{
    mixer->dB_gain = dBgain;
    mixer->again = dB_to_gain(mixer->dB_gain);    
    mixer->again_target = mixer->again;
    mixer->again_coeff = 1.0;
}

//Fade the mixer to a new level, over a given period of time
void set_gain_mixer(GrainMixer *mixer, float dBgain, float time)
{
    mixer->again_target = dB_to_gain(mixer->dB_gain);
    mixer->again_coeff = rc_time(time, -80.0);
}

// set the level of the reverb, in decibels
void set_reverb_level_mixer(GrainMixer *mixer, float wet)
{
    mixer->reverb_level = dB_to_gain(wet);        
}

// set the reverb mode (which of the Dattoro/random reverbs to use)
void set_reverb_mode_mixer(GrainMixer *mixer, int mode)
{
    mixer->reverb_mode = mode;

}




RandomReverb *get_random_reverb_mixer(GrainMixer *mixer)
{

    return mixer->random_reverb;
}

// return the eq object
StereoEQ *get_eq_mixer(GrainMixer *mixer)
{
    return mixer->eq;
}


// Add a grain stream to the mixer
void add_stream(GrainMixer *mixer, GrainStream *stream)
{
    list_append(mixer->stream_list, stream);
}


// Remove a grain stream from the mixer
void remove_stream(GrainMixer *mixer, GrainStream *stream)
{
    int index;
    index = list_locate(mixer->stream_list, stream);
    if(index>=0)
        list_delete_at(mixer->stream_list, index);
}


// Destroy the mixer object
void destroy_mixer(GrainMixer *mixer)
{
    list_destroy(mixer->stream_list);
    free(mixer->stream_list);
    destroy_widener(mixer->widener);
    destroy_random_reverb(mixer->random_reverb);
    destroy_eq(mixer->eq);
}



// produce a stereo test tone
void test_tone(Buffer *left, Buffer *right)
{
   static double phase;
   float l,r;
   double q, qpan;
   int i;
   for(i=0;i<left->n_samples;i++)
    {   
        // sine oscillator, at 440.0Hz modulated at 8.8Hz  panning left to right at 1Hz
        q = sin(phase * 440.0 * 2 * M_PI) * sin(phase * 8.8 * 2 * M_PI);
        qpan = phase;            
        phase += 1.0/GLOBAL_STATE.sample_rate;
        if(phase>=1.0)
            phase = 0.0;            
        // pan and output
        pan(q, qpan*2-1, &l, &r);
        left->x[i] = l;
        right->x[i] = r;                        
    }
}



// Mix all off the streams into a stereo buffer
void grain_mix(GrainMixer *mixer, Buffer *left, Buffer *right)
{
    int i;
    float l, r, l0, r0, again;
    Buffer *in_buffers[3], *out_buffers[2];    
    GrainStream *stream;
    
    in_buffers[0] = left;
    in_buffers[1] = right;
    in_buffers[2] = mixer->aux;
    
    out_buffers[0] = mixer->temp_left;
    out_buffers[1] = mixer->temp_right;
    
    
    
    //GrainStream *stream;
    again = dB_to_gain(mixer->dB_gain);
    
    // Clear the buffers
    zero_buffer(left);
    zero_buffer(right);
    zero_buffer(mixer->aux);
    
    
    // if there is a test tone, play that and don't compute anything else
    if(mixer->test_tone_enabled)
    {       
        test_tone(left, right);        
        return;
    }
    
    

    // sum up all the incoming streams
    list_iterator_start(mixer->stream_list);
    while(list_iterator_hasnext(mixer->stream_list))
    {
          
          stream = (GrainStream *) list_iterator_next(mixer->stream_list);            
          sum_buffer_stream(stream, &in_buffers[0]); // add the values into the buffer    
    }
    list_iterator_stop(mixer->stream_list);

   
        
    // apply the final effects
    for(i=0;i<left->n_samples;i++)
    {   

        l = left->x[i];
        r = right->x[i];
        
        if(mixer->eq_enabled)
        {
            // eq
            compute_eq(mixer->eq, l, r, &l0, &r0);
            l = l0;
            r = r0;
        }
        
        if(mixer->compressor_enabled)
        {
            // compressor
            compute_compressor(mixer->compressor, l, r, &l0, &r0);
            l = l0;
            r = r0;
        }
        
        if(mixer->widener_enabled)
        {
            // stereo widening
            compute_widener(mixer->widener, l,r, &l0, &r0);                    
            l = l0;
            r = r0;        
        }
                                               
    }
        
   if(mixer->reverb_enabled)
   { 
        // reverb
        scale_buffer(mixer->aux, dB_to_gain(-10.0));
        biquad_buffer(mixer->aux, mixer->diffuse_lowpass);
        compute_random_reverb(mixer->random_reverb, &in_buffers[0], 3, &out_buffers[0]);        
        mix_buffer(left, out_buffers[0], mixer->reverb_level);
        mix_buffer(right, out_buffers[1], mixer->reverb_level);            
    }        
    
    // gain fade/clipping
    for(i=0;i<left->n_samples;i++)
    {
        left->x[i] *= mixer->again;
        right->x[i] *= mixer->again;
        mixer->again = mixer->again_coeff * mixer->again + (1.0-mixer->again_coeff)*mixer->again_target;        
    }
    
    clip_buffer(left);
    clip_buffer(right);
    
    
}


