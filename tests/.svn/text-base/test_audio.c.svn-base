/**    
    @file test_audio.c
    @brief Tests the audio device querying capabilities. Lists all attached devices and their properties.
    @author John Williamson
    
    Copyright (c) 2011 All rights reserved.
    Licensed under the BSD 3 clause license. See COPYING.
    
    This file is part of the OpenGrain distribution.
    http://opengrain.sourceforge.net   
*/       

#include <stdio.h>
#include <stdlib.h>
#include <gr.h>


int main(int argc, char **argv)
{
    int i, j, devices, default_in, default_out;
    int sample_rates[5] = {8000, 11025, 22050, 44100, 96000};
    
    grInit();
    
    printf("\n\n\n\nUsing driver: %s\n", grGetDriverName());
    devices = grGetAudioParameteri(GR_N_DEVICES);
    
    default_in = grGetAudioParameteri(GR_DEFAULT_INPUT_DEVICE);
    default_out = grGetAudioParameteri(GR_DEFAULT_INPUT_DEVICE);
    
    printf("Default input device is %d: %s\n", default_in, grGetAudioDeviceName(default_in));
    printf("Default output device is %d: %s\n", default_out, grGetAudioDeviceName(default_out));
    
    
    printf("Available devices:\n");
    for(i=0;i<devices;i++)
    {
        printf("Device %d: %s\n", i, grGetAudioDeviceName(i));
        printf("    Input channels: %d\n", grQueryAudioDevice(i, GR_N_INPUT_CHANNELS, 0));
        printf("    Output channels: %d\n", grQueryAudioDevice(i, GR_N_OUTPUT_CHANNELS, 0));                
        for(j=0;j<5;j++)
        {
            if(grQueryAudioDevice(i, GR_SUPPORTS_INPUT_SAMPLE_RATE, sample_rates[j]))
                printf("        Supports %dHz sample rate for input\n", sample_rates[j]);                
            if(grQueryAudioDevice(i, GR_SUPPORTS_OUTPUT_SAMPLE_RATE, sample_rates[j]))
                printf("        Supports %dHz sample rate for output\n", sample_rates[j]);                
        }
        printf("\n\n");
                
    }            
    
    grAudioParameteri(GR_INPUT_DEVICE, GR_DEFAULT_DEVICE);
    grAudioParameteri(GR_OUTPUT_DEVICE, GR_DEFAULT_DEVICE);
    grAudioParameteri(GR_SAMPLE_RATE, 44100);
    grAudioParameteri(GR_INPUT_CHANNELS, 0);
    grAudioParameteri(GR_OUTPUT_CHANNELS, 2);
    grAudioParameteri(GR_PUMP_MODE, GR_PUMP_MANUAL);
    grAudioParameteri(GR_BUFFER_SIZE, 2048);
    grAudioParameterf(GR_LATENCY, GR_DEFAULT_LATENCY);
    
    
    
    grInitAudio();
    grStartAudio();
    
    grStopAudio();    
    grShutdownAudio();
            
    grShutdown();
    return 0;
}