/**    
    @file hrtf.h
    @brief Implements an head-related transfer function system for 3D audio rendering. Uses an FFT based convolution method.
    @author John Williamson
    
    Copyright (c) 2011 All rights reserved.
    Licensed under the BSD 3 clause license. See COPYING.
    
    This file is part of the OpenGrain distribution.
    http://opengrain.sourceforge.net   
*/              

#ifndef __HRTF_H__
#define __HRTF_H__
#include "audio.h"
#include "wavereader.h"
#include "complex_buffer.h"
#include "convolver.h"

#include <stdlib.h>
#include <math.h>



typedef struct StereoSound
{
    Buffer *left;
    Buffer *right;
    ComplexBuffer *left_fft;
    ComplexBuffer *right_fft;
    
} StereoSound;


typedef struct HRTFImpulse
{
    StereoSound *sound;
    float azimuth;
    float elevation;    
    
} HRTFImpulse;

#define HRTF_FIND_AZIMUTH_LESS_THAN 1 
#define HRTF_FIND_AZIMUTH_GREATER_THAN 2 
#define HRTF_FIND_ELEVATION_LESS_THAN 4
#define HRTF_FIND_ELEVATION_GREATER_THAN 8


HRTFImpulse *create_hrtf_impulse(WaveSound *sound, float azimuth, float elevation, FFT *fft, int switch_channels);
void destroy_hrtf_impulse(HRTFImpulse *impulse);


typedef struct HRTFModel
{   
    list_t *impulses;    
    int buffer_size;
    FFT *fft;
} HRTFModel;

HRTFModel *create_hrtf_model(char *path);
void load_MIT_hrtf_model(HRTFModel *model, char *path);
void  destroy_hrtf_model(HRTFModel *model);
HRTFImpulse *get_hrtf(HRTFModel *model, float azimuth, float elevation, int flags);
int get_hrtf_buffer_size(HRTFModel *model);
double great_circle_distance(float az1, float el1, float az2, float el2);

typedef struct HRTFConvolver
{
    HRTFModel *model;
    Convolver *left;
    Convolver *right;
    Convolver *old_left;
    Convolver *old_right;
    float interpolate;
    float interpolate_rate;
    Buffer *left_v1, *right_v1, *left_v2, *right_v2;
    ComplexBuffer *temp_1;
    ComplexBuffer *temp_2;
    float azimuth, elevation;
    
    
    
    
} HRTFConvolver;

void hrtf_convolve(HRTFConvolver *convolver, Buffer *in, Buffer *left_out, Buffer *right_out);
void set_interpolate_time_hrtf_convolver(HRTFConvolver *convolver, float time);
void set_hrtf_convolver(HRTFConvolver *model, float azimuth, float elevation);
void destroy_hrtf_convolver(HRTFConvolver *hrtfconvolver);
HRTFConvolver *create_hrtf_convolver(HRTFModel *model);









#endif