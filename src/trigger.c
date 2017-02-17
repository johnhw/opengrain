/**    
    @file trigger.c
    @brief
    @author John Williamson
    
    Copyright (c) 2011 All rights reserved.
    Licensed under the BSD 3 clause license. See COPYING.
    
    This file is part of the OpenGrain distribution.
    http://opengrain.sourceforge.net   
*/              

#include "trigger.h"

#define RINGBUFFER_SIZE 65536

// create an empty trigger object, with a ringbuffer ready to go
Trigger *create_trigger(void)
{   
    Trigger *trigger;
    trigger = malloc(sizeof(*trigger));
    trigger->loop = 0;
    trigger->wave_input_phase = 0;
    trigger->mode = TRIGGER_FROM_RINGBUFFER;
    trigger->sound = NULL;
    trigger->trigger_data = NULL;
    trigger->process_callback = NULL;
    trigger->grain_stream = NULL;    
    trigger->ringbuffer = malloc(sizeof(*trigger->ringbuffer));
    trigger->ringdata = malloc(RINGBUFFER_SIZE);    
    trigger->input = create_buffer(GLOBAL_STATE.frames_per_buffer);
    PaUtil_InitializeRingBuffer(trigger->ringbuffer, RINGBUFFER_SIZE, trigger->ringdata);            
    return trigger;
}

// destroy a trigger object
void destroy_trigger(Trigger *trigger)
{    
    free(trigger->ringdata);
    free(trigger->ringbuffer);    
    free(trigger);
}


// set the stream that this trigger will generate grains on
void set_grain_stream_trigger(Trigger *trigger, GrainStream  *stream)
{
    trigger->grain_stream = stream;
}

// set the function which processes incoming audio buffers and the data for the object that processes them
void set_processor_trigger(Trigger *trigger, TriggerProcessCallback callback, void *trigger_data)
{
    trigger->process_callback = callback;
    trigger->trigger_data = trigger_data;
}


// set whether to use the wavefile or whether to use realtime data
void set_mode_trigger(Trigger *trigger, int mode, int loop)
{
    trigger->loop = loop;
    trigger->mode = mode;
}


// set the waveform to use for input if the mode is from wave files
void set_wave_trigger(Trigger *trigger, WaveSound *sound)
{
    trigger->sound = sound;
    trigger->wave_input_phase = 0;
}


// return the ring buffer for input to this trigger to write into
PaUtilRingBuffer *get_ringbuffer_trigger(Trigger *trigger)
{
        return trigger->ringbuffer;
}


// Run the trigger over the input, either from the input ringbuffer, or
// from the currently active wave sound
void process_trigger(Trigger *trigger)
{
    int wave_end, len, i, data_bytes;
    Buffer *channel;
    
    // can't trigger without a callback and a grain stream
    if(!trigger->process_callback || !trigger->grain_stream)
        return;
        
    // can't trigger without a sound, if using wave files
    if(trigger->mode==TRIGGER_FROM_WAVE && !trigger->sound)
        return;
            
    
    // ringbuffer mode
    if(trigger->mode==TRIGGER_FROM_RINGBUFFER)
    {
        data_bytes =trigger->input->n_samples * sizeof(*trigger->input->x);
        // is there enough data waiting?
        if(PaUtil_GetRingBufferReadAvailable(trigger->ringbuffer) >= data_bytes)        
            PaUtil_ReadRingBuffer(trigger->ringbuffer, &(trigger->input->x[0]), data_bytes);                
        else        
           // not enough data, try again next buffer
            return;        
    }
    
    // wavefile mode
    if(trigger->mode==TRIGGER_FROM_WAVE)
    {
        channel = get_channel_wave_sound(trigger->sound, 0);
        wave_end = trigger->wave_input_phase + trigger->input->n_samples;
        
        // need to split if the wave block runs over the end of the wave file
        if(wave_end >= channel->n_samples)
        {
            len = channel->n_samples - trigger->wave_input_phase;
            copy_buffer_partial(trigger->input, 0,  len, channel, trigger->wave_input_phase, len);
            if(trigger->loop)
            {                
                trigger->wave_input_phase += trigger->input->n_samples;
                trigger->wave_input_phase -= channel->n_samples;
                // copy in wrapped block from the start of the audio
                copy_buffer_partial(trigger->input, len,  trigger->input->n_samples-len, channel, trigger->wave_input_phase, trigger->input->n_samples-len);                
                
            }
            else
            {
                trigger->sound = NULL;                
                // clear remainder of buffer
                for(i=len;i<trigger->input->n_samples;i++)                
                    trigger->input->x[i] = 0.0;
                
            }                    
        }
        // no wrapping
        else 
        {
           len = trigger->input->n_samples;
           copy_buffer_partial(trigger->input, 0,  len, channel, trigger->wave_input_phase, len);
           trigger->wave_input_phase += trigger->input->n_samples;           
        }            
    }
    
    // process this block
    trigger->process_callback(trigger->trigger_data, trigger->input, trigger->grain_stream);    

}
