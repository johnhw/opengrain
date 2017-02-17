/**    
    @file wavereader.h
    @brief
    @author John Williamson
    
    Copyright (c) 2011 All rights reserved.
    Licensed under the BSD 3 clause license. See COPYING.
    
    This file is part of the OpenGrain distribution.
    http://opengrain.sourceforge.net   
*/              

#ifndef __WAVEREADER_H__
#define __WAVEREADER_H__

#include "audio.h"
#include "soundfile.h"
#include <math.h>



#define PCM_SIGNED_8 0
#define PCM_UNSIGNED_8 1
#define PCM_SIGNED_16 2
#define PCM_SIGNED_32 4
#define PCM_FLOAT 5
#define PCM_DOUBLE 6


typedef struct WaveSound
{    
    int sample_rate;
    int n_channels;
    int frames;
    list_t *channels;
} WaveSound;



Buffer *get_channel_wave_sound(WaveSound *sound, int channel);

int wave_sound_get_raw_bytes(WaveSound *sound, int format);
void wave_sound_to_raw(WaveSound *sound, int format, void *bytes);

WaveSound *wave_sound_from_raw(void *bytes, int frames, int channels, int sample_rate, int format);
WaveSound *create_wave_sound(char *fname);
void destroy_wave_sound(WaveSound *wave_sound);
Buffer *resample_buffer(Buffer *in, double rate);
void resample_wave_sound(WaveSound *wave_sound, int new_rate);
list_t *load_soundbank(char *basename);

list_t *create_soundbank(char *basename);
void destroy_soundbank(list_t *bank);

#endif