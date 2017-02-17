/**    
    @file convolver.c
    @brief
    @author John Williamson
    
    Copyright (c) 2011 All rights reserved.
    Licensed under the BSD 3 clause license. See COPYING.
    
    This file is part of the OpenGrain distribution.
    http://opengrain.sourceforge.net   
*/              

#include "convolver.h"


Convolver *create_convolver()
{
    Convolver *convolver;
    convolver = malloc(sizeof(*convolver));    
    convolver->impulse = NULL;
    convolver->in = NULL;
    convolver->out = NULL;
    convolver->overlap = NULL;
    convolver->impulse_fft = NULL;
    convolver->in_fft = NULL;
    convolver->fft = NULL;
    return convolver;
}



// set a blank convolver impulse
void set_empty_impulse_convolver(Convolver *convolver, int size)
{
    Buffer *buffer;
    buffer = create_buffer(size);
    zero_buffer(buffer);
    set_impulse_convolver(convolver, buffer);
    destroy_buffer(buffer);

}


// returns the impulse fft, so it can be modified
ComplexBuffer *get_fft_impulse_convolver(Convolver *convolver)
{
    return convolver->impulse_fft;
    
}


// set the impulse response from a convolver
void set_impulse_convolver(Convolver *convolver, Buffer *impulse)
{
    int impulse_len;
    
    // find power of 2 just bigger than the impulse
    impulse_len = pow(2.0, ceil(log(impulse->n_samples)/log(2.0)));
        
    // remove old buffers if they exist and are not the right size
    if(convolver->impulse && convolver->impulse->n_samples != impulse_len*2)
    {
        destroy_buffer(convolver->impulse);
        destroy_buffer(convolver->in);
        destroy_buffer(convolver->out);
        destroy_complex_buffer(convolver->impulse_fft);
        destroy_complex_buffer(convolver->in_fft);
        destroy_buffer(convolver->overlap);
        destroy_fft(convolver->fft);
    }
    
    // copy the impulse into the structure
    convolver->impulse = create_buffer(impulse_len*2);    
    zero_buffer(convolver->impulse);
    mix_buffer(convolver->impulse, impulse, 1.0);
    
    
    
    // create input and output buffers
    convolver->in = create_buffer(impulse_len*2);
    convolver->overlap = create_buffer(impulse_len);
    convolver->out = create_buffer(impulse_len*2);        
    convolver->impulse_fft = create_complex_buffer(impulse_len+1);
    convolver->in_fft = create_complex_buffer(impulse_len+1);
    
    // create the fft object
    convolver->fft = create_fft(impulse_len*2);    
    
    // fft the impulse for later use
    fft_buffer(convolver->fft, convolver->impulse, convolver->impulse_fft);
    
    convolver->buffer_ptr = 0;
    convolver->len = impulse_len;
}


// set the impulse from a mono file
void load_impulse_from_file_convolver(Convolver *convolver, char *fname)
{
        WaveSound *sound;
        sound = create_wave_sound(fname);
        set_impulse_convolver(convolver, get_channel_wave_sound(sound, 0));
        destroy_wave_sound(sound);       
}

// destroy a convolver object
void destroy_convolver(Convolver *convolver)
{
   if(convolver->impulse)
    {
        destroy_buffer(convolver->impulse);
        destroy_buffer(convolver->in);
        destroy_buffer(convolver->out);
        destroy_complex_buffer(convolver->impulse_fft);
        destroy_complex_buffer(convolver->in_fft);
        destroy_buffer(convolver->overlap);
        destroy_fft(convolver->fft);
    }
    free(convolver); 
}


// convolve a buffer of arbitrary size, and copy the result into an output buffer of the same size
void process_convolver(Convolver *convolver, Buffer *input, Buffer *output)
{
    int i;
    // can't convolve without an impulse
    if(!convolver->impulse)
        return;
                   
    // do the convolution    
    for(i=0;i<input->n_samples;i++)
    {   
        // accumulate input
        convolver->in->x[convolver->buffer_ptr] = input->x[i];
        
        
        // output previous buffer + previous overlap buffer
        output->x[i] = (convolver->out->x[convolver->buffer_ptr] + convolver->overlap->x[convolver->buffer_ptr]);
        
        
        convolver->buffer_ptr++;       
        
        // if one buffer filled, do a convolution
        if(convolver->buffer_ptr == convolver->len)
        {
            // copy _LAST_ buffer into the overlap region for the next buffer
            copy_buffer_partial(convolver->overlap, 0, convolver->len, convolver->out, convolver->len, convolver->len);                        
            // do the convolution and reset the buffer pointer
            fft_convolution(convolver);            
            convolver->buffer_ptr = 0;            
        }
    }
            

}


// convolve two power-of-2 length buffers by FFT'ing and then
// multiplying the results
void fft_convolution(Convolver *convolver)
{    
    fft_buffer(convolver->fft, convolver->in, convolver->in_fft);    
    multiply_complex_buffer(convolver->in_fft, convolver->impulse_fft);
    scale_complex_buffer(convolver->in_fft, 1.0/(convolver->len*2));
    ifft_buffer(convolver->fft, convolver->in_fft, convolver->out);
}





// create a convolver for multiple channels
MultichannelConvolver *create_multichannel_convolver(int channels)
{
    int i;
    MultichannelConvolver *convolver = malloc(sizeof(*convolver));
    convolver->convolvers = malloc(sizeof(*convolver->convolvers) * channels);
    convolver->channels = channels;
    for(i=0;i<channels;i++)    
        convolver->convolvers[i] = create_convolver();
    return convolver;
    
}

// destroy a mulitchannel convolver
void destroy_multichannel_convolver(MultichannelConvolver *convolver)
{
    int i;
    for(i=0;i<convolver->channels;i++)    
        free(convolver->convolvers[i]);
    
    free(convolver->convolvers);
    free(convolver);
}


// set the impulse for all channels
void set_common_impulse_multichannel_convolver(MultichannelConvolver *convolver, Buffer *impulse)
{
    int i;
    for(i=0;i<convolver->channels;i++)    
        set_impulse_convolver(convolver->convolvers[i], impulse);    
}

// set a different impulse per channel
void set_independent_impulses_multichannel_convolver(MultichannelConvolver *convolver, Buffer **impulses)
{
    int i;
    for(i=0;i<convolver->channels;i++)    
        set_impulse_convolver(convolver->convolvers[i], impulses[i]);    
}


// pass multiple channels through a convolver
void process_multichannel_convolver(MultichannelConvolver *convolver, Buffer **ins, Buffer **outs)
{
    int i;
    for(i=0;i<convolver->channels;i++)    
        process_convolver(convolver->convolvers[i], ins[i], outs[i]);
    
}
