/**    
    @file chorus.h
    @brief Simple chorus/flanger with a modulating delay line
    @author John Williamson
    
    Copyright (c) 2011 All rights reserved.
    Licensed under the BSD 3 clause license. See COPYING.
    
    This file is part of the OpenGrain distribution.
    http://opengrain.sourceforge.net   
*/              

#ifndef __CHORUS_H__
#define __CHORUS_H__
#include "audio.h"
#include "moddelayline.h"

typedef struct Chorus
{
    ModDelayLine *delay;
    float blend, feedback, feedforward;
} Chorus;



Chorus *create_chorus();
void destroy_chorus(Chorus *chorus);
void set_chorus(Chorus *chorus, float delay, float mod_freq, float mod_depth, float blend, float feedback, float feedforward);
void set_chorus_feedback(Chorus *chorus, float feedback);
void set_chorus_blend(Chorus *chorus, float blend);
void set_chorus_feedforward(Chorus *chorus, float feedforward);
void set_chorus_delay(Chorus *chorus, float delay);
void set_chorus_modulation(Chorus *chorus, float mod_freq, float mod_extent);
float process_chorus(Chorus *chorus, float x);


typedef struct MultichannelChorus
{
    Chorus **choruses;
    int channels;
} MultichannelChorus;

MultichannelChorus *create_multichannel_chorus(int channels);
void destroy_multichannel_chorus(MultichannelChorus *chorus);
void set_multichannel_chorus(MultichannelChorus *chorus, float delay, float mod_freq, float mod_depth, float blend, float feedback, float feedforward);
void set_multichannel_chorus_feedback(MultichannelChorus *chorus, float feedback);
void set_multichannel_chorus_blend(MultichannelChorus *chorus, float blend);
void set_multichannel_chorus_feedforward(MultichannelChorus *chorus, float feedforward);
void set_multichannel_chorus_delay(MultichannelChorus *chorus, float delay);
void set_multichannel_chorus_modulation(MultichannelChorus *chorus, float mod_freq, float mod_extent);
void process_multichannel_chorus(MultichannelChorus *chorus, Buffer **ins, Buffer **outs);



#endif