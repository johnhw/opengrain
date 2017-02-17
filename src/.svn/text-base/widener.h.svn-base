/**    
    @file widener.h
    @brief
    @author John Williamson
    
    Copyright (c) 2011 All rights reserved.
    Licensed under the BSD 3 clause license. See COPYING.
    
    This file is part of the OpenGrain distribution.
    http://opengrain.sourceforge.net   
*/              

#ifndef __WIDENER_H__
#define __WIDENER_H__
#include "audio.h"
#include "delayline.h"
#include <math.h>

typedef struct Widener
{
    DelayLine *left_delay, *right_delay;    
    float delay_length;
    float mix_amount;
} Widener;

Widener *create_widener();
void destroy_widener(Widener *widener);
void set_widener(Widener *widener, float delay, float mix);
void compute_widener(Widener *widener, float l, float r, float *l_out, float *r_out);


#endif