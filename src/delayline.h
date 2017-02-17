/**    
    @file delayline.h
    @brief
    @author John Williamson
    
    Copyright (c) 2011 All rights reserved.
    Licensed under the BSD 3 clause license. See COPYING.
    
    This file is part of the OpenGrain distribution.
    http://opengrain.sourceforge.net   
*/              

#ifndef __DELAYLINE_H__
#define __DELAYLINE_H__

#include "audio.h"


typedef struct DelayLine
{
    float *samples;
    int n_samples;
    int max_n_samples;    
    
    int read_head;
    int write_head;
        
} DelayLine;


DelayLine *create_delay(void);
void destroy_delay(DelayLine *delay);

float delay(DelayLine *delay, float sample);
void delay_in(DelayLine *delay, float sample);
float delay_out(DelayLine *delay);
void set_delay(DelayLine *delay, int delay_length);
float get_delay(DelayLine *delay, int index);
void expand_delay(DelayLine *delay, int samples);




#endif