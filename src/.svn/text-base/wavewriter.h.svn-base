/**    
    @file wavewriter.h
    @brief
    @author John Williamson
    
    Copyright (c) 2011 All rights reserved.
    Licensed under the BSD 3 clause license. See COPYING.
    
    This file is part of the OpenGrain distribution.
    http://opengrain.sourceforge.net   
*/              

#ifndef __WAVEWRITER_H__
#define __WAVEWRITER_H__

#include "audio.h"
#include "soundfile.h"
#include "location.h"
#include <math.h>


// stereo output modes



#define MULTICHANNEL_INTERLEAVE 0
#define MULTICHANNEL_SEPARATE 1
#define MULTICHANNEL_MIX 2
#define MULTICHANNEL_HRTF 3

// file overwriting modes
#define OVERWRITE_MODE_OVERWRITE 0
#define OVERWRITE_MODE_APPEND 1
#define OVERWRITE_MODE_NEW_FILE 2



// bit depths
#define WAVEWRITER_BIT_DEPTH_8 8
#define WAVEWRITER_BIT_DEPTH_16 16
#define WAVEWRITER_BIT_DEPTH_24 24
#define WAVEWRITER_BIT_DEPTH_FLOAT 32


typedef struct WaveWriter
{
    int channel_mode;
    int bit_depth;
    int overwrite_mode;
    char *path;
    int writing;
    
    list_t *handles;
    
    list_t *speaker_locations;
    int channels;
    int out_channels;
    
    Buffer *write_buffer;
    
} WaveWriter;



WaveWriter* create_wavewriter(void);

void set_speaker_locations_wavewriter(WaveWriter *writer, list_t *locations);
void destroy_wavewriter(WaveWriter *writer);
void set_path_wavewriter(WaveWriter *writer, char *path);
void set_multichannel_wavewriter(WaveWriter *writer, int channels, int channel_mode);
void set_bit_depth_wavewriter(WaveWriter *writer, int bit_depth);
void set_overwrite_wavewriter(WaveWriter *writer, int overwrite_mode);
void start_wavewriter(WaveWriter *writer);
void write_wavewriter(WaveWriter *writer, Buffer **channels);
void stop_wavewriter(WaveWriter *writer);
int file_exists(char *path);

#endif