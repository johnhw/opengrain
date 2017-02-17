/**    
    @file soundfile.h
    @brief
    @author John Williamson
    
    Copyright (c) 2011 All rights reserved.
    Licensed under the BSD 3 clause license. See COPYING.
    
    This file is part of the OpenGrain distribution.
    http://opengrain.sourceforge.net   
*/ 

#ifndef __SOUNDFILE_H__
#define __SOUNDFILE_H__
 
struct WaveSound;
struct WaveWriter;

struct WaveSound *soundfile_load(char *fname);
void *soundfile_open(int channels, int sample_rate, int bit_depth, int overwrite_mode, char *output_path_name);
void soundfile_stop(void *handle);
void soundfile_write(void *handle, float *data, int n);

#endif