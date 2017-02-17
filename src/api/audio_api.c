/**    
    @file audio_api.c
    @brief Implementation of the audio device parts of the opengrain api
    @author John Williamson
    
    Copyright (c) 2011 All rights reserved.
    Licensed under the BSD 3 clause license. See COPYING.
    
    This file is part of the OpenGrain distribution.
    http://opengrain.sourceforge.net   
*/             
#include "gr.h"
#include "errors.h"
#include "base_api.h"
#include "../audio.h"
#include "../sys_audio.h"
#include "../output.h"


/** 
    Set a parmeter of the audio device. Valid values for parameter are:
    
    GR_SAMPLE_RATE      The output sample rate, in Hz. Default is 44100
    GR_INPUT_CHANNELS   The number of input channels to use. Default is 0.
    GR_OUTPUT_CHANNELS  The number of output channels to use  (e.g. 2 for stereo). Default is 2.
    GR_BUFFER_SIZE      Buffer size in samples. Smaller buffers are less efficient but allow smoother effects. Default size is 512.
                        Note that buffer size and latency are different things; buffer sizes can be smaller or larger than the 
                        underlying audio buffer.
    GR_OUTPUT_DEVICE    The device ID to use for output. GR_DEFAULT_DEVICE is always the default output device. Default is GR_DEFAULT_DEVICE.
    GR_INPUT_DEVICE    The device ID to use for output. GR_DEFAULT_DEVICE is always the default output device. Default is GR_DEFAULT_DEVICE.   
    GR_PUMP_MODE        The output mode to use. Can be GR_PUMP_MANUAL (user must call grPump() regularly), 
                        GR_PUMP_THREAD (background thread pumps automatically), GR_PUMP_BLOCKING (blocks until synthesis
                        is complete) or GR_PUMP_CALLBACK (minimum latency
                        but may cause stability issues if synthesis takes a long time). Default is GR_PUMP_THREAD                      
   
    
*/    
void grAudioParameteri(int parameter, int value)
{
    switch(parameter)
    {
        case GR_SAMPLE_RATE:
            gr_context->prototype->sample_rate = value;
            break;
        case GR_OUTPUT_CHANNELS:
            gr_context->prototype->n_channels = value;
            break;
        case GR_INPUT_CHANNELS:
            gr_context->prototype->n_input_channels = value;
            break;
        case GR_BUFFER_SIZE:
            gr_context->prototype->frames_per_buffer = value;
            break;  
        case GR_PUMP_MODE:
            gr_context->pump_mode = value;
            break;            
        case GR_INPUT_DEVICE:
            gr_context->prototype->in_device = value;
            break;
        case GR_OUTPUT_DEVICE:        
            gr_context->prototype->out_device = value;
            break;
        default:
            grError(GR_ERROR_BAD_PARAMETER, "Invalid parameter code %d for querying in grAudioParameteri", parameter);
            return;
            break;            
    }

}


/** 
    Set a parmeter of the audio device. Valid values for parameter are:
    
    GR_SAMPLE_RATE      The output sample rate, in Hz. Default is 44100
    GR_INPUT_CHANNELS   The number of input channels to use. Default is 0.
    GR_OUTPUT_CHANNELS  The number of output channels to use  (e.g. 2 for stereo). Default is 2.
    GR_LATENCY          Desired latency in seconds. This value is not guaranteed to be acheived; use grGetAudioParameteri(GR_LATENCY)
                        to determine the actual latency. Default is 0.01 (10ms).
    GR_BUFFER_SIZE      Buffer size in samples. Smaller buffers are less efficient but allow smoother effects. Default size is 512.
    GR_OUTPUT_DEVICE    The device ID to use for output. GR_DEFAULT_DEVICE is always the default output device. Default is GR_DEFAULT_DEVICE.
    GR_INPUT_DEVICE    The device ID to use for output. GR_DEFAULT_DEVICE is always the default output device. Default is GR_DEFAULT_DEVICE.   
    GR_PUMP_MODE        The output mode to use. Can be GR_PUMP_MANUAL (user must call grPump() regularly), 
                        GR_PUMP_THREAD (background thread pumps automatically) or GR_PUMP_CALLBACK (minimum latency
                        but may cause stability issues. Default is GR_PUMP_THREAD                
*/    
void grAudioParameterf(int parameter, float value)
{
    switch(parameter)
    {  
        case GR_SAMPLE_RATE:
        case GR_INPUT_CHANNELS:
        case GR_OUTPUT_CHANNELS:
        case GR_BUFFER_SIZE:        
        case GR_PUMP_MODE:
            grAudioParameteri(parameter, value);   
            break;            
        case GR_LATENCY:
            gr_context->prototype->latency = value;
            break;
        default:
            grError(GR_ERROR_BAD_PARAMETER, "Invalid parameter code %d for querying in grAudioParameterf", parameter);
            return;
            break;
    }
}


/** Get a parameter of the audio device. See grAudioParameteri for a list of valid parameters. Additonal parameters:
    GR_N_DEVICES        Number of available devices. Use grAudioDeviceName(x) to get the device name for each device.
    GR_DEFAULT_INPUT_DEVICE        The ID of the default device for input.
    GR_DEFAULT_OUTPUT_DEVICE       The ID of the default device for output.
    @return The value of the parameter
    @arg parameter The parameter to return.
*/
int grGetAudioParameteri(int parameter)
{
    switch(parameter)
    {    
        case GR_PUMP_MODE:
            return gr_context->pump_mode;
            break;
        case GR_SAMPLE_RATE:
            return gr_context->prototype->sample_rate;    
            break;
        case GR_INPUT_CHANNELS:
            return gr_context->prototype->n_input_channels;    
            break;
        case GR_OUTPUT_CHANNELS:
            return gr_context->prototype->n_channels;
            break;
          case GR_BUFFER_SIZE:
            return gr_context->prototype->frames_per_buffer;
        case GR_INPUT_DEVICE:
            return gr_context->prototype->in_device;
            break;
        case GR_OUTPUT_DEVICE:    
            return gr_context->prototype->out_device;
            break;
        case GR_DEFAULT_OUTPUT_DEVICE:
            return get_default_output_device_sys_audio();
            break;
        case GR_DEFAULT_INPUT_DEVICE:
            return get_default_input_device_sys_audio();
            break;                        
        case GR_N_DEVICES:
            return get_n_devices_sys_audio();
            break;
        default:
            grError(GR_ERROR_BAD_PARAMETER, "Invalid parameter code %d for in grAudioParameteri", parameter);
            return 0;
            break;
    }
    

}

/** Get a parameter of the audio device. See grAudioParameterf for a list of valid parameters. Additional parameters:
    GR_N_DEVICES        Number of available devices. Use grAudioDeviceName(x) to get the device name for each device.    
    GR_DEFAULT_INPUT_DEVICE        The ID of the default device for input.
    GR_DEFAULT_OUTPUT_DEVICE       The ID of the default device for output.
    @return The value of the parameter
    @arg parameter The parameter to return.
*/
int grGetAudioParameterf(int parameter)
{
    switch(parameter)
    {  
        case GR_SAMPLE_RATE:
        case GR_INPUT_CHANNELS:
        case GR_OUTPUT_CHANNELS:
        case GR_BUFFER_SIZE:
        case GR_INPUT_DEVICE:
        case GR_OUTPUT_DEVICE:        
        case GR_DEFAULT_OUTPUT_DEVICE:
        case GR_DEFAULT_INPUT_DEVICE:        
        case GR_PUMP_MODE:
            return (float) grGetAudioParameteri(parameter);    
        case GR_LATENCY:
            return gr_context->prototype->latency;
            break;
        default:
            grError(GR_ERROR_BAD_PARAMETER, "Invalid parameter code %d for querying in grGetAudioParameterf", parameter);
            return 0;
            break;
    }
}


/** Return the name of a given device, or NULL if device<0 or >grGetAudioParameteri(GR_N_DEVICES).
    Use grGetAudioParameteri(GR_N_DEVICES) to get the total number of available devices
    @return The name of the device, as returned by the system.
    @arg device The device number, from 0 -> grGetAudioParameteri(GR_N_DEVICES)
*/
const char *grGetAudioDeviceName(int device)
{
    int max_devices; 
    max_devices = grGetAudioParameteri(GR_N_DEVICES);
    
    // check device ID is valid
    if(device<0 || device>=max_devices)
    {
        grError(GR_ERROR_BAD_DEVICE, "Invalid device number %d for querying in grAudioDeviceName", device);
        return NULL;
    }        
    return (const char*)get_name_sys_audio(device);
}


/*  Return the name of the audio driver in use, e.g. "PortAudio" or "Dummy driver" or "iOS".    
    @return A string describing the audio driver in use
**/
const char *grGetDriverName(void)
{
    return driver_name_sys_audio;
}


/** Query properties of an audio device. Queriable properies
    GR_SUPPORTS_INPUT_SAMPLE_RATE     1 if the device supports the given sample rate, 0 otherwise. Sample rate is given in value.
    GR_SUPPORTS_OUTPUT_SAMPLE_RATE     1 if the device supports the given sample rate, 0 otherwise. Sample rate is given in value.
    GR_N_INPUT_CHANNELS            Number of input channels on this device.
    GR_N_OUTPUT_CHANNELS           Number of output channels on this device.
    @arg device The device to query
    @arg property The property to query of that device.
    @arg value Value for the propery query (if needed, otherwise just 0)
*/
int grQueryAudioDevice(int device, int property, int value)
{
    int max_devices; 
    max_devices = grGetAudioParameteri(GR_N_DEVICES);
    
    // check device ID is valid
    if(device<0 || device>=max_devices)
    {
        grError(GR_ERROR_BAD_DEVICE, "Invalid device number %d for querying in grQueryAudioDevice", device);
        return -1;
    }    
    
    switch(property)
    {        
        case GR_SUPPORTS_INPUT_SAMPLE_RATE:
            return supports_sample_rate_sys_audio(device, value, 1);
            break;
        case GR_SUPPORTS_OUTPUT_SAMPLE_RATE:
            return supports_sample_rate_sys_audio(device, value, 0);
            break;          
        case GR_N_INPUT_CHANNELS:
            return get_max_in_channels_sys_audio(device);
            break;
        case GR_N_OUTPUT_CHANNELS:
            return get_max_out_channels_sys_audio(device);
            break;
        default:
            grError(GR_ERROR_BAD_PARAMETER, "Invalid parameter code %d for querying in grQueryAudioDevice", property);
            return -1;
            break;
    }

}


/** Initialise the audio and open the device. All calls to grAudioParameteri() should have been executed _before_
    calling this function. By default, will open a 44Khz 16 bit stereo stream on the default
    output device with no inputs.
*/
void grInitAudio(void)
{
    
    // open audio device
    init_audio(gr_context->prototype, audio_callback_output_info, audio_finished_output_info, &gr_context->output_info);    
    gr_context->output_info = create_output_info(); 
    
    // set up pumping            
        
}

/** Starts the audio playback. When this is called, buffers will be synthesized
    and fed out as needed.
*/    
void grStartAudio(void)
{
    start_audio();
}

/** Stops/pauses the audio playback. Playback can later be restared by calling grStartAudio() 
*/
void grStopAudio(void)
{
    stop_audio();
}


/** Stops the audio playback, and shuts down the entire audio subsystem. Following this, a call to
    grInitAudio() will be required to reinitialise the audio device. 
    This function should be called at program exit to release the audio device for other processes.
*/
void grShutdownAudio(void)
{
    shutdown_audio();
    destroy_output_info(gr_context->output_info);

}

/** Set the default audio settings. 
    44.1Khz, 16 bit, stereo output on the default device, no input channels, manual pumping
*/
void set_default_audio_api(void)
{
    // audio pump things
    gr_context->pump_mode = GR_PUMP_MANUAL;
    gr_context->pump_thread_handle = NULL;    
    gr_context->pump_thread_data = NULL;
    
    // allocate audio prototype
    gr_context->prototype =  malloc(sizeof(*gr_context->prototype));   
        
    // initialise the prototype
    gr_context->prototype->sample_rate = GR_DEFAULT_SAMPLE_RATE;
    gr_context->prototype->frames_per_buffer = GR_DEFAULT_SAMPLE_RATE;
    gr_context->prototype->n_channels = GR_DEFAULT_OUTPUT_CHANNELS;
    gr_context->prototype->n_input_channels = GR_DEFAULT_INPUT_CHANNELS;
    gr_context->prototype->input_channel = 0;
    gr_context->prototype->in_device = GR_DEFAULT_DEVICE;
    gr_context->prototype->out_device = GR_DEFAULT_DEVICE;        
    gr_context->prototype->latency = GR_DEFAULT_LATENCY;

}


/** Pump the audio device; synthesisze one buffers worth, and prepare it for output. 
    This is only needed if the GR_PUMP_MANUAL mode is set for GR_PUMP_MODE; otherwise
    another thread or the audio device callback will automatically process buffers.
    
    Returns number of buffers in queue to output, or 0 if none. If output is 
    1 does synthesis and generates a new buffer; otherwise just feeds through existing ones.
    So to output 10 seconds of data.
    for(i=0;i<10*SAMPLE_RATE/BUFFER_SIZE;i++)
        grPump(1);
    while(grPump(0));
    @return The number of buffers in the queue.
    @arg output Whether to do synthesis, or just pass a buffer onto to the audio device.
    */
int grPump(int output)
{
    return pump_audio(output);
}