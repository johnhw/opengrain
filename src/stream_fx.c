/**    
    @file stream_fx.c
    @brief
    @author John Williamson
    
    Copyright (c) 2011 All rights reserved.
    Licensed under the BSD 3 clause license. See COPYING.
    
    This file is part of the OpenGrain distribution.
    http://opengrain.sourceforge.net   
*/              

#include "stream_fx.h"


StreamFX *create_stream_fx(int channels)
{
    StreamFX *fx;
    fx = malloc(sizeof(*fx));
    
    // effects 
    fx->filters = create_multichannel_SVF(channels);
    fx->filter_enabled = 0;
    
    fx->delays = create_cross_delay(channels);
    fx->delay_enabled = 0;
        
    fx->resonators = create_multichannel_resonator_bank(channels);    
    fx->resonators_enabled = 0;    
    
    fx->convolvers = create_multichannel_convolver(channels);
    fx->convolver_enabled = 0;
    
    fx->choruses = create_multichannel_chorus(channels);    
    fx->chorus_enabled = 0;
    
    fx->channels = channels;
   
    
        
    return fx;
}



void destroy_stream_fx(StreamFX *fx)
{
    // free effects       
    destroy_multichannel_resonator_bank(fx->resonators);
    destroy_multichannel_SVF(fx->filters);
    destroy_multichannel_chorus(fx->choruses);
    destroy_multichannel_convolver(fx->convolvers);
    destroy_cross_delay(fx->delays);
}


// return the delay object
CrossDelay *get_delay_stream_fx(StreamFX *fx)
{
    return fx->delays;
}

// return the filter object
MultichannelSVF *get_filter_stream_fx(StreamFX *fx)
{
    return fx->filters;

}

// return the resonator bank object
MultichannelResonatorBank *get_resonators_stream_fx(StreamFX *fx)
{
    return fx->resonators;

}

// return the resonator bank object
MultichannelChorus *get_chorus_stream_fx(StreamFX *fx)
{
    return fx->choruses;      
}

// return the resonator bank object
MultichannelConvolver *get_convolver_stream_fx(StreamFX *fx)
{
    return fx->convolvers;      
}

// apply the effects to a pair of buffers
void compute_stream_fx(StreamFX *fx, Buffer **ins, Buffer **outs)
{
    
    
    if(fx->convolver_enabled)
        process_multichannel_convolver(fx->convolvers, ins, outs);
        
    // cross delay
    if(fx->delay_enabled)               
        process_cross_delay(fx->delays, ins, outs);            
        
    if(fx->chorus_enabled)               
        process_multichannel_chorus(fx->choruses, ins, outs);            
        
    if(fx->resonators_enabled)               
        process_multichannel_resonator_bank(fx->resonators, ins, outs);            
    
    if(fx->filter_enabled)               
        process_multichannel_SVF(fx->filters, ins, outs);            
        

}



// enable the filter
void enable_filter_stream_fx(StreamFX *fx)
{
    fx->filter_enabled = 1;

}

// disable the filter
void disable_filter_stream_fx(StreamFX *fx)
{
    fx->filter_enabled = 0;
}


// enable the delay
void enable_delay_stream_fx(StreamFX *fx)
{
    fx->delay_enabled = 1;
}

// disable the delay
void disable_delay_stream_fx(StreamFX *fx)
{
    fx->delay_enabled = 0;
}

// enable the chorus
void enable_chorus_stream_fx(StreamFX *fx)
{
    fx->chorus_enabled = 1;
}

// disable the chorus
void disable_chorus_stream_fx(StreamFX *fx)
{
    fx->chorus_enabled = 0;
}

// enable the convolver
void enable_convolver_stream_fx(StreamFX *fx)
{
    fx->convolver_enabled = 1;
}

// disable the convolver
void disable_convolver_stream_fx(StreamFX *fx)
{
    fx->convolver_enabled = 0;
}

// enable the resonators
void enable_resonators_stream_fx(StreamFX *fx)
{
    fx->resonators_enabled = 1;
}

// disable the chorus
void disable_resonators_stream_fx(StreamFX *fx)
{
    fx->resonators_enabled = 0;
}



