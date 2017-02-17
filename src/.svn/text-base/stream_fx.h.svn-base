/**    
    @file stream_fx.h
    @brief
    @author John Williamson
    
    Copyright (c) 2011 All rights reserved.
    Licensed under the BSD 3 clause license. See COPYING.
    
    This file is part of the OpenGrain distribution.
    http://opengrain.sourceforge.net   
*/              

#ifndef __STREAM_FX_H__
#define __STREAM_FX_H__
#include "audio.h"
#include "svf.h"
#include "delayline.h"
#include "feedbackdelay.h"
#include "biquad.h"
#include "resonator_bank.h"
#include "crossdelay.h"
#include "grain_source.h"
#include "convolver.h"
#include "chorus.h"


typedef struct StreamFX
{    
    
    // stream effects
    // Filter settings
    int filter_enabled;
    MultichannelSVF *filters;
    
    // Delay settings
    int delay_enabled;
    CrossDelay *delays;
    
    // Resonator settings
    int resonators_enabled;
    MultichannelResonatorBank *resonators;
    
    MultichannelChorus *choruses;
    int chorus_enabled;
    
    int convolver_enabled;
    MultichannelConvolver *convolvers;
    
    int channels;
} StreamFX;


StreamFX *create_stream_fx(int channels);
void destroy_stream_fx(StreamFX *fx);

CrossDelay *get_delay_stream_fx(StreamFX *fx);
MultichannelSVF *get_filter_stream_fx(StreamFX *fx);
MultichannelConvolver *get_convolver_stream_fx(StreamFX *fx);
MultichannelChorus *get_chorus_stream_fx(StreamFX *fx);
MultichannelResonatorBank *get_resonator_stream_fx(StreamFX *fx);

void enable_chorus_stream_fx(StreamFX *fx);
void disable_chorus_stream_fx(StreamFX *fx);

void enable_convolver_stream_fx(StreamFX *fx);
void disable_convolver_stream_fx(StreamFX *fx);

void enable_filter_stream_fx(StreamFX *fx);
void disable_filter_stream_fx(StreamFX *fx);

void enable_delay_stream_fx(StreamFX *fx);
void disable_delay_stream_fx(StreamFX *fx);

void enable_resonators_stream_fx(StreamFX *fx);
void disable_resonators_stream_fx(StreamFX *fx);

void compute_stream_fx(StreamFX *fx, Buffer **ins, Buffer **outs);




#endif