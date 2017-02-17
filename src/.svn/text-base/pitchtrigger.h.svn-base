/**    
    @file pitchtrigger.h
    @brief
    @author John Williamson
    
    Copyright (c) 2011 All rights reserved.
    Licensed under the BSD 3 clause license. See COPYING.
    
    This file is part of the OpenGrain distribution.
    http://opengrain.sourceforge.net   
*/              

#ifndef __PITCHTRIGGER_H__
#define __PITCHTRIGGER_H__
#include "audio.h"
#include "biquad.h"
#include "grain_stream.h"
#include "rms.h"
#include "complex_buffer.h"


#include <stdlib.h>
#include <math.h>


typedef struct PitchTrigger
{
    
    FFT *fft;
    ComplexBuffer *complex_buffer;
    Buffer *hps_buffer;
    Buffer *temp_buffer;
    Buffer *buffer;    
    int buffer_ptr;
    int window_size;
    
    float frequency, target_frequency, frequency_coeff;
    float phase;
 
    RMS *power_tracker;
    float min_level;
    int copy_amplitude;
    float copy_boost;
} PitchTrigger;

PitchTrigger *create_pitch_trigger(int window_size);

void set_minimum_level_pitch_trigger(PitchTrigger *trigger, float min_level);
void set_copy_amplitude_pitch_trigger(PitchTrigger *trigger, int copy_amplitude, float copy_boost);
void destroy_pitch_trigger(PitchTrigger *trigger);
void set_speed_pitch_trigger(PitchTrigger *trigger, float time);
void process_pitch_trigger(void *pitch_trigger, Buffer *input, GrainStream *stream);


#endif