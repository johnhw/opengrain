/**    
    @file complex_buffer.c
    @brief
    @author John Williamson
    
    Copyright (c) 2011 All rights reserved.
    Licensed under the BSD 3 clause license. See COPYING.
    
    This file is part of the OpenGrain distribution.
    http://opengrain.sourceforge.net   
*/              

#include "complex_buffer.h"


// create a newly-allocated complex buffer of a given size
ComplexBuffer *create_complex_buffer(int size)
{
    ComplexBuffer *complex;
    complex = malloc(sizeof(*complex));
    
    complex->n_samples = size;
    complex->x = calloc(sizeof(*complex->x), size);
    return complex;
}

// destroy a complex buffer
void destroy_complex_buffer(ComplexBuffer *complex)
{
    free(complex->x);
    free(complex);
}

// clear a complex buffer
void zero_complex_buffer(ComplexBuffer *buffer)
{
    memset(buffer->x, 0, buffer->n_samples * sizeof(*buffer->x));
}


FFT *create_fft(int size)
{
    FFT *fft;
    fft = malloc(sizeof(*fft));
    fft->fft = kiss_fftr_alloc(size, 0, 0, 0);
    fft->ifft = kiss_fftr_alloc(size, 1, 0, 0);
    fft->n_samples = size;
    return fft;
}


void destroy_fft(FFT *fft)
{
    free(fft->fft);
    free(fft->ifft);
    free(fft);
}


// compute the fft of a real buffer
void fft_buffer(FFT *fft, Buffer *src, ComplexBuffer *dest)
{

    kiss_fftr(fft->fft, src->x, dest->x);
}

// compute the ifft of a complex buffer
void ifft_buffer(FFT *fft, ComplexBuffer *src, Buffer *dest)
{
    kiss_fftri(fft->ifft, src->x, dest->x);
}


// add a complex buffer into an existing complex buffer
void accumulate_complex_buffer(ComplexBuffer *dest, ComplexBuffer *src)
{
    int i;
    for(i=0;i<dest->n_samples;i++)
    {
        dest->x[i].r += src->x[i].r;    
        dest->x[i].i += src->x[i].i;
    }    
}


// scale a complex buffer by a scalar value
void scale_complex_buffer(ComplexBuffer *dest, float scale)
{
    int i;
    for(i=0;i<dest->n_samples;i++)
    {
        dest->x[i].r *= scale;    
        dest->x[i].i *= scale;
        
    }    
}


#define BLUR_KERNEL_SIZE 512
// randomize the phase of a complex buffer, and optionally part of the amplitude
void randomize_complex_buffer(ComplexBuffer *dest, float spectral_blurring)
{
    int i;
    double mag, angle, x;
    
    Buffer *amp, *blurred_amp, *gaussian;
    
    amp = create_buffer(dest->n_samples);
    gaussian = create_buffer(BLUR_KERNEL_SIZE);
    blurred_amp = create_buffer(dest->n_samples*2);
    
    // get the amplitude spectrum
    for(i=0;i<dest->n_samples;i++)
    {
        mag = sqrt(dest->x[i].r*dest->x[i].r + dest->x[i].i*dest->x[i].i);
        amp->x[i] = mag;
    }
    
    
    
    spectral_blurring = 0.5 + (BLUR_KERNEL_SIZE/2) * spectral_blurring;
    
    // create convolution kernel
    for(i=0;i<BLUR_KERNEL_SIZE;i++)
    {
        x = i - BLUR_KERNEL_SIZE/2;
        gaussian->x[i] = exp(-(x*x)/(2*spectral_blurring*spectral_blurring)) / sqrt(spectral_blurring*spectral_blurring*2*M_PI);
    }
    
    
    // blur the spectrum
    convolution(amp, gaussian, blurred_amp);
    
    // create the new spectrum, with blurred magnitude and randomized phase
    for(i=0;i<dest->n_samples;i++)
    {
   
        // ignore samples > length of the spectrum
        // and compensate for offset caused by convolution
        mag = blurred_amp->x[i+BLUR_KERNEL_SIZE/2];
        angle = uniform_double()*2*M_PI;
        
        dest->x[i].r = sin(angle) * mag;
        dest->x[i].i = cos(angle) * mag;                        
    }
    
    destroy_buffer(amp);
    destroy_buffer(gaussian);
    destroy_buffer(blurred_amp);
}



// linearly interpolate a pair of buffers into a destination buffer
void interpolate_complex_buffer(ComplexBuffer *dest, ComplexBuffer *src1, ComplexBuffer *src2, float t)
{
    int i;
    for(i=0;i<dest->n_samples;i++)
    {
        dest->x[i].i = (1-t)*src1->x[i].i + t*src2->x[i].i;
        dest->x[i].r = (1-t)*src1->x[i].r + t*src2->x[i].r;    
    }
}



// find the index of the biggest absolute value in an array
int find_absolute_peak(Buffer *buffer)
{
    float max, a;
    int i, max_index;
    max = 0.0;
    max_index = 0;
    for(i=1;i<buffer->n_samples;i++)
    {        
        a = abs(buffer->x[i]);
        if(a>max)        
        {
            max = a;            
            max_index = i;
        }
    }    
    return max_index;
}


// compute the product of the magnitude of the spectrum with shrunken versions
// of the spectrum, up to a factor of harmonics
void harmonic_product_spectrum(Buffer *dest, Buffer *temp, ComplexBuffer *src, int harmonics)
{
    int i,h,a;
    int index;
        
    // first "harmonic"
    for(i=0;i<dest->n_samples;i++)
    {        
        a = sqrt(src->x[i].i*src->x[i].i + src->x[i].r*src->x[i].r);                   
        dest->x[i] = a;
        temp->x[i] = a;
    }
        
    // for each harmonic
    for(h=2;h<harmonics;h++)
    {
        for(i=0;i<dest->n_samples/h;i++)
        {                    
            index = i*h;            
            dest->x[i] *= temp->x[index];
        }                      
    }
}


// element wise multiply a complex buffer
void multiply_complex_buffer(ComplexBuffer *dest, ComplexBuffer *src)
{
    int i;
    double cr,ci;
    for(i=0;i<dest->n_samples;i++)
    {
        cr = dest->x[i].r * src->x[i].r - dest->x[i].i * src->x[i].i;
        ci = dest->x[i].r * src->x[i].i + dest->x[i].i * src->x[i].r;
        dest->x[i].r = cr;
        dest->x[i].i = ci;    
    }
    

}



// convolve two sequences via the FFT
// does lots of allocation/deallocation, not good in a
// realtime block!
void convolution(Buffer *in, Buffer *impulse, Buffer *out)
{
    ComplexBuffer *cin_temp, *cimpulse_temp;
    Buffer *in_temp, *impulse_temp;
    FFT *fft;
    int len;
    
    // get length of buffers
    len = pow(2.0, ceil(log(in->n_samples+impulse->n_samples)/log(2.0)));
    
    // create temporary storage
    fft = create_fft(len);
    in_temp = create_buffer(len);
    impulse_temp = create_buffer(len);
    
    cin_temp = create_complex_buffer(len/2+1);
    cimpulse_temp = create_complex_buffer(len/2+1);
    
    // put the original signals in (with zero pad)
    zero_buffer(in_temp);
    zero_buffer(impulse_temp);
    mix_buffer(in_temp, in, 1.0);
    mix_buffer(impulse_temp, impulse, 1.0);
        
    // do the convolution
    fft_buffer(fft, in_temp, cin_temp);    
    fft_buffer(fft, impulse_temp, cimpulse_temp);        
    multiply_complex_buffer(cin_temp, cimpulse_temp);    
    scale_complex_buffer(cin_temp, 1.0/(len));
    ifft_buffer(fft, cin_temp, out);
    
    // free temporary storage
    destroy_fft(fft);
    destroy_buffer(in_temp);
    destroy_buffer(impulse_temp);
    destroy_complex_buffer(cin_temp);
    destroy_complex_buffer(cimpulse_temp);
    
    

}

