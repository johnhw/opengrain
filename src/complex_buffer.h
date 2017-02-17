/**    
    @file complex_buffer.h
    @brief
    @author John Williamson
    
    Copyright (c) 2011 All rights reserved.
    Licensed under the BSD 3 clause license. See COPYING.
    
    This file is part of the OpenGrain distribution.
    http://opengrain.sourceforge.net   
*/              

#ifndef __COMPLEX_BUFFER_H__
#define __COMPLEX_BUFFER_H__

#include "audio.h"
#include "kiss_fftr.h"
#include "kiss_fft.h"


typedef struct ComplexBuffer
{
    int n_samples;
    kiss_fft_cpx *x;
} ComplexBuffer;


typedef struct FFT
{
    kiss_fftr_cfg fft;
    kiss_fftr_cfg ifft;
    int n_samples;
} FFT;


FFT *create_fft(int size);
void destroy_fft(FFT *fft);


void fft_buffer(FFT *fft, Buffer *src, ComplexBuffer *dest);
void ifft_buffer(FFT *fft, ComplexBuffer *src, Buffer *dest);


ComplexBuffer *create_complex_buffer(int size);
void destroy_complex_buffer(ComplexBuffer *buffer);
void zero_complex_buffer(ComplexBuffer *buffer);

void interpolate_complex_buffer(ComplexBuffer *dest, ComplexBuffer *src1, ComplexBuffer *src2, float t);


void accumulate_complex_buffer(ComplexBuffer *dest, ComplexBuffer *src);
void scale_complex_buffer(ComplexBuffer *buffer, float scale);
void randomize_complex_buffer(ComplexBuffer *dest, float noise_level);

void convolution(Buffer *in, Buffer *impulse, Buffer *out);
void harmonic_product_spectrum(Buffer *dest, Buffer *temp, ComplexBuffer *src, int harmonics);
void convolution(Buffer *in, Buffer *impulse, Buffer *out);

void multiply_complex_buffer(ComplexBuffer *dest, ComplexBuffer *src);
int find_absolute_peak(Buffer *buffer);

#endif