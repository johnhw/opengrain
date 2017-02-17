/**    
    @file loopgrain.c
    @brief
    @author John Williamson
    
    Copyright (c) 2011 All rights reserved.
    Licensed under the BSD 3 clause license. See COPYING.
    
    This file is part of the OpenGrain distribution.
    http://opengrain.sourceforge.net   
*/              

#include "loopgrain.h"
#include <string.h>

// create an empty loop grain parameter set
LoopGrainParameters *create_loop_parameters(GrainSource *source)
{

    LoopGrainParameters *loopgrain;
    loopgrain = malloc(sizeof(*loopgrain));
    loopgrain->attacks = malloc(sizeof(*loopgrain->attacks));
    loopgrain->sustains = malloc(sizeof(*loopgrain->sustains));
    loopgrain->attack_crossfade_time = 0.01;
    loopgrain->loop_crossfade_time = 0.01;       
    list_init(loopgrain->attacks);
    list_init(loopgrain->sustains);
    
    loopgrain->pitch =create_distribution();
    return loopgrain;
}


// add a single waveform to an loop parameter set
void add_attack_loop_parameters(LoopGrainParameters *loop_parameters, WaveSound *sound)
{
    list_append(loop_parameters->attacks, sound);
}

// append a whole list of sounds to an loop parameter set
void add_attack_bank_loop_parameters(LoopGrainParameters *loop_parameters, list_t *sound_list)
{
    WaveSound *sound;
    
    list_iterator_start(sound_list);
    while(list_iterator_hasnext(sound_list))
    {
        sound = (WaveSound*)list_iterator_next(sound_list);    
        list_append(loop_parameters->attacks, sound);
    }
    list_iterator_stop(sound_list);
    
}


// add a single waveform to an loop parameter set
void add_sustain_loop_parameters(LoopGrainParameters *loop_parameters, WaveSound *sound)
{
    list_append(loop_parameters->attacks, sound);
}

// append a whole list of sounds to an loop parameter set
void add_sustain_bank_loop_parameters(LoopGrainParameters *loop_parameters, list_t *sound_list)
{
    WaveSound *sound;
    
    list_iterator_start(sound_list);
    while(list_iterator_hasnext(sound_list))
    {
        sound = (WaveSound*)list_iterator_next(sound_list);    
        list_append(loop_parameters->attacks, sound);
    }
    list_iterator_stop(sound_list);
    
}

// set the crossfade time for blending the attack into the sustain, and the loop into itself
void set_crossfade_loop_parameters(LoopGrainParameters *loop_parameters, float attack_time, float loop_time)
{
    loop_parameters->attack_crossfade_time = attack_time;
    loop_parameters->loop_crossfade_time = loop_time;    
}

// get the pitch distribution, in semitones
Distribution *get_pitch_distribution(LoopGrainParameters *loop_parameters)
{
    return loop_parameters->pitch;
}

// destroy an loop parameter set
void destroy_loop_parameters(LoopGrainParameters *loop_parameters)
{

    destroy_distribution(loop_paramters->pitch);
    list_destroy(loop_parameters->sustains);
    list_destroy(loop_parameters->attacks);
    free(loop_parameters->sounds);
    free(loop_parameters);

}


// create an empty loop grain
void *create_loopgrain(void *source)
{
    LoopGrainParameters *loop_parameters;
    LoopGrain *loop;
    loop_parameters = (LoopGrainParameters*)source;    
    loop = malloc(sizeof(*loop));
    
    return loop;
}


// create a new loop grain which will play one of the sounds
// of it's source randomly
void init_loopgrain(void *loopgrain, void *source, Grain *grain)
{

    LoopGrainParameters *loop_parameters;
    LoopGrain *loop;
    int index;
    loop_parameters = (LoopGrainParameters*)source;    
    loop = (LoopGrain*)loopgrain;
    
    // choose a random source (can be NULL)
    index = random_int(0, list_size(loop_parameters->attacks));
    loop->attack = list_get_at(loop_parameters->sounds, index);
    
    index = random_int(0, list_size(loop_parameters->sustains));
    loop->sustain = list_get_at(loop_parameters->sustains, index);
    
    loop->phase_increment = SEMITONES_TO_RATE(sample_from_distribution(loop_parameters->pitch));
    
    loop->phase = 0;
    loop->master_grain = grain;
    loop->attack_finished = 0;
}


// destroy an loop grain (don't free the sounds!)
void destroy_loopgrain(void *loopgrain)
{
    
    free((LoopGrain*)loopgrain);
}


// copy a block from an loop into a buffer
void fill_loopgrain(void *loopgrain, Buffer *buffer)
{
    int i;
    float t;
    int index;
    LoopGrain *loop;
    Buffer *channel;
    loop = (LoopGrain *) loopgrain;
    
    if(loop->attack && !loop->attack_finished)
        channel = get_channel_wave_sound(loop->attack, 0);
    
    for(i=0;i<buffer->n_samples;i++)
    {
    
        index = floor(loop->phase);
        t = loop->phase-index;
        
        // linear interpolation
        if(loop->phase < channel->n_samples-1)
            buffer->x[i] = (1-t*)*channel->x[index]+t*channel->x[index+1];
    
        
        if(loop->phase >= channel->n_samples)
        {
            // finish attack portion
            if(!loop->attack_finished)
            {
                loop->attack_finished = 1;
                channel = get_channel_wave_sound(loop->sustain, 0);
            }
            loop->phase = 0;
        }
        
        // pitch shift sustain only
        if(loop->attack_finished)
            loop->phase += loop->phase_increment;
        else
            loop->phase ++;
    }
           
    
    
}
