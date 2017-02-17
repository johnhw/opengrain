/**    
    @file audio.c
    @brief
    @author John Williamson
    
    Copyright (c) 2011 All rights reserved.
    Licensed under the BSD 3 clause license. See COPYING.
    
    This file is part of the OpenGrain distribution.
    http://opengrain.sourceforge.net   
*/              

#include "audio.h"
#include "sys_audio.h"
#include <stdlib.h>
#include <math.h>
#include <string.h>


#define RANDOM_SEED 50

/*** GLOBAL STATE VARIABLES ***/
AudioState GLOBAL_STATE;          // Audio device state
static void *audio_stream;




// Initialise the audio system
void init_audio(AudioState *prototype, AudioCallback callback, AudioFinishedCallback finished_callback, void *stream_data)
{

    AudioInfo *info;
    
    info = malloc(sizeof(*info));
    info->callback = callback;    
    info->finished_callback = finished_callback;
    info->user_data = stream_data;
                    
    // reset the state
    GLOBAL_STATE.sample_rate = prototype->sample_rate;
    GLOBAL_STATE.n_channels = prototype->n_channels;    
    GLOBAL_STATE.n_input_channels = prototype->n_input_channels;    
    GLOBAL_STATE.input_channel = prototype->input_channel;    
    GLOBAL_STATE.in_device = prototype->in_device;
    GLOBAL_STATE.out_device = prototype->out_device;
    
    GLOBAL_STATE.frames_per_buffer = prototype->frames_per_buffer;
    GLOBAL_STATE.elapsed = 0.0;
    GLOBAL_STATE.elapsed_samples = 0;
    
    info->state = &GLOBAL_STATE;
    
    
    // INITIALISE GLOBAL STATES 
    make_sine_table();
    init_random(RANDOM_SEED);
    
    // initialise sys_audio
    audio_stream = init_sys_audio(info);            
}


// shutdown the audio subsystem and close the audio stream
void shutdown_audio()
{
    shutdown_sys_audio(audio_stream);
}


// Start the audio playback
void start_audio(void)
{
    start_sys_audio(audio_stream);    
}

// Stop the audio playback
void stop_audio(void)
{
    stop_sys_audio(audio_stream);
}


int pump_audio(int synthesize)
{
    return pump_sys_audio(audio_stream, synthesize);
}



int buffers_remaining_audio(void)
{
    return buffers_remaining_sys_audio(audio_stream);
}