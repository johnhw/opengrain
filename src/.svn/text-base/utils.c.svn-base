/**    
    @file utils.c
    @brief
    @author John Williamson
    
    Copyright (c) 2011 All rights reserved.
    Licensed under the BSD 3 clause license. See COPYING.
    
    This file is part of the OpenGrain distribution.
    http://opengrain.sourceforge.net   
*/              

#include "utils.h"
#include <stdarg.h>
#include "audio.h"

// GLOBAL sine table
float sine_table[1024];           // sine look up table


// Pan a mono signal to two channels with constant power (-3dB panning law)
void pan(float x, float pan, float *l, float *r)
{
    pan = (pan+1.0)/2.0;
    *l = x * sin((1-pan)*M_PI/2.0);
    *r = x * sin((pan)*M_PI/2.0);
}

// convert decibels to a gain factor
float dB_to_gain(float dB)
{
    return pow(10, dB/20.0);

}

// convert a gain factor to decibels
float gain_to_dB(float gain)
{
    return 20.0*(log(gain)/log(10.0));

}


// convert semitones to a ratio
float semitones_to_rate(float semitones)
{
    return pow(2.0,(semitones/12.0));
}


// covert a ratio to semitones
float rate_to_semitones(float rate)
{
    return (log(rate)/log(2)) * 12.0;
}


// convert a  number of octaves (postive or negative) to a ratio
float octaves_to_rate(float octaves)
{
    return pow(2.0, octaves);
}


// convert a ratio to a number of octaves
float rate_to_octaves(float rate)
{
    return (log(rate)/log(2));
}


// convert samples to seconds at the current sample rate
float samples_to_seconds(float samples)
{
    return samples / (float)GLOBAL_STATE.sample_rate;
}

// convert seconds to samples at the current sample rate
float seconds_to_samples(float seconds)
{
    return seconds * (float)GLOBAL_STATE.sample_rate;
}

// return the coefficient of an one-pole filter that takes time seconds to decay to to_dB (must be negative!)
float rc_time(float time, float to_dB)
{
    float c, tau;
    
    if(time<1e-6)
        time = 1e-6;
    c   = pow(10.0, to_dB/20.0);  
    tau = time / -log(c); 
    return exp(-1.0 / (tau*GLOBAL_STATE.sample_rate));
}

// return the coefficient of an one-pole filter that has a -3dB frequency of freq
float rc_freq(float freq)
{
    return exp(-2.0 * M_PI * (freq/GLOBAL_STATE.sample_rate));
}


// Make a fixed table of sine values
void make_sine_table()
{
    int i;
    double a;
    for(i=0;i<SINE_TABLE_SIZE;i++)
    {
        a = i/((double)SINE_TABLE_SIZE) * 2 * M_PI;
        sine_table[i] = sin(a);    
    }
}




void opengrain_error(char *error, ...)
{
    va_list args;
    va_start(args, error);
    vfprintf(stderr, error, args);
    fprintf(stderr, "\n");
    va_end(args);
    exit(-1);
}

void opengrain_warning(char *error, ...)
{
    va_list args;
    va_start(args, error);
    vfprintf(stderr, error, args);
    fprintf(stderr, "\n");
    va_end(args);    
}
