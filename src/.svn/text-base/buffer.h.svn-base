/**    
    @file buffer.h
    @brief Provides a basic buffer of floating point values, and 
    functions for copying, clearing and mixing them.
    
    @author John Williamson
    
    Copyright (c) 2011 All rights reserved.
    Licensed under the BSD 3 clause license. See COPYING.
    
    This file is part of the OpenGrain distribution.
    http://opengrain.sourceforge.net   
*/              

#ifndef __BUFFER_H__
#define __BUFFER_H__

#include "audio.h"

/** @struct Buffer A buffer with a length and an array of floating point values */
typedef struct Buffer
{
    float *x;    
    int n_samples;
} Buffer;

struct Biquad;

Buffer *create_buffer(int n_samples);
void destroy_buffer(Buffer *buffer);
void copy_buffer(Buffer *dest, Buffer *src);
void clip_buffer(Buffer *buffer);
void soft_clip_buffer(Buffer *buffer);
void zero_buffer(Buffer *buffer);
void mix_buffer(Buffer *dest, Buffer *src, float weight);
void mix_buffer_offset(Buffer *dest, Buffer *src, int offset, int len);
void mix_buffer_offset_weighted(Buffer *dest, Buffer *src, int offset, int len, float weight);
void linear_resample_buffer(Buffer *dest, Buffer *source, float rate);
Buffer *duplicate_buffer(Buffer *a);
void copy_buffer_partial(Buffer *a, int offset_a, int len_a, Buffer *b, int offset_b, int len_b);
void scale_buffer(Buffer *buffer, float weight);
void biquad_buffer(Buffer *buffer, struct Biquad *biquad);

#endif