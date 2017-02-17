/**    
    @file grain_stream.c
    @brief
    @author John Williamson
    
    Copyright (c) 2011 All rights reserved.
    Licensed under the BSD 3 clause license. See COPYING.
    
    This file is part of the OpenGrain distribution.
    http://opengrain.sourceforge.net   
*/              

#include "grain_stream.h"


// Create a new stream object
GrainStream *create_stream(int channels)
{
    GrainStream *stream;
    
    stream = malloc(sizeof(*stream));
    
    stream->source_list = malloc(sizeof(*stream->source_list));
    list_init(stream->source_list);
    
    // grain lists
    
    stream->grain_list = malloc(sizeof(*stream->grain_list));
    list_init(stream->grain_list);
    stream->dead_grain_list = malloc(sizeof(*stream->dead_grain_list));
    list_init(stream->dead_grain_list);
    
    stream -> time_until_next_grain = 0;
    stream->model = create_grain_model();
    
    
    stream->gain = 1.0;
    stream->target_gain = 1.0;
    stream->gain_coeff = 1.0;
    
    stream->channels = channels;
    
    // effects 
    stream->fx = create_stream_fx(stream->channels+1);
    stream->spatializer = create_spatializer();
    
        
    // temporary buffer for summing grains into
    stream->temp_grain = create_buffer(GLOBAL_STATE.frames_per_buffer);
           
    return stream;
}

// get the effects unit for this stream
StreamFX *get_stream_fx_stream(GrainStream *stream)
{
    return stream->fx;
}


// set the overall gain of the stream in decibels, and the time to fade to that level
// fade is only computed every buffer
void fade_gain_stream(GrainStream *stream, float gaindB, float time)
{
    stream->target_gain = dB_to_gain(gaindB);
    stream->gain_coeff = rc_time(time / GLOBAL_STATE.frames_per_buffer, -120);    
}


// set the current gain in dB
void set_gain_stream(GrainStream *stream, float gaindB)
{
    stream->target_gain = dB_to_gain(gaindB);
    stream->gain = dB_to_gain(gaindB);
    
}

// delete a stream and all of its attached sources
void destroy_stream(GrainStream *stream)
{
    Grain *grain;
    GrainSource *source;
    
    destroy_grain_model(stream->model);
    
    // delete each individial grain
    list_iterator_start(stream->grain_list);    
    while(list_iterator_hasnext(stream->grain_list))
    {
        grain = (Grain *) list_iterator_next(stream->grain_list);
        destroy_grain(grain);    
    }
    list_iterator_stop(stream->grain_list);
    
    // destroy the list
    list_destroy(stream->grain_list);
    free(stream->grain_list);
    
    
    
    // delete each individial grain
    list_iterator_start(stream->dead_grain_list);    
    while(list_iterator_hasnext(stream->dead_grain_list))
    {
        grain = (Grain *) list_iterator_next(stream->dead_grain_list);
        destroy_grain(grain);    
    }
    list_iterator_stop(stream->dead_grain_list);
    
    // destroy the list
    list_destroy(stream->dead_grain_list);
    free(stream->dead_grain_list);
    
    
    // free sources    
    list_iterator_start(stream->source_list);    
    while(list_iterator_hasnext(stream->source_list))
    {
        source = (GrainSource *) list_iterator_next(stream->source_list);
        destroy_source(source);        
    }
    list_iterator_stop(stream->source_list);
    
    // destroy the list
    list_destroy(stream->source_list);
    free(stream->source_list);
    
    destroy_stream_fx(stream->fx);
    destroy_spatializer(stream->spatializer);
    
    
    // free buffers
    destroy_buffer(stream->temp_grain);
        
    free(stream);
}

// add a source to this stream
void add_source_stream(GrainStream *stream, GrainSource *source)
{
    list_append(stream->source_list, source);
}

// get the GrainModel for this stream
GrainModel *get_grain_model_stream(GrainStream *stream)
{
    return stream->model;
}

// if there are grains in the dead list, draw them back out
// otherwise generate a new one and return
Grain *revive_grain_stream(GrainStream *stream)
{
    Grain *grain;
    if(list_size(stream->dead_grain_list)>0)    
       grain = list_extract_at(stream->dead_grain_list, 0);    
    else    
        grain = create_grain();                
            
    return grain;
}


// put a grain back into the dead list, and remove it from the active list
void kill_grain_stream(GrainStream *stream, Grain *grain)
{
    // old grains never die, they just... go into the dead pool    
    int index;
    
    // remove from list
    index = list_locate(stream->grain_list, grain);
    grain = list_extract_at(stream->grain_list, index);       
    kill_specifics_source(grain->source, grain->specifics);
    
    // put into the dead list
    list_append(stream->dead_grain_list, grain);
    
}



// select a source, and add a grain from that source to the active list
void add_grain_stream(GrainStream *stream, int when)
{        
        Grain *grain;
        int distance_delay;
        
        grain = revive_grain_stream(stream);
        fill_from_grain_model(stream->model, stream->source_list, when, grain);                
        
        // apply sample delay to grains, according to distance        
        distance_delay = get_sample_delay_spatializer(stream->spatializer, grain->location->distance);        
        grain->samples_passed -= distance_delay;
        
        // put the grain in the list
        list_append(stream->grain_list, grain);                        
}



// trigger grains according to the rate parameter (if not using manual triggering)
// needs a buffer to know how much data to fill in 
void auto_trigger_grains_stream(GrainStream *stream, Buffer *buffer)
{
    double interval, done;
    double t;
        
    // make sure we wait until the next period
    done = stream->time_until_next_grain;
    if(done >= buffer->n_samples)    
    {
        // no grain this buffer, try again later...
        done -= buffer->n_samples;                
        
    }
        
    
    while(done < buffer->n_samples)
    {
        t = next_time_grain_model(stream->model);            
        interval = t * GLOBAL_STATE.sample_rate;            
        
        
        // this grain model is not in firing mode if it returns -1, NaN or inf
        if(interval==-1 || isinf(interval) || isnan(interval))
            return;
            
            
        // can't have negative times!
        if(interval<0)
            interval = 0;         
                
        if(done<buffer->n_samples)
            add_grain_stream(stream, done);
        done += interval;
    }                
    
    stream->time_until_next_grain = done;
}



// add in this streams contributions to the audio output
// outs should have stream->channels+1 entries (for the channels + diffuse reverb channel)
void sum_buffer_stream(GrainStream *stream, Buffer **outs)
{
    
    int i;
    
    // fade the overall gain
    stream->gain = stream->gain_coeff * stream->gain + (1-stream->gain_coeff) * stream->target_gain;
    
    zero_buffer(stream->temp_grain);
    auto_trigger_grains_stream(stream, outs[0]);              
    start_spatializer(stream->spatializer);        
    synthesize_stream(stream);        
    stop_spatializer(stream->spatializer);       
        
    for(i=0;i<get_n_channels_spatializer(stream->spatializer);i++)   
            mix_buffer(outs[i], get_channel_spatializer(stream->spatializer, i), stream->gain);
                           
    mix_buffer(outs[stream->channels], stream->spatializer->reverb, stream->gain);
    compute_stream_fx(stream->fx, outs, outs);                      
    
}


// Take all active grains, and sum them into a stereo buffer
void synthesize_stream(GrainStream *stream)
{
    Grain *grain;
    
    list_t kill_list;
    list_init(&kill_list);    
    int offset, len;
    
    Buffer fake_buffer;
        
    
    //for each grain
        
    list_iterator_start(stream->grain_list);
    
    while(list_iterator_hasnext(stream->grain_list))
    {
        grain = (Grain *) list_iterator_next(stream->grain_list);        
        // do the actual synthesis
   
        
        // only play grains which will actually sound in this buffer
        if(grain->samples_passed > -stream->temp_grain->n_samples)
        {
                    
            // offset is zero if grain is already started, or the end of the buffer minus the start time
            if(grain->samples_passed>=0)
                offset = 0;
            else
                offset = -grain->samples_passed;
                
                
            // the length is minimum of the offset to the end, or the remainder of the grains duration
            len = MIN(stream->temp_grain->n_samples - offset, grain->duration_samples - grain->samples_passed);
                   
            // make a "fake" buffer which really points directly into the correct portion of the grain buffer
            fake_buffer.x = &(stream->temp_grain->x[offset]);
            fake_buffer.n_samples = len;           
    
            // synthesise
            grain->source->fill_grain(grain->specifics, &fake_buffer);            
            
                        
            // apply envelope
            envelope_buffer(grain->envelope, &fake_buffer);        
                           
            // spatializer
            spatialize(stream->spatializer, grain->location, grain->amplitude, &fake_buffer, offset, len);

            // move on grain pointer
            grain->samples_passed += stream->temp_grain->n_samples;               
            
            // check if finished and remove if so!
            if(grain->samples_passed >= grain->duration_samples || grain->finished)
                list_append(&kill_list, grain);                                  
            
      }
      else
      {
            // keep increasing samples_passed until the grain enters a buffer
             grain->samples_passed += stream->temp_grain->n_samples;                      
      }
    }
    
    list_iterator_stop(stream->grain_list);
    
    // remove all expired grains
    list_iterator_start(&kill_list);    
    while(list_iterator_hasnext(&kill_list))
    {
        grain = (Grain *) list_iterator_next(&kill_list);
        kill_grain_stream(stream, grain);               
    }    
    list_iterator_stop(&kill_list);
    list_destroy(&kill_list);
}



// return the spatializer object for this stream
Spatializer *get_spatializer_stream(GrainStream *stream)
{
    return stream->spatializer;
}


// manually trigger a single grain
void trigger_single_grain_stream(GrainStream *stream, double in_time)
{
    add_grain_stream(stream, in_time * GLOBAL_STATE.sample_rate);
}

// manually trigger a number of grains over the next time period
void trigger_grains_stream(GrainStream *stream, int n_grains, double in_time, double after, int trigger_mode)
{
    int i;
    int div;
    
    // evenly spaced grains
    if(trigger_mode == TRIGGER_MODE_REGULAR)    
    {    
        div = (in_time / (double)n_grains + after) * GLOBAL_STATE.sample_rate;
        for(i=0;i<n_grains;i++)       
            add_grain_stream(stream, i*div);        
    }
    
    // randomly spaced grains
    if(trigger_mode == TRIGGER_MODE_STOCHASTIC)
    {
        for(i=0;i<n_grains;i++)        
            add_grain_stream(stream, (after + uniform_double() * in_time) * GLOBAL_STATE.sample_rate);        
    }

}


