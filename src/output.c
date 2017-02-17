/**    
    @file output.c
    @brief
    @author John Williamson
    
    Copyright (c) 2011 All rights reserved.
    Licensed under the BSD 3 clause license. See COPYING.
    
    This file is part of the OpenGrain distribution.
    http://opengrain.sourceforge.net   
*/              

#include "output.h"
#include "trigger_tests.h"
#include "grain_tests.h"

#ifdef USE_PORTAUDIO
#include <portaudio.h>
#endif


// send data to the live triggers
void fire_triggers_output_info(OutputInfo *info, float *in, int n_frames)
{
    Trigger *trigger;
    PaUtilRingBuffer *ringbuffer;
    int i;            
    
    // deinterleave stereo/multichannel input data
    for(i=0;i<n_frames;i++)
            info->input->x[i] = in[i*GLOBAL_STATE.n_input_channels + GLOBAL_STATE.input_channel];
    
    // copy this buffer to all connected triggers
    list_iterator_start(info->live_triggers);
    while(list_iterator_hasnext(info->live_triggers))
    {        
        trigger = list_iterator_next(info->live_triggers);   
        
        ringbuffer = get_ringbuffer_trigger(trigger);    
        // if not enough room for this buffer, try again next time
        if(PaUtil_GetRingBufferWriteAvailable(ringbuffer) >= n_frames)               
            // write this block into all of the triggers' ringbuffers
            PaUtil_WriteRingBuffer(ringbuffer, info->input->x, n_frames * sizeof(*info->input->x));                    
            
        process_trigger(trigger);
    }    
    list_iterator_stop(info->live_triggers);    
}


// process a buffer of data
// note that data is stereo interleaved
void audio_callback_output_info(void *data, float *in, float *out)
{
    OutputInfo *info = *((OutputInfo **)data);
    Buffer *output_buffers[2];
    int i;
    int n;
    n = GLOBAL_STATE.frames_per_buffer;
    if(in)
        fire_triggers_output_info(info, in, n);
    
    // do the synthesis!
    grain_mix(info->mixer, info->left, info->right);    
    
    if(GLOBAL_STATE.n_channels==2)
    {
        // write to stereo audio buffer
        if(info->output_mode & OUTPUT_REALTIME_AUDIO)
        {
            for( i=0; i<n; i++ )
            {
                *out++ = info->left->x[i];
                *out++ = info->right->x[i];
            }
        }
    }   
    else
    {
        // write to mono audio buffer
        if(info->output_mode & OUTPUT_REALTIME_AUDIO)
        {
            for( i=0; i<n; i++ )            
                *out++ = (info->left->x[i] + info->right->x[i])/2.0;                            
        }             
    }
            
    output_buffers[0] = info->left;
    output_buffers[1] = info->right;
    
    // write to file (output is always stereo)
    if(info->output_mode & OUTPUT_WAVEFILE_AUDIO)        
        write_wavewriter(info->writer, output_buffers);    
     
        
    // update time elapsed
    GLOBAL_STATE.elapsed_samples += n;
    GLOBAL_STATE.elapsed = GLOBAL_STATE.elapsed_samples / (double)GLOBAL_STATE.sample_rate;     
}

// called when the audio stream is closed
void audio_finished_output_info(void *data)
{
    OutputInfo *info = *((OutputInfo **)data);
    stop_wavewriter(info->writer);
}

OutputInfo *create_output_info(void)
{
    GrainStream *stream;    
    
  
    
    OutputInfo *info = malloc(sizeof(*info));
    info->left = create_buffer(GLOBAL_STATE.frames_per_buffer);    
    info->right = create_buffer(GLOBAL_STATE.frames_per_buffer);
    info->input = create_buffer(GLOBAL_STATE.frames_per_buffer);
    info->writer = create_wavewriter();
    set_multichannel_wavewriter(info->writer, 2, MULTICHANNEL_INTERLEAVE);
    info->mixer = create_mixer();
    info->live_triggers = malloc(sizeof(*info->live_triggers));
    list_init(info->live_triggers);        
    set_output_mode_output_info(info, OUTPUT_REALTIME_AUDIO | OUTPUT_WAVEFILE_AUDIO);
       
    
    
    // testing 
    stream = create_stream(2);

    
    add_stream(info->mixer, stream);    
    test_impulsegrain(stream);    
    test_default_grain_generation(stream);
    test_default_mixer_settings(info->mixer);
    
    return info;
}

    
// connect a live trigger to receive data from the microphone input
void connect_live_trigger(OutputInfo *info, Trigger *trigger)
{
    list_append(info->live_triggers, trigger);
  
}

// disconnect a live trigger
void disconnect_live_trigger(OutputInfo *info, Trigger *trigger)
{
    int index;
    index = list_locate(info->live_triggers, trigger);
    if(index>=0)
        list_delete_at(info->live_triggers, index);

}


// Destroy an output object, stopping any wavewriting first of all
void destroy_output_info(OutputInfo *info)
{
    stop_wavewriter(info->writer);
    destroy_wavewriter(info->writer);
    destroy_buffer(info->left);
    destroy_buffer(info->right);
    destroy_mixer(info->mixer);
    
    list_destroy(info->live_triggers);
    free(info->live_triggers);
    free(info);
}


// set the output mode
void set_output_mode_output_info(OutputInfo *info, int mode)
{
    // start writing if wavewriting just got enabled
    if((mode & OUTPUT_WAVEFILE_AUDIO) && !(info->output_mode & OUTPUT_WAVEFILE_AUDIO))    
        start_wavewriter(info->writer);    
        
    // stop writing, if wavewriting just got disabled
    if(!(mode & OUTPUT_WAVEFILE_AUDIO) && (info->output_mode & OUTPUT_WAVEFILE_AUDIO))    
        stop_wavewriter(info->writer);    
    
    
    info->output_mode = mode;    
}

/*
int main(void)
{
    OutputInfo *global_info=NULL;            
    AudioState prototype;
    
    int i;
    
    
    prototype.sample_rate = 44100;
    prototype.frames_per_buffer = 440;
    prototype.n_channels = 2;
    prototype.n_input_channels = 0;
    prototype.input_channel = 0;
    prototype.in_device = -1;
    prototype.out_device = -1;
    
    
     
    init_audio(&prototype, audio_callback_output_info, audio_finished_output_info, &global_info);    
    global_info = create_output_info();        
    start_audio();                   
    
    
    //test_hrtf();
    //test_matrix();  

    
#ifdef USE_PORTAUDIO
    
    i=0;
    
    
    while(i<60*GLOBAL_STATE.sample_rate / GLOBAL_STATE.frames_per_buffer)        
    {        
        if(buffers_remaining_audio()<8)
        {
            pump_audio(1);
            i++;
            
    
        }
    }
    while(pump_audio(0));
    
    
    
    //Pa_Sleep( 10 * 1000 );    
#else
    for(i=0;i<10*GLOBAL_STATE.sample_rate / GLOBAL_STATE.frames_per_buffer+1 ;i++)
        pump_audio();
#endif
    shutdown_audio();
    destroy_output_info(global_info);
    return 0;
}
*/
