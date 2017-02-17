/**    
    @file sys_portaudio.c
    @brief
    @author John Williamson
    
    Copyright (c) 2011 All rights reserved.
    Licensed under the BSD 3 clause license. See COPYING.
    
    This file is part of the OpenGrain distribution.
    http://opengrain.sourceforge.net   
*/              


#include "sys_portaudio.h"


// Name of this device
const char *driver_name_sys_audio = "PortAudio";




/** Create a PortaudioInfo object, with ringbuffers to hold data being transferred between streams */
PortaudioInfo *create_portaudio_info(AudioInfo *info)
{
    PortaudioInfo *paInfo;
    // create info object
    paInfo = malloc(sizeof(*paInfo));
  
        // size of one input buffer
    paInfo->in_block_size = sizeof(*paInfo->in_buffer) * info->state->n_input_channels * info->state->frames_per_buffer;
    paInfo->out_block_size = sizeof(*paInfo->out_buffer) * info->state->n_channels * info->state->frames_per_buffer;

    paInfo->in_buffer = malloc(paInfo->in_block_size);
    paInfo->out_buffer = malloc(paInfo->out_block_size);
    
    // create a ringbuffer to feed the buffers into/out of
    paInfo->in_ringbuffer = malloc(sizeof(*paInfo->in_ringbuffer));    
    paInfo->in_ringbuffer_storage = malloc(PORTAUDIO_RINGBUFFER_SIZE);
    PaUtil_InitializeRingBuffer(paInfo->in_ringbuffer, PORTAUDIO_RINGBUFFER_SIZE, paInfo->in_ringbuffer_storage);
    
    paInfo->out_ringbuffer = malloc(sizeof(*paInfo->out_ringbuffer));    
    paInfo->out_ringbuffer_storage = malloc(PORTAUDIO_RINGBUFFER_SIZE);
    PaUtil_InitializeRingBuffer(paInfo->out_ringbuffer, PORTAUDIO_RINGBUFFER_SIZE, paInfo->out_ringbuffer_storage);
    
    // set whether or not to use the ringbuffer
    paInfo->ringbuffer_mode = 1;
    paInfo->audio_info = info;
    return paInfo;
}



/** Free a PortaudioInfo structure, including all the audio buffers. */    
void destroy_portaudio_info(PortaudioInfo *info)
{
    free(info->in_ringbuffer_storage);
    free(info->out_ringbuffer_storage);
    free(info->in_buffer);
    free(info->out_buffer);
    free(info->in_ringbuffer);
    free(info->out_ringbuffer);
    free(info->audio_info);
    free(info); 

}



// synthesize one buffer and get it ready for output, or just pump existing buffers out of the queue
int pump_sys_audio(void *ptr, int synthesize)
{
    PortaudioInfo *info = (PortaudioInfo *)ptr;
    AudioInfo *audio = (AudioInfo *)info->audio_info;
    int flags;
    
    flags = 0;
    
    if(synthesize)
    {
        // if data to read
        if(PaUtil_GetRingBufferReadAvailable(info->in_ringbuffer) > info->in_block_size)
        {
            // read the data from the ringbuffer
            PaUtil_ReadRingBuffer(info->in_ringbuffer, info->in_buffer, info->in_block_size);        
        }
        
        // if space to write
        if(PaUtil_GetRingBufferWriteAvailable(info->out_ringbuffer) > info->out_block_size)
        {
        
            // synthesize, and write to the output ringbuffer
            if(audio->callback)
                audio->callback(audio->user_data, info->in_buffer, info->out_buffer);    
            PaUtil_WriteRingBuffer(info->out_ringbuffer, info->out_buffer, info->out_block_size);
            flags |= SYSAUDIO_WRITE_SUCCEEDED;
        }            
    }
    
    // if there is still is stuff ready to output
    if(PaUtil_GetRingBufferReadAvailable(info->out_ringbuffer) > info->out_block_size)
        flags |= SYSAUDIO_STILL_PLAYING;
    
    return flags;    
}

// return number of buffers remaining in the output queue
int buffers_remaining_sys_audio(void *ptr)
{
    PortaudioInfo *info = (PortaudioInfo *)ptr;    
    return PaUtil_GetRingBufferReadAvailable(info->out_ringbuffer) / info->out_block_size;
}

// Shutdown the audio system
int shutdown_sys_audio(void *ptr)
{    
    PortaudioInfo *info = (PortaudioInfo *)ptr;    
              
    if(Pa_IsStreamActive(info->stream))
         Pa_AbortStream(info->stream );            
        
    // free all the data    
    destroy_portaudio_info(info);
        
    return 0;
}


static int paTestCallback( const void *inputBuffer, void *outputBuffer,
                            unsigned long framesPerBuffer,
                            const PaStreamCallbackTimeInfo* timeInfo,
                            PaStreamCallbackFlags statusFlags,
                            void *userData )
{
    
    float *out = (float*)outputBuffer;
    float *in = (float*)inputBuffer;
    PortaudioInfo *info = (PortaudioInfo *) userData;   
    int i;
    
    (void) timeInfo; 
    (void) statusFlags;
    (void) inputBuffer;    

    if(info->ringbuffer_mode)
    {
        // ringbuffer mode, just write/read a block from the ringbuffer, if we can
        
        // input
        if(PaUtil_GetRingBufferWriteAvailable(info->in_ringbuffer) > info->in_block_size)        
            PaUtil_WriteRingBuffer(info->in_ringbuffer, in, info->in_block_size);
                
        // output
        if(PaUtil_GetRingBufferReadAvailable(info->out_ringbuffer) > info->out_block_size)        
        {
            PaUtil_ReadRingBuffer(info->out_ringbuffer, out, info->out_block_size);                 
        }
        else
        {
            // oh dear.
            // A stall has happened, try copying out and then attenuating it
            // get an echo effect instead of buffer mayhem            
            for(i=0;i<GLOBAL_STATE.frames_per_buffer * GLOBAL_STATE.n_channels;i++)
            {
                out[i] = info->out_buffer[i];
                info->out_buffer[i] *= 0.95;
                    
            }
        }
        
    }
    else
    {
        // straightforward synthesis in this thread
        // WARNING! If synthesis doesn't complete on time, very bad things happen        
        if(info->audio_info->callback)
            info->audio_info->callback(info->audio_info->user_data, in, out);    
    
    }
    
    
        
    return paContinue;
}

// audio stream finished
void StreamFinished( void* userData )
{
    PortaudioInfo *info = (PortaudioInfo *) userData;   
    if(info->audio_info->finished_callback)
        info->audio_info->finished_callback(info->audio_info->user_data);
}


void start_sys_audio(void *ptr)
{
    PortaudioInfo *info = (PortaudioInfo *)ptr;
    Pa_StartStream(info->stream);
}

void stop_sys_audio(void *ptr)
{
    PortaudioInfo *info = (PortaudioInfo *)ptr;
    
    Pa_StopStream(info->stream);
}



void pre_init_sys_audio(void)
{
    Pa_Initialize();
}


void post_shutdown_sys_audio(void)
{
    Pa_Terminate();
}



// start a sys_audio stream with the given sample rate, on the given device
void *init_sys_audio(AudioInfo *info)
{
    PaStreamParameters outputParameters;
    PaStreamParameters inputParameters;
    PaStream *stream;
    PaError err;
    PortaudioInfo *paInfo;

    
    if(info->state->in_device<0)
        inputParameters.device = Pa_GetDefaultInputDevice(); /* default input device */
    else
        inputParameters.device = info->state->in_device;         
     inputParameters.channelCount = info->state->n_input_channels;       /* stereo input */
     inputParameters.sampleFormat = paFloat32;
     inputParameters.suggestedLatency = Pa_GetDeviceInfo( inputParameters.device )->defaultLowInputLatency;
     inputParameters.hostApiSpecificStreamInfo = NULL;    

    // open output device
    if(info->state->out_device<0)
        outputParameters.device = Pa_GetDefaultOutputDevice(); /* default output device */
    else
        inputParameters.device = info->state->out_device;         
        
    outputParameters.channelCount = info->state->n_channels;       /* stereo output */
    outputParameters.sampleFormat = paFloat32; /* 32 bit floating point output */
    outputParameters.suggestedLatency = Pa_GetDeviceInfo( outputParameters.device )->defaultLowOutputLatency;
    outputParameters.hostApiSpecificStreamInfo = NULL;

    paInfo = create_portaudio_info(info);
    
    
    
    
    // open input/output stream
    if(info->state->n_input_channels>0)
        err = Pa_OpenStream(
              &stream,
              &inputParameters, /* stereo input */
              &outputParameters,
              info->state->sample_rate,
              info->state->frames_per_buffer,
              paNoFlag,      
              paTestCallback,
              paInfo );
    
    else //open input stream
    err = Pa_OpenStream(
              &stream,
              NULL, /* no input */
              &outputParameters,
              info->state->sample_rate,
              info->state->frames_per_buffer,
              paNoFlag,      
              paTestCallback,
              paInfo );
              
              
    paInfo->stream = stream;
    if( err != paNoError ) goto error;

    // set the callback
    err = Pa_SetStreamFinishedCallback( stream, StreamFinished);
    if( err != paNoError ) goto error;

    return (void*)paInfo;
error:
    Pa_Terminate();
    fprintf( stderr, "An error occured while using the sys_audio stream\n" );
    fprintf( stderr, "Error number: %d\n", err );
    fprintf( stderr, "Error message: %s\n", Pa_GetErrorText( err ) );
    return NULL;

}


// device interrogation


/** Return the number of available audio devices. Includes devices available from all available API's
    @return The number of devices, as an integer. 
    */
int get_n_devices_sys_audio(void)
{
    return Pa_GetDeviceCount();
}



/** Return the name of the device with a given ID.
    @return The device name. 
    */
char *get_name_sys_audio(int device)
{
    const PaDeviceInfo *deviceInfo;
    deviceInfo = Pa_GetDeviceInfo(device);
    return (char *)deviceInfo->name;   
}


int get_max_in_channels_sys_audio(int device)
{
    const PaDeviceInfo *deviceInfo;
    deviceInfo = Pa_GetDeviceInfo(device);
    return deviceInfo->maxInputChannels;
}

int get_max_out_channels_sys_audio(int device)
{
    const PaDeviceInfo *deviceInfo;
    deviceInfo = Pa_GetDeviceInfo(device);
    return deviceInfo->maxOutputChannels;
}


int get_default_output_device_sys_audio(void)
{
    return Pa_GetDefaultOutputDevice();
}

int get_default_input_device_sys_audio(void)
{
       return Pa_GetDefaultInputDevice();
}

int supports_sample_rate_sys_audio(int device, int sample_rate, int for_input)
{
    PaStreamParameters inputParameters, outputParameters;
    const PaDeviceInfo *deviceInfo;
    PaError err;
    
    deviceInfo = Pa_GetDeviceInfo(device);
    
    if(for_input)
    {
        inputParameters.device = device;
        inputParameters.channelCount = deviceInfo->maxInputChannels;
        inputParameters.sampleFormat = paInt16;
        inputParameters.suggestedLatency = 0; /* ignored by Pa_IsFormatSupported() */
        inputParameters.hostApiSpecificStreamInfo = NULL;
        err = Pa_IsFormatSupported(&inputParameters, NULL, sample_rate);
    }
    
    else
    {
        outputParameters.device = device;
        outputParameters.channelCount = deviceInfo->maxOutputChannels;
        outputParameters.sampleFormat = paInt16;
        outputParameters.suggestedLatency = 0; /* ignored by Pa_IsFormatSupported() */
        outputParameters.hostApiSpecificStreamInfo = NULL;       
        err = Pa_IsFormatSupported(NULL, &outputParameters, sample_rate);
    }
        return ( err == paFormatIsSupported );
}

