/**    
    @file acoustics.h
    @brief
    @author John Williamson
    
    Copyright (c) 2011 All rights reserved.
    Licensed under the BSD 3 clause license. See COPYING.
    
    This file is part of the OpenGrain distribution.
    http://opengrain.sourceforge.net   
*/              

#ifndef __ACOUSTICS_H__
#define __ACOUSTICS_H__
#include "audio.h"
#include <stdlib.h>
#include <math.h>


#define ACOUSTICS_RANDOM_REVERB 1
#define ACOUSTICS_FDN_REVERB 2


typedef struct AcousticsChannel
{    
    RandomReverb *random_reverb;
    Convolver *fir_convolver;
    //FDN *fdn;
    int n_channels;
} AcousticsChannel;



AcousticsChannel *create_acoustics_channel(int n_channels);
void destroy_acoustics_channel(AcousticsChannel *channel);
void process_acoustics_channel(AcousticsChannel *channel, Buffer *in, Buffer **out);


typedef struct Acoustics
{
    AcousticsChannel *channel_a;
    AcousticsChannel *channel_b;
    AcousticsChannel *active_channel;
    float interpolate;
    float interpolate_rate;
    float interpolate_time;
    float wet, dry;
    float aux_level;
    Biquad *aux_filter;
    Buffer *mix_buffer;
    Buffer **channel_a_buffers;
    Buffer **channel_b_buffers;
    
    int channels;
} Acoustics;






Acoustics *create_acoustics(int n_channels);
void destroy_acoustics(Acoustics *acoustics);

void switch_active_acoustics(Acoustic *acoustics);

void set_interpolate_time_acoustics(Acoustics *acoustics, float time);
void set_reverb_levels_acoustics(Acoustics *acoustics, float dry, float wet);
void set_aux_level_acoustics(Acoustics *acoustics, float aux_level);
void set_aux_cutoff_acoustics(Acoustics *acoustics, float aux_cutoff);
void set_size_acoustics(Acoustics *acoustics, float size);
void set_impulse_acoustics(Acoustics *acoustics, Buffer *impulse);
void set_impulse_envelope_acoustics(Acoustics *acoustics, float decay_time);


void set_reverb_mode_acoustics(Acoustics *acoustics, int mode);
void set_input_diffusion_acoustics(Acoustics *acoustics, float diffusion_1, float diffusion_2);
void set_decay_diffusion_acoustics(Acoustics *acoustics, float diffusion_1, float diffusion_2);
void set_decay_acoustics(Acoustics *acoustics, float decay_time);
void set_bandwidth_acoustics(Acoustics *acoustics, float bandwidth_hz);
void set_predelay_acoustics(Acoustics *acoustics, float predelay);

void process_acoustics(Acoustics *acoustics, Buffer **in_channels, int in_channels, Buffer *aux, Buffer **out_channels);




#endif