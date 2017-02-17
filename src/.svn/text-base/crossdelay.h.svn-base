/**    
    @file crossdelay.h
    @brief
    @author John Williamson
    
    Copyright (c) 2011 All rights reserved.
    Licensed under the BSD 3 clause license. See COPYING.
    
    This file is part of the OpenGrain distribution.
    http://opengrain.sourceforge.net   
*/              

#ifndef __CROSS_DELAY_H__
#define __CROSS_DELAY_H__
#include "audio.h"
#include "delayline.h"
#include "moddelayline.h"
#include "biquad.h"



typedef struct CrossDelay
{    
    float *z;
    
    DelayLine **delays;
    Biquad **filters;
    
    int filtering;
    float feedback;
    float cross;
    float wet_level;
    float dry_level;    
    int channels;
} CrossDelay;

// Create a feedback delay unit
CrossDelay *create_cross_delay(int channels);

void set_length_cross_delay(CrossDelay *cross_delay, double delay);
void set_feedback_cross_delay(CrossDelay *cross_delay, double feedback, double cross);
void set_levels_cross_delay(CrossDelay *cross_delay, double wet, double dry);
void set_filter_cross_delay(CrossDelay *cross_delay, float freq, float q, int type);


void destroy_cross_delay(CrossDelay *cross_delay);

void process_cross_delay(CrossDelay *cross_delay, Buffer **ins, Buffer **outs);



#endif

