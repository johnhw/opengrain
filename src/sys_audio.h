/**    
    @file sys_audio.h
    @brief
    @author John Williamson
    
    Copyright (c) 2011 All rights reserved.
    Licensed under the BSD 3 clause license. See COPYING.
    
    This file is part of the OpenGrain distribution.
    http://opengrain.sourceforge.net   
*/              

#include "opengrain.h"


struct AudioInfo;

#define SYSAUDIO_WRITE_SUCCEEDED 1
#define SYSAUDIO_STILL_PLAYING 2


// all audio implementations need to implement these methods
void *init_sys_audio(struct AudioInfo *info);
int shutdown_sys_audio(void *stream);
void start_sys_audio(void *stream);
void stop_sys_audio(void *stream);

void pre_init_sys_audio(void);
void post_shutdown_sys_audio(void);

// only for audio systems which need the buffers to be manually triggered
// forces one buffer to be put through
int pump_sys_audio(void *stream, int synthesize);
int buffers_remaining_sys_audio(void *ptr);

// device interrogation
int get_n_devices_sys_audio(void);
char *get_name_sys_audio(int device);
int get_max_in_channels_sys_audio(int device);
int get_max_out_channels_sys_audio(int device);
int get_default_output_device_sys_audio(void);
int get_default_input_device_sys_audio(void);
int supports_sample_rate_sys_audio(int device, int sample_rate, int for_input);



extern const char *driver_name_sys_audio;