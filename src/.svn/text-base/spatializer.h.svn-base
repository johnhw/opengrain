/**    
    @file spatializer.h
    @brief
    @author John Williamson
    
    Copyright (c) 2011 All rights reserved.
    Licensed under the BSD 3 clause license. See COPYING.
    
    This file is part of the OpenGrain distribution.
    http://opengrain.sourceforge.net   
*/              

#ifndef __SPATIALIZER_H__
#define __SPATIALIZER_H__
#include "audio.h"
#include "biquad.h"
#include "grain.h"
#include "location.h"
#include "hrtf.h"
#include <stdlib.h>
#include <math.h>

// just distance attenuation
#define SPATIALIZATION_MONO 0 
 // just panning + attenuation
#define SPATIALIZATION_PAN 1
 // panning + delay + attenuation
#define SPATIALIZATION_PAN_ITD 2
 // panning + delay + filtering + attenuation
#define SPATIALIZATION_3D_FILTERING 3
 // HRTF spatialization + attenuation
#define SPATIALIZATION_3D_HRTF 4 
// level/delay per speaker. no filtering.
#define SPATIALIZATION_MULTICHANNEL 5


#define SPATIALIZATION_PER_STREAM 0
#define SPATIALIZATION_PER_GRAIN 1

// max itd = 0.66ms (30 samples at 44100Hz)
// distances in m
// speed of sound = 343.2 m/s
// 1/r falloff
// attenuation at 1km 1000Hz = -5.6, 4000Hz =-105   


// freq falloff in dB/m = 1.449e-9*f*f + 1.302e-6*f + 1.778e-3


typedef struct Spatializer
{    
    
    double distance_filter_factor;
    
    // 1.0 = units of meters
    double distance_delay_factor;    
    double distance_attenuation_factor; 
    double itd_factor;    
    double head_damp_factor; 
    double head_amplitude_factor; 
        
    Biquad *distance_filter_1_left;    
    Biquad *distance_filter_2_left;    
    Biquad *distance_filter_1_right;    
    Biquad *distance_filter_2_right;     
    
    
    HRTFConvolver *hrtf;
            
    int spatialization_mode;
    int global_mode;
    
    int spatializing;
    
    Buffer *left, *right, *left_distance, *right_distance, *mono, *reverb;        
    Buffer *left_excess, *right_excess;
    Buffer *left_distance_excess, *right_distance_excess;
        
        
    // global spatialization    
    Location3D *stream_location;
    
    // for multichannel speakers
    list_t *speaker_locations;
    list_t *channels;
    int n_channels;
    list_t *channel_excesses;
    
    // transformation matrix
    Matrix3D *matrix;
    
    // pointer to the world transformation matrix
    Matrix3D *world_matrix;
    
    Matrix3D *working_matrix;
} Spatializer;

Buffer *get_channel_spatializer(Spatializer *spatializer, int i);
int get_n_channels_spatializer(Spatializer *spatializer);
int get_sample_delay_spatializer(Spatializer *spatializer, float distance);
void set_multichannel_spatializer(Spatializer *spatializer, list_t *speaker_locations);
void start_spatializer(Spatializer *spatializer);
void stop_spatializer(Spatializer *spatializer);
void set_world_matrix_spatializer(Spatializer *spatializer, Matrix3D *matrix);
void create_split_buffer(int n, Buffer **first, Buffer **second);
Location3D *get_location_spatializer(Spatializer *spatializer);
Matrix3D *get_matrix_spatializer(Spatializer *spatializer);

Spatializer *create_spatializer();
void destroy_spatializer(Spatializer *spatializer);
void set_spatializer_mode(Spatializer *spatializer, int mode, int per_stream);
void spatialize(Spatializer *spatializer, Location3D *location, float amplitude, Buffer *mono,  int offset, int len);
void set_hrtf_spatializer(Spatializer *spatializer, HRTFModel *model);

#endif