/**    
    @file sys_dummyaudio.c
    @brief A dummy sound driver implementation, which does nothing at all.
    @author John Williamson
    
    Copyright (c) 2011 All rights reserved.
    Licensed under the BSD 3 clause license. See COPYING.
    
    This file is part of the OpenGrain distribution.
    http://opengrain.sourceforge.net   
*/              

#include "sys_audio.h"
#include "audio.h"

const char *driver_name_sys_audio = "Dummy driver";

typedef struct DummyStream
{
    int running;
    void *user_data;
    AudioCallback callback;
    Buffer *in, *out;

} DummyStream;

// Shutdown the audio system
int shutdown_sys_audio(void *ptr)
{    
    DummyStream *stream = (DummyStream *)ptr;
    destroy_buffer(stream->in);
    destroy_buffer(stream->out);    
    free(stream);
}


int pump_sys_audio(void *ptr)
{
    DummyStream *stream = (DummyStream *)ptr;
    stream->callback(stream->user_data, stream->in->x, stream->out->x);
}


void start_sys_audio(void *ptr)
{
    DummyStream *stream = (DummyStream *)ptr;
    stream->running = 1;
}

void stop_sys_audio(void *ptr)
{
    DummyStream *stream = (DummyStream *)ptr;
    stream->running = 0;
}


// start a sys_audio stream with the given sample rate, on the given device
void *init_sys_audio(AudioInfo *info)
{
    DummyStream *stream;
    printf("Started the dummy audio stream. You won't hear anything with this driver!\n");
    stream = malloc(sizeof(*stream));
    stream->running = 0;
    stream->callback = info->callback;
    stream->user_data = info->user_data;
    
    // some temporary space to read/write data
    stream->in = create_buffer(info->state->frames_per_buffer * info->state->n_input_channels);
    stream->out = create_buffer(info->state->frames_per_buffer * info->state->n_channels);
    
    return stream;
}

void pre_init_sys_audio(void)
{
    // nothing to do...
}


void post_shutdown_sys_audio(void)
{
    // nothing to do...
}

int get_n_devices_sys_audio(void)
{
    return 1;
}


char *get_name_sys_audio(int device)
{
    return "DUMMY: Do-nothing audio device";    
}


int get_max_in_channels_sys_audio(int device)
{
    return 2;
}

int get_max_out_channels_sys_audio(int device)
{
    return 2;
}


int get_default_output_device_sys_audio(void)
{
    return 0;
}

int get_default_input_device_sys_audio(void)
{
    return 0;
}

int supports_sample_rate_sys_audio(int device, int sample_rate, int for_input)
{
    return 1; // we support every kind of audio format!
}

