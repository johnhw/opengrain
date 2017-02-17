/**    
    @file moddelayline.h
    @brief
    @author John Williamson
    
    Copyright (c) 2011 All rights reserved.
    Licensed under the BSD 3 clause license. See COPYING.
    
    This file is part of the OpenGrain distribution.
    http://opengrain.sourceforge.net   
*/              

#ifndef __MODDELAYLINE_H__
#define __MODDELAYLINE_H__

#include "audio.h"


#define MODDELAY_INTERPOLATION_LINEAR 0
#define MODDELAY_INTERPOLATION_ALLPASS 1

typedef struct ModDelayLine
{
    float *samples;
    int n_samples;
    int max_n_samples;    
    int read_offset;
    int read_head;
    int write_head;
    
    // for modulation
    float read_fraction;
    int excursion;
    float phase;
    float modulation_frequency;
    float modulation_extent;    
    
    int interpolation_mode;
    
    int modulated;
    float allpass_a;
} ModDelayLine;






ModDelayLine *create_mdelay(void);


void set_interpolation_mode_mdelay(ModDelayLine *delay, int mode);
void set_modulation_mdelay(ModDelayLine *delay,  float modulation_extent, float modulation_frequency);
void destroy_mdelay(ModDelayLine *delay);
float mdelay(ModDelayLine *delay, float sample);
void mdelay_in(ModDelayLine *delay, float sample);
float mdelay_out(ModDelayLine *delay);
float unmodulated_mdelay_out(ModDelayLine *delay);
void set_mdelay(ModDelayLine *delay, int delay_length);
float get_mdelay(ModDelayLine *delay, int index);




#endif