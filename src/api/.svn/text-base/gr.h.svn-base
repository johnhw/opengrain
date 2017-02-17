/**    
    @file opengrain.h
    @brief The main header file for the public OpenGrain API
    @author John Williamson
    
    Copyright (c) 2011 All rights reserved.
    Licensed under the BSD 3 clause license. See COPYING.
    
    This file is part of the OpenGrain distribution.
    http://opengrain.sourceforge.net   
*/              

#ifndef __OPENGRAIN_API_H__
#define __OPENGRAIN_API_H__

#include "gr_errors.h"

/* This section implemented in base_api.c */

/** Initialises the OpenGrain system. This must be called _before_ any other OpenGrain calls. */
void grInit(void);

/** Shuts down the OpenGrain system, stopping playback, releasing audio devices, shutting down all
    playback threads and freeing all allocated memory */
void grShutdown(void);

/** 
    Return the last error that occured.
    @return The integer error code, or 0 if no error 
**/
int grGetLastError(void);

/** Return the last error that occured as a string description.
    @return The description of the errorm or "No error" if no error.
**/
const char *grGetLastErrorString(void);

/** Get the last error message in full, with any additional information
    that the error might have
*/

char *grGetLastErrorMessage(void);

/*****************************************************************************************/

/* This section implemented in audio_api.c */

/* grAudioParameteri parameters */
#define GR_SAMPLE_RATE 1
#define GR_INPUT_CHANNELS 2
#define GR_OUTPUT_CHANNELS 3
#define GR_LATENCY 5
#define GR_BUFFER_SIZE 6
#define GR_OUTPUT_DEVICE 7
#define GR_INPUT_DEVICE 8
#define GR_PUMP_MODE 9
#define GR_N_DEVICES 10
#define GR_DEFAULT_INPUT_DEVICE 11
#define GR_DEFAULT_OUTPUT_DEVICE 12



/* Devices */
#define GR_DEFAULT_DEVICE -1

/* Pump types */
#define GR_PUMP_MANUAL 0
#define GR_PUMP_THREAD 1
#define GR_PUMP_CALLBACK 2
#define GR_PUMP_BLOCKING 3

/* Audio defaults */
#define GR_DEFAULT_SAMPLE_RATE 44100
#define GR_DEFAULT_BUFFER_SIZE 512
#define GR_DEFAULT_INPUT_CHANNELS 0
#define GR_DEFAULT_OUTPUT_CHANNELS 2
#define GR_DEFAULT_LATENCY 0.01


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
void grAudioParameteri(int parameter, int value);


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
void grAudioParameterf(int parameter, float value);


/** Get a parameter of the audio device. See grAudioParameteri for a list of valid parameters. Additonal parameters:
    GR_N_DEVICES        Number of available devices. Use grAudioDeviceName(x) to get the device name for each device.
    GR_DEFAULT_INPUT_DEVICE        The ID of the default device for input.
    GR_DEFAULT_OUTPUT_DEVICE       The ID of the default device for output.
    @return The value of the parameter
    @arg parameter The parameter to return.
*/
int grGetAudioParameteri(int parameter);

/** Get a parameter of the audio device. See grAudioParameterf for a list of valid parameters. Additional parameters:
    GR_N_DEVICES        Number of available devices. Use grAudioDeviceName(x) to get the device name for each device.    
    GR_DEFAULT_INPUT_DEVICE        The ID of the default device for input.
    GR_DEFAULT_OUTPUT_DEVICE       The ID of the default device for output.
    @return The value of the parameter
    @arg parameter The parameter to return.
*/
int grGetAudioParameterf(int parameter);



/** Return the name of a given device, or NULL if device<0 or >grGetAudioParameteri(GR_N_DEVICES).
    Use grGetAudioParameteri(GR_N_DEVICES) to get the total number of available devices
    @return The name of the device, as returned by the system.
    @arg device The device number, from 0 -> grGetAudioParameteri(GR_N_DEVICES)
*/
const char *grGetAudioDeviceName(int device);

/*  Return the name of the audio driver in use, e.g. "PortAudio" or "Dummy driver" or "iOS".    
    @return A string describing the audio driver in use
**/
const char *grGetDriverName(void);


#define GR_SUPPORTS_INPUT_SAMPLE_RATE 3
#define GR_SUPPORTS_OUTPUT_SAMPLE_RATE 4

#define GR_N_INPUT_CHANNELS 1
#define GR_N_OUTPUT_CHANNELS 2


/** Query properties of an audio device. Queriable properies
    GR_SUPPORTS_INPUT_SAMPLE_RATE     1 if the device supports the given sample rate, 0 otherwise. Sample rate is given in value.
    GR_SUPPORTS_OUTPUT_SAMPLE_RATE     1 if the device supports the given sample rate, 0 otherwise. Sample rate is given in value.
    GR_N_INPUT_CHANNELS            Number of input channels on this device.
    GR_N_OUTPUT_CHANNELS           Number of output channels on this device.
   
    @arg device The device to query
    @arg property The property to query of that device.
    @arg value Value for the propery query (if needed, otherwise just 0)
*/
int grQueryAudioDevice(int device, int property, int value);


/** Initialise the audio and open the device. All calls to grAudioParameteri() should have been executed _before_
    calling this function. By default, will open a 44Khz 16 bit stereo stream on the default
    output device with no inputs.
*/
void grInitAudio(void);

/** Starts the audio playback. When this is called, buffers will be synthesized
    and fed out as needed.
*/    
void grStartAudio(void);

/** Stops/pauses the audio playback. Playback can later be restared by calling grStartAudio() 
*/
void grStopAudio(void);


/** Stops the audio playback, and shuts down the entire audio subsystem. Following this, a call to
    grInitAudio() will be required to reinitialise the audio device. 
    This function should be called at program exit to release the audio device for other processes.
*/
void grShutdownAudio(void);



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
int grPump(int output);

/*****************************************************************************************/

/* This section implemented in mixer_api.c */

/** Set the overall output gain instanteously.
    @arg gaindB Gain, in decibels (e.g. -20.0 = 1/10th, 6.0 ~= 2x)
*/
void grGaindBf(float gaindB);

/** Fade the overall output gain.
    @arg gaindB Gain to fade to
    @arg time Time in seconds to reach gain (actually to be within -80dB of the gain)
*/
void grFadetodBf(float gaindB, float time);

/** Sets the various global effect parameters. Possible parameters are:
    GR_REVERB_ROOM_SIZE             Room size as a factor (1.0 = default, 0.5 = half size etc.)
    GR_REVERB_LEVEL                 Reverb level in decibels.
    GR_REVERB_PREDELAY              Predelay of reverb, in seconds.
    GR_REVERB_DIFFUSE_LEVEL         Additional contribution from diffuse field, in decibels.
    GR_REVERB_DECAY_TIME            Time in seconds to decay to ~36% of maximum level.
    GR_REVERB_DIFFUSION             Diffusion of reverb (0.0--1.0)
    GR_REVERB_MODULATION_FREQUENCY  Frequency of delay modulation as a factor (1.0 = default).
    GR_REVERB_MODULATION_DEPTH      Depth of delay modulation as a factor (1.0 = default, 0.0 = no modulation).
    GR_REVERB_DAMPING               Damping, as a frequency in Hz (gives the -3dB rolloff for the damping)
    GR_REVERB_BANDWIDTH             Bandwidth, as a frequency in Hz (gives the -3dB rolloff for the damping)
    
    GR_WIDENER_DELAY                The inter-channel delay for the stereo widener
    
    GR_COMPRESSOR_THRESHOLD         The knee/transition point for the compressor.
    GR_COMPRESSOR_RATIO             The compression ratio (e.g. 3.0 = 3:1)
    GR_COMPRESSOR_ATTACK            Attack speed of the compressor in 10dB/s units (e.g. 1.0 = 10dB/s attack rate)
    GR_COMPRESSOR_DECAY             Decay speed, in 10dB/s units.
    GR_COMPRESSOR_GAIN              Make-up gain for the compressor, in dB.
    GR_REVERB_MODE                  Selects the reverb algorithm. Can be GR_REVERB_DATTORO or GR_REVERB_RANDOM.      
    @arg parameter The effect parameter to modify.
    @arg value The new value for the given parameter.
*/
void grEffectParameterf(int parameter, float value);

/** Sets the various integer global effect parameters. Possible parameters are:
    GR_REVERB_MODE                  Selects the reverb algorithm. Can be GR_REVERB_DATTORO or GR_REVERB_RANDOM.
    @arg parameter The effect parameter to modify.
    @arg value The new value for the given parameter.
*/
void grEffectParameteri(int parameter, int value);


/** Return a global effect parameter. See grEffectParameterf for a list of valid parameters 
    @arg parameter The effect parameter to modify.
    @return The value of the given effect parameter.
*/
float getEffectParameterf(int parameter);

/** Return am integer global effect parameter. See grEffectParameteri for a list of valid parameters 
    @arg parameter The effect parameter to modify.
    @return The value of the given effect parameter.
*/
int getEffectParameteri(int parameter);


/** Set the output parameteric EQ. Various equalizer sections can be modified. 
    Each section takes a frequency (in Hz) and a gain (in decibels).
    
    Possible EQ sections are
    GR_EQ_LOW           The lowshelf, below which all frequencies will be attenuated/amplified.
    GR_EQ_PEAK_1        The first of the three peaking sections, around which frequencies are altered.
    GR_EQ_PEAK_2        The second peaking section.
    GR_EQ_PEAK_3        The third peaking section.
    GR_EQ_HIGH          The highshelf.
    
    The default is all gains = 0dB and frequencies are 100Hz, 300Hz, 900Hz, 2500Hz and 7000Hz.
*/
void grEq2f(int section, float frequency, float gaindB);


/** Get the output parameteric EQ values. See grEq2f() for details.
    Note that the values are returned via pointers. */
void grGetEq2f(int section, float *frequency, float *gaindB); 
    
    

/*****************************************************************************************/

/* This section implemented in global_api.c */ 

/** Various flags that can be enabled and disabled */

#define GR_REVERB 0
#define GR_EQ 1
#define GR_COMPRESSOR 2
#define GR_CLIPPER 3
#define GR_TEST_TONE 4

#define GR_MAX_FLAGS 4 // update as the above increases

/** Enable one of the global flags.
    @arg flag Flag to enable 
*/
void grEnable(int flag);

/** Disable one of the global flags.
    @arg flag Flag to disable 
*/
void grDisable(int flag);

/** Query the state of a global flag.
    Returns 1 if the given flag is enabled, 0 otherwise.
    @arg flag Flag to query
    @return 0 if flag is disabled, 1 if it is enabled
*/
int grIsEnabled(int flag);

/*****************************************************************************************/

#endif