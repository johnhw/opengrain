/**    
    @file sys_portaudio.h
    @brief
    @author John Williamson
    
    Copyright (c) 2011 All rights reserved.
    Licensed under the BSD 3 clause license. See COPYING.
    
    This file is part of the OpenGrain distribution.
    http://opengrain.sourceforge.net   
*/              

#ifndef __SYS_PORTAUDIO_H__
#define __SYS_PORTAUDIO_H__
#include "sys_audio.h"
#include <portaudio.h>
#include "pa_ringbuffer.h"
#include "audio.h"

/** @define size of the ringbuffer used for ferrying data to/from the audio thread.
    This should be at least 8 times the maximum frames per buffer * the maximum number of channels */
#define PORTAUDIO_RINGBUFFER_SIZE (65536*8*sizeof(float))

/** @struct A structure to hold the buffers and stream information
    for portaudio access */
typedef struct PortaudioInfo
{
    PaStream *stream;
    float *in_buffer;
    float *out_buffer;
    int in_block_size; // size of one buffer in bytes
    int out_block_size; // size of one buffer in bytes
    AudioInfo *audio_info;
    PaUtilRingBuffer *in_ringbuffer;
    PaUtilRingBuffer *out_ringbuffer;
    int ringbuffer_mode;
    char *in_ringbuffer_storage;    
    char *out_ringbuffer_storage;
    
} PortaudioInfo;

PortaudioInfo *create_portaudio_info(AudioInfo *info);
void destroy_portaudio_info(PortaudioInfo *info);

#endif




