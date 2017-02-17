/**    
    @file audio.h
    @brief System independent audio handing code. Handles opening and closing of devices,
    starting and stopping devices and interrogating available devices
    
    @warning This file contains the only static global variable in OpenGrain: GLOBAL_STATE. 
    This represents the (read-only) state of the audio driver (channels, sample rate, etc.)
    
    @author John Williamson
    
    Copyright (c) 2011 All rights reserved.
    Licensed under the BSD 3 clause license. See COPYING.
    
    This file is part of the OpenGrain distribution.
    http://opengrain.sourceforge.net   
*/              

#ifndef __AUDIO_H__
#define __AUDIO_H__
#include "opengrain.h"
#include <stdlib.h>
#include "utils.h"
#include "random.h"
#include "buffer.h"
#include "simclist.h"

#define AUDIO_NO_INPUT 0
#define AUDIO_WITH_INPUT 1

#define OUTPUT_REALTIME_AUDIO 1
#define OUTPUT_WAVEFILE_AUDIO 2


typedef void (*AudioCallback)(void *, float *, float *);
typedef void (*AudioFinishedCallback)(void *);

typedef struct AudioState
{
    int n_channels;
    int n_input_channels;
    int input_channel;
    
    int in_device;
    int out_device;
    
    int sample_rate;
    int frames_per_buffer;
    
    float latency;
    // updated automatically in output.c
    double elapsed;
    int elapsed_samples;       
} AudioState;


typedef struct AudioInfo
{
    AudioCallback callback;
    AudioFinishedCallback finished_callback;    
    void *user_data;
    AudioState *state;
    
    
    void *stream;    
} AudioInfo;


extern AudioState GLOBAL_STATE;
void init_audio(AudioState *prototype, AudioCallback callback, AudioFinishedCallback finished_callback, void *stream_data);
void start_audio(void);
void stop_audio(void);
void shutdown_audio(void);
int pump_audio(int synthesize);
int buffers_remaining_audio(void);





#endif