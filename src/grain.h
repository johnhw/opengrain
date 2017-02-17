/**    
    @file grain.h
    @brief
    @author John Williamson
    
    Copyright (c) 2011 All rights reserved.
    Licensed under the BSD 3 clause license. See COPYING.
    
    This file is part of the OpenGrain distribution.
    http://opengrain.sourceforge.net   
*/              

#ifndef __GRAIN_H__
#define __GRAIN_H__

#include "audio.h"
#include "distributions.h"
#include "envelope.h"

#include "location.h"

// time in seconds for the RMS power tracker to cutoff the grain
#define AMPLTIUDE_CUTOFF_TIME 0.1

#define TIME_MODE_RELATIVE 0
#define TIME_MODE_ABSOLUTE 1


struct GrainSource;

struct GrainSpatializer;


typedef struct Grain
{ 
    Envelope *envelope;
    float amplitude;
    
    float duration;
    int duration_samples;
    int samples_passed;
    void *specifics;
    int finished;   
    Location3D *location;
    float frequency;
   

    
    struct GrainSource *source;
} Grain;




struct GrainSource;
void finish_grain(Grain *grain);
Grain *create_grain(void);
void reset_grain(Grain *grain);
void destroy_grain(Grain *grain);

#endif