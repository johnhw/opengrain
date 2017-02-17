/**    
    @file utils.h
    @brief
    @author John Williamson
    
    Copyright (c) 2011 All rights reserved.
    Licensed under the BSD 3 clause license. See COPYING.
    
    This file is part of the OpenGrain distribution.
    http://opengrain.sourceforge.net   
*/              

#ifndef __UTILS_H__
#define __UTILS_H__
#include <stdlib.h>
#include <math.h>

#define MIN(A,B) ((A<B) ? (A) : (B))
#define MAX(A,B) ((A>B) ? (A) : (B))


#define TO_DEGREES(X) (180.0*((X)/(M_PI)))
#define TO_RADIANS(X) (M_PI*((X)/(180.0)))

#define SECONDS_TO_SAMPLES(X) ((X)*(GLOBAL_STATE.sample_rate))
#define SAMPLES_TO_SECONDS(X) ((X)/((double)GLOBAL_STATE.sample_rate))
#define SEMITONES_TO_RATE(X) (pow(2.0,(X/12.0)))


void pan(float x, float pan, float *l, float *r);

float dB_to_gain(float dB);
float gain_to_dB(float gain);

float semitones_to_rate(float semitones);
float rate_to_semitones(float rate);

float octaves_to_rate(float octaves);
float rate_to_octaves(float rate);

float samples_to_seconds(float samples);
float seconds_to_samples(float seconds);

float rc_time(float time, float to_dB);
float rc_freq(float freq);



#define SINE_TABLE_SIZE 1024
extern float sine_table[SINE_TABLE_SIZE];


void make_sine_table();

void opengrain_error(char *error, ...);
void opengrain_warning(char *error, ...);

#endif