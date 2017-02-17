/**    
    @file simplewav.h
    @brief
    @author John Williamson
    
    Copyright (c) 2011 All rights reserved.
    Licensed under the BSD 3 clause license. See COPYING.
    
    This file is part of the OpenGrain distribution.
    http://opengrain.sourceforge.net   
*/              

#include <stdio.h>
#include "audio.h"
struct WaveSound;


typedef struct SimpleWavInfo
{
    FILE *wavfile;
    int n_channels;
    int sample_rate;
    int frames;
    unsigned char *output_buffer;
    int output_ptr;
    int output_len;
    int output_total;
    unsigned char header[44];    
} SimpleWavInfo;

void soundfile_sub_open(void *ptr, char *fname);
int get_little_endian(unsigned char *data, int n_bytes);
void little_endian(unsigned char *data, int n_bytes, int x);



