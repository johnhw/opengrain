/**    
    @file output.h
    @brief
    @author John Williamson
    
    Copyright (c) 2011 All rights reserved.
    Licensed under the BSD 3 clause license. See COPYING.
    
    This file is part of the OpenGrain distribution.
    http://opengrain.sourceforge.net   
*/              
#ifndef __OUTPUT_H__
#define __OUTPUT_H__
#include "audio.h"
#include "grainmixer.h"
#include "random.h"
#include "wavewriter.h"
#include "trigger.h"



typedef struct OutputInfo
{
    Buffer *left, *right;
    Buffer *input;
    GrainMixer *mixer;
    int output_mode;
    WaveWriter *writer;
    list_t *live_triggers;    
} OutputInfo;

void audio_callback_output_info(void *data, float *in, float *out);
void audio_finished_output_info(void *data);
void fire_triggers_output_info(OutputInfo *info, float *in, int n_frames);

void fill_buffer_output_info(OutputInfo *info);
void set_output_mode_output_info(OutputInfo *info, int mode);

void connect_live_trigger(OutputInfo *info, Trigger *trigger);
void disconnect_live_trigger(OutputInfo *info, Trigger *trigger);


OutputInfo *create_output_info(void);
void destroy_output_info(OutputInfo *info);
WaveWriter *get_wavewriter_output_info(OutputInfo *info);

#endif