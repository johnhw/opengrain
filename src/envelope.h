/**    
    @file envelope.h
    @brief
    @author John Williamson
    
    Copyright (c) 2011 All rights reserved.
    Licensed under the BSD 3 clause license. See COPYING.
    
    This file is part of the OpenGrain distribution.
    http://opengrain.sourceforge.net   
*/              

#ifndef __ENVELOPE_H__
#define __ENVELOPE_H__
#include "audio.h"
#include <math.h>

#define ENVELOPE_TYPE_NONE 0
#define ENVELOPE_TYPE_EXP 1
#define ENVELOPE_TYPE_LINEAR 2
#define ENVELOPE_TYPE_SINC 3
#define ENVELOPE_TYPE_GAUSSIAN 4
#define ENVELOPE_TYPE_HAMMING 5
#define ENVELOPE_TYPE_FOF 6
#define ENVELOPE_TYPE_BETA 7






// negative attack/decay values indicate absolute times in seconds
// positive attack decay values indicate fractions of the duration

typedef struct Envelope
{
    int type;
    float attack; // attack/decay, where appropriate
    float decay;
    float shape; // shape of envelope, where appropriate
    float duration;     // duration in seconds    
    
    float phase;   
    float phase_samples;
    float phase_relative;
    
    
    int duration_samples;
    float attack_time, decay_time;
    float attack_stop, decay_stop;
    float coeff_a, coeff_b, coeff_c, coeff_d;
    
    int mid_samples;
    float sample_divide;
    
    float state_a, state_b;
} Envelope;

Envelope *create_envelope();
void destroy_envelope(Envelope *env);


void set_type_envelope(Envelope *env, int type);
void set_attack_envelope(Envelope *env, float attack);
void set_decay_envelope(Envelope *env, float decay);
void set_shape_a_envelope(Envelope *env, float shape);
void set_duration_envelope(Envelope *env, float duration);

void set_envelope(Envelope *envelope, int type, float attack, float decay, float shape, float duration);
void recompute_envelope(Envelope *env);

void retrigger_envelope(Envelope *env);
float compute_envelope(Envelope *env);
void envelope_buffer(Envelope *env, Buffer *buffer);

#endif