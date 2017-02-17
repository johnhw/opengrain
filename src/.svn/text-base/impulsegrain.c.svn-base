/**    
    @file impulsegrain.c
    @brief
    @author John Williamson
    
    Copyright (c) 2011 All rights reserved.
    Licensed under the BSD 3 clause license. See COPYING.
    
    This file is part of the OpenGrain distribution.
    http://opengrain.sourceforge.net   
*/              

#include "impulsegrain.h"
#include <string.h>

// create an empty impulse grain parameter set
ImpulseGrainParameters *create_impulse_parameters(GrainSource *source)
{

    ImpulseGrainParameters *impulsegrain;
    impulsegrain = malloc(sizeof(*impulsegrain));
    impulsegrain->sounds = malloc(sizeof(*impulsegrain->sounds));
    impulsegrain->loop = 0;
    list_init(impulsegrain->sounds);
    return impulsegrain;
}


// add a single waveform to an impulse parameter set
void add_sound_impulse_parameters(ImpulseGrainParameters *impulse_parameters, WaveSound *sound)
{
    list_append(impulse_parameters->sounds, sound);
}


void set_loop_impulse_parameters(ImpulseGrainParameters *impulse_parameters, int loop)
{
    impulse_parameters->loop = loop;
}


// append a whole list of sounds to an impulse parameter set
void add_sound_bank_impulse_parameters(ImpulseGrainParameters *impulse_parameters, list_t *sound_list)
{
    WaveSound *sound;
    
    list_iterator_start(sound_list);
    while(list_iterator_hasnext(sound_list))
    {
        sound = (WaveSound*)list_iterator_next(sound_list);    
        list_append(impulse_parameters->sounds, sound);
    }
    list_iterator_stop(sound_list);
    
}


// destroy an impulse parameter set
void destroy_impulse_parameters(ImpulseGrainParameters *impulse_parameters)
{

    list_destroy(impulse_parameters->sounds);
    free(impulse_parameters->sounds);
    free(impulse_parameters);

}


// create an empty impulse grain
void *create_impulsegrain(void *source)
{
    ImpulseGrainParameters *impulse_parameters;
    ImpulseGrain *impulse;
    impulse_parameters = (ImpulseGrainParameters*)source;    
    impulse = malloc(sizeof(*impulse));
    
    return impulse;
}


// create a new impulse grain which will play one of the sounds
// of it's source randomly
void init_impulsegrain(void *impulsegrain, void *source, Grain *grain)
{

    ImpulseGrainParameters *impulse_parameters;
    ImpulseGrain *impulse;
    int index;
    impulse_parameters = (ImpulseGrainParameters*)source;    
    impulse = (ImpulseGrain*)impulsegrain;
    
    // choose a random source
    index = random_int(0, list_size(impulse_parameters->sounds));
    impulse->sound = list_get_at(impulse_parameters->sounds, index);
    impulse->phase = 0;
    impulse->master_grain = grain;
    impulse->loop = impulse_parameters->loop;
}


// destroy an impulse grain (don't free the sounds!)
void destroy_impulsegrain(void *impulsegrain)
{
    free((ImpulseGrain*)impulsegrain);
}


// copy a block from an impulse into a buffer
void fill_impulsegrain(void *impulsegrain, Buffer *buffer)
{
    int i;
    ImpulseGrain *impulse;
    Buffer *channel;
    impulse = (ImpulseGrain *) impulsegrain;
    channel = get_channel_wave_sound(impulse->sound, 0);
    
    for(i=0;i<buffer->n_samples;i++)
    {
    
        if(impulse->phase < channel->n_samples)
            buffer->x[i] = channel->x[impulse->phase++];
    
        // if finished, force the grain to finish (even if it's early)
        if(impulse->phase >= channel->n_samples)
        {
            if(impulse->loop)
                impulse->phase = 0;
            else    
                finish_grain(impulse->master_grain);
        }
    }
           
    
    
}
