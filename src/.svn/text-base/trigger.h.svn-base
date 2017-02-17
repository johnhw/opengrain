/**    
    @file trigger.h
    @brief
    @author John Williamson
    
    Copyright (c) 2011 All rights reserved.
    Licensed under the BSD 3 clause license. See COPYING.
    
    This file is part of the OpenGrain distribution.
    http://opengrain.sourceforge.net   
*/              

#ifndef __TRIGGER_H__
#define __TRIGGER_H__
#include "audio.h"
#include "wavereader.h"
#include "grain_stream.h"
#include "pa_ringbuffer.h"

#include <stdlib.h>
#include <math.h>

#define TRIGGER_FROM_WAVE 0
#define TRIGGER_FROM_RINGBUFFER 1



typedef  void(*TriggerProcessCallback)(void *, Buffer *, GrainStream *);

typedef struct Trigger
{
    WaveSound *sound;
    int mode;
    Buffer *input;
    TriggerProcessCallback process_callback;
    PaUtilRingBuffer *ringbuffer; // for realtime
    char *ringdata; // for ringbuffer
    void *trigger_data;
    GrainStream *grain_stream;
    int wave_input_phase;    
    int loop;
} Trigger;



Trigger *create_trigger(void);
void destroy_trigger(Trigger *trigger);
void set_processor_trigger(Trigger *trigger, TriggerProcessCallback callback, void *trigger_data);
void set_grain_stream_trigger(Trigger *trigger, GrainStream  *stream);
void set_mode_trigger(Trigger *trigger, int mode, int loop);
void set_wave_trigger(Trigger *trigger, WaveSound *sound);
PaUtilRingBuffer *get_ringbuffer_trigger(Trigger *trigger);
void process_trigger(Trigger *trigger);



#endif