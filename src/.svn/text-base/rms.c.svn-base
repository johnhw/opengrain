/**    
    @file rms.c
    @brief
    @author John Williamson
    
    Copyright (c) 2011 All rights reserved.
    Licensed under the BSD 3 clause license. See COPYING.
    
    This file is part of the OpenGrain distribution.
    http://opengrain.sourceforge.net   
*/              

#include "rms.h"


// create an RMS tracking object
RMS *create_RMS()
{
    RMS *rms;
    rms = malloc(sizeof(*rms));
    rms->decay = 1.0;
    rms->level = 0.0;
    return rms;
}



// destroy the RMS object
void destroy_RMS(RMS *rms)
{
    free(rms);
}


// Set the decay time of the envelope follower
void set_time_RMS(RMS *rms, float time)
{
    rms->decay = exp(-1.0 / (time * GLOBAL_STATE.sample_rate));
}

// update the RMS power
void update_RMS(RMS *rms, float sample)
{
    rms->level = (1.0-rms->decay) * rms->level + (rms->decay)*sample*sample;
}

// compute the RMS level
float compute_RMS(RMS *rms)
{    
    rms->power = sqrt(rms->level);
    return rms->power;    
}

