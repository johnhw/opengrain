/**    
    @file convolver.h
    @brief
    @author John Williamson
    
    Copyright (c) 2011 All rights reserved.
    Licensed under the BSD 3 clause license. See COPYING.
    
    This file is part of the OpenGrain distribution.
    http://opengrain.sourceforge.net   
*/              

#ifndef __CONVOLVER_H__
#define __CONVOLVER_H__
#include "audio.h"
#include "wavereader.h"
#include "complex_buffer.h"


typedef struct Convolver
{
    Buffer *out;
    
    ComplexBuffer *impulse_fft;
    ComplexBuffer *in_fft;
    
    Buffer *in;
    Buffer *impulse;
    Buffer *overlap;
    
    FFT *fft;
    int len;
    int buffer_ptr;    
} Convolver;




typedef struct MultichannelConvolver
{
    Convolver **convolvers;    
    int channels;
} MultichannelConvolver;


Convolver *create_convolver();
void set_empty_impulse_convolver(Convolver *convolver, int size);
ComplexBuffer *get_fft_impulse_convolver(Convolver *convolver);

void set_impulse_convolver(Convolver *convolver, Buffer *impulse);
void load_impulse_from_file_convolver(Convolver *convolver, char *fname);
void destroy_convolver(Convolver *convolver);
void process_convolver(Convolver *convolver, Buffer *input, Buffer *output);




MultichannelConvolver *create_multichannel_convolver(int channels);
void set_common_impulse_multichannel_convolver(MultichannelConvolver *convolver, Buffer *impulse);
void set_independent_impulses_multichannel_convolver(MultichannelConvolver *convolver, Buffer **impulses);
void destroy_multichannel_convolver(MultichannelConvolver *convolver);
void process_multichannel_convolver(MultichannelConvolver *convolver, Buffer **ins, Buffer **outs);






void fft_convolution(Convolver *convolver);


#endif


