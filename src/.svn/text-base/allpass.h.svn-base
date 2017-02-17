/**    
    @file allpass.h
    @brief
    @author John Williamson
    
    Copyright (c) 2011 All rights reserved.
    Licensed under the BSD 3 clause license. See COPYING.
    
    This file is part of the OpenGrain distribution.
    http://opengrain.sourceforge.net   
*/              

#ifndef __ALLPASS_H__
#define __ALLPASS_H__
#include "audio.h"
#include "delayline.h"
#include <math.h>

typedef struct ThiranAllpass
{
    float x1, y1, tau, delay;
    DelayLine *integer_delay;    
    
    float maximum_delay;
} ThiranAllpass;


ThiranAllpass *create_thiran_allpass();
void destroy_thiran_allpass(ThiranAllpass *allpass);
void set_thiran_allpass_delay(ThiranAllpass *allpass, float delay);
float process_thiran_allpass(ThiranAllpass *allpass, float sample);
float get_thiran_allpass(ThiranAllpass *allpass);
void insert_thiran_allpass(ThiranAllpass *allpass, float sample);



#endif