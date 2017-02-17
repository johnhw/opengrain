/**    
    @file grain_stream.h
    @brief
    @author John Williamson
    
    Copyright (c) 2011 All rights reserved.
    Licensed under the BSD 3 clause license. See COPYING.
    
    This file is part of the OpenGrain distribution.
    http://opengrain.sourceforge.net   
*/              

#ifndef __GRAIN_STREAM_H__
#define __GRAIN_STREAM_H__

#include "audio.h"
#include "reverb.h"
#include "svf.h"
#include "delayline.h"
#include "feedbackdelay.h"
#include "biquad.h"
#include "distributions.h"
#include "resonator_bank.h"
#include "envelope.h"
#include "crossdelay.h"
#include "grain_source.h"
#include "stream_fx.h"
#include "spatializer.h"
#include "convolver.h"
#include "grain_model.h"


#define DURATION_MODE_DETERMINISTIC
#define DURATION_MODE_POISSON

#define RATE_MODE_REGULAR 0
#define RATE_MODE_STOCHASTIC 1
#define RATE_MODE_TRIGGERED 2

#define TRIGGER_MODE_REGULAR 0
#define TRIGGER_MODE_STOCHASTIC 0



#define DURATION_INFINITE 1e20


typedef struct GrainStream
{        
    float time_until_next_grain;
    Spatializer *spatializer;
    Buffer *temp_grain;   
    float gain;
    float target_gain;
    float gain_coeff;
    list_t *source_list;    
    list_t *grain_list;
    list_t *dead_grain_list;
    StreamFX *fx;       
    GrainModel *model;
    int channels;
} GrainStream;


GrainModel *get_grain_model_stream(GrainStream *stream);
Grain *revive_grain_stream(GrainStream *stream);
void kill_grain_stream(GrainStream *stream, Grain *grain);
Spatializer *get_spatializer_stream(GrainStream *stream);
StreamFX *get_stream_fx_stream(GrainStream *stream);
void synthesize_stream(GrainStream *stream);

void set_gain_stream(GrainStream *stream, float gaindB);
void fade_gain_stream(GrainStream *stream, float gaindB, float time);
void add_grain_stream(GrainStream *stream, int when);
GrainStream *create_stream(int channels);
void destroy_stream(GrainStream *stream);
void sum_buffer_stream(GrainStream *stream, Buffer **outs);
void add_source_stream(GrainStream *stream, GrainSource *source);
void trigger_grains_stream(GrainStream *stream, int n_grains, double in_time, double after, int trigger_mode);
void trigger_single_grain_stream(GrainStream *stream, double in_time);
void auto_trigger_grains_stream(GrainStream *stream, Buffer *buffer);





#endif  