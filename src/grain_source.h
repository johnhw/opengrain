/**    
    @file grain_source.h
    @brief
    @author John Williamson
    
    Copyright (c) 2011 All rights reserved.
    Licensed under the BSD 3 clause license. See COPYING.
    
    This file is part of the OpenGrain distribution.
    http://opengrain.sourceforge.net   
*/              

#ifndef __GRAIN_SOURCE_H__
#define __GRAIN_SOURCE_H__

#include "audio.h"
#include "distributions.h"
#include "envelope.h"
#include "grain.h"





typedef void *(*create_grain_func)(void *);
typedef void (*init_grain_func)(void *, void *, Grain *);
typedef void (*destroy_grain_func)(void *);
typedef void (*fill_grain_func)(void *, Buffer *);


typedef struct GrainSource
{    
    void *source_data;       
    // function pointers for generating new grains and filling buffers from grains
    
    // returns a specific active grain given the source data
    create_grain_func create_grain;
    init_grain_func init_grain;
    destroy_grain_func destroy_grain;
    
    // add a grain into a pair of existing buffer
    fill_grain_func fill_grain;
    int valid; // true when the source has been set
    list_t *specifics_list;        
    list_t *dead_specifics_list;                
} GrainSource;




void *revive_specifics_source(GrainSource *source, Grain *grain);
void kill_specifics_source(GrainSource *source, void *specifics);



GrainSource *create_grain_source(void);
void set_grain_source(GrainSource *source, create_grain_func create_func, init_grain_func init_func, destroy_grain_func destroy_func, fill_grain_func fill_func, void *source_data);
void destroy_source(GrainSource *source);


#endif