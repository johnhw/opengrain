/**    
    @file buffer.c
    @brief Provides a basic buffer of floating point values, and 
    functions for copying, clearing and mixing them.
    
    @author John Williamson
    
    Copyright (c) 2011 All rights reserved.
    Licensed under the BSD 3 clause license. See COPYING.
    
    This file is part of the OpenGrain distribution.
    http://opengrain.sourceforge.net   
*/              

#include "biquad.h"
#include "buffer.h"
#include <string.h>

/** Copy a buffer into another. 
    No bounds checking.
    @param a destination
    @param b source    
*/
void copy_buffer(Buffer *a, Buffer *b)
{
    int i;
    for(i=0;i<b->n_samples;i++)            
        a->x[i] = b->x[i];    
}


/** Duplicate an existing buffer (i.e. allocate a new buffer and copy data into it).
    @param a buffer to duplicate
    @return a newly allocated buffer object
*/
Buffer *duplicate_buffer(Buffer *a)
{
    Buffer *buffer;
    buffer = create_buffer(a->n_samples);
    copy_buffer(buffer, a);
    return buffer;        
}

/** linearly resamples a buffer. Only used if libresample is not available
    @param dest destination
    @param source source
    @param rate rate to resample at. >1.0 means downsample source, <1.0 means upsample source
*/
void linear_resample_buffer(Buffer *dest, Buffer *source, float rate)
{
    int i,j;
    float ptr, t;
           
    ptr = 0;
    for(i=0;i<dest->n_samples;i++)
    {
        ptr += rate;
        t = ptr - floor(ptr);
        j = (int)floor(ptr);
        
        // interpolate input 
        if(j<source->n_samples-1)
            dest->x[i] = (1-t)*source->x[j] + t*source->x[j+1];    
        else
            dest->x[i] = source->x[source->n_samples-1];
    }    
}


/** Create a buffer of a given length
    @param n_samples length of buffer to create
    @return a newly allocated buffer object
*/
Buffer *create_buffer(int n_samples)
{
    Buffer *buffer = malloc(sizeof(Buffer));
    buffer->n_samples = n_samples;
    buffer->x = calloc(sizeof(*(buffer->x)), n_samples);
    return buffer;
}

/** Destroy a buffer
    @param buffer a buffer to deallocate
*/
void destroy_buffer(Buffer *buffer)
{
    free(buffer->x);
    free(buffer);
}

/** Clear (silence) a buffer
    @param buffer the buffer to zero out 
*/
void zero_buffer(Buffer *buffer)
{
    memset(buffer->x, 0, sizeof((*buffer->x))*buffer->n_samples);
}

/** Hard clip a buffer to [-1, 1]
    @param buffer buffer to clip
*/
void clip_buffer(Buffer *buffer)
{
    int i;
    for(i=0;i<buffer->n_samples;i++)
    {   
        if(buffer->x[i]<-1.0)
            buffer->x[i] = -1.0;
        if(buffer->x[i]>1.0)
            buffer->x[i] = 1.0;                   
    }
}


/** Soft clip a buffer to [-1, 1]
    @param buffer buffer to clip
*/
void soft_clip_buffer(Buffer *buffer)
{
    int i;
    for(i=0;i<buffer->n_samples;i++)      
        buffer->x[i] = tanh(buffer->x[i]);           
}


/** Copy part of a buffer into part of another buffer
    @param a destination buffer
    @param offset_a offset into a to start writing into
    @param len_a amount of a to write into (usually == len_b)
    @param b source buffer
    @param offset_b offset to start reading from
    @param len_b amount of b to read
    */
void copy_buffer_partial(Buffer *a, int offset_a, int len_a, Buffer *b, int offset_b, int len_b)
{
    int i,len;
    len = MIN(len_a, len_b);       
    for(i=0;i<len;i++)    
        a->x[i+offset_a] = b->x[i+offset_b];
}



/** Mix a buffer into a smaller section of the destination buffer.
    @param dest destination buffer
    @param src destination buffer
    @param offset offset into the destination buffer
    @param len length of data to mix
*/
  
void mix_buffer_offset(Buffer *dest, Buffer *src, int offset, int len)
{
    int i;
    for(i=0;i<len;i++)            
        dest->x[offset+i] += src->x[i];    
}

/** mix a buffer into a smaller section of the destination buffer with a given weight
    @param dest destination buffer
    @param src destination buffer
    @param offset offset into the destination buffer
    @param len length of data to mix
    @param weight weighting of source to mix into dest
*/
void mix_buffer_offset_weighted(Buffer *dest, Buffer *src, int offset, int len, float weight)
{
   int i;
    for(i=0;i<len;i++)            
        dest->x[offset+i] += src->x[i] * weight;
}


/** mix a buffer into a destination buffer with a given weight
    @param dest destination buffer
    @param src destination buffer
    @param weight weighting of source to mix into dest
*/
void mix_buffer(Buffer *dest, Buffer *src, float weight)
{
    int i;
    int n;    
    n = MIN(dest->n_samples, src->n_samples);
    for(i=0;i<n;i++)            
        dest->x[i] += src->x[i]*weight;    
}

/** Apply a biquad to an entire buffer.
    @arg biquad The filter to apply.
    @arg buffer The buffer to apply it to.
*/
void biquad_buffer(Buffer *buffer, struct Biquad *biquad)
{
    int i;       
    reset_biquad(biquad);
    for(i=0;i<buffer->n_samples;i++)            
        buffer->x[i] = process_biquad(biquad, buffer->x[i]);

}

/** Scale a buffer by a scalar.
    @param buffer The buffer to rescale.
    @param weight The value to multiply each element of buffer by
    */
void scale_buffer(Buffer *buffer, float weight)
{
    int i;    
    for(i=0;i<buffer->n_samples;i++)                
        buffer->x[i] *= weight;
}