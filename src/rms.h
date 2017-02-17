/**    
    @file rms.h
    @brief
    @author John Williamson
    
    Copyright (c) 2011 All rights reserved.
    Licensed under the BSD 3 clause license. See COPYING.
    
    This file is part of the OpenGrain distribution.
    http://opengrain.sourceforge.net   
*/              

#ifndef __RMS_H__
#define __RMS_H__
#include "audio.h"
#include <stdlib.h>
#include <math.h>

typedef struct RMS
{
    float decay;
    float level;
    float power;
} RMS;

RMS *create_RMS();
void set_time_RMS(RMS *rms, float time);
void destroy_RMS(RMS *rms);
void update_RMS(RMS *rms, float sample);
float compute_RMS(RMS *rms);


#endif