/**    
    @file base_api.h
    @brief Implementation of the basic parts of the OpenGrain API (init, shutdown, error reporting)
    @author John Williamson
    
    Copyright (c) 2011 All rights reserved.
    Licensed under the BSD 3 clause license. See COPYING.
    
    This file is part of the OpenGrain distribution.
    http://opengrain.sourceforge.net   
*/             

#ifndef __BASE_API_H__
#define __BASE_API_H__

#include "gr.h"
#include "../audio.h"
#include "gr_errors.h"
#include "../output.h"

struct GrainMixer;

/** @struct GRContext
    Structure which holds complete state of the current OpenGrain context.
**/
typedef struct GRContext
{
    /* Error reporting */
    int last_error;    
    char last_error_string[GR_MAX_ERROR_STRING_LENGTH+1];
    
    /* Global flags */
    int global_flags[GR_MAX_FLAGS];
    
    /* Audio state for the device */
    AudioState *prototype;        
    OutputInfo *output_info;
    int pump_mode;
    
    /* For pumping via threads (system-dependent implementation) */
    void *pump_thread_handle;
    void *pump_thread_data;
    
    /* Mixer */
    struct GrainMixer *mixer;
    
} GRContext;

extern GRContext *gr_context;



#endif
