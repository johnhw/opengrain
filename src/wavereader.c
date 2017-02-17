/**    
    @file wavereader.c
    @brief
    @author John Williamson
    
    Copyright (c) 2011 All rights reserved.
    Licensed under the BSD 3 clause license. See COPYING.
    
    This file is part of the OpenGrain distribution.
    http://opengrain.sourceforge.net   
*/              

#include "wavereader.h"
#include "simplewav.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef USE_LIBRESAMPLE
#include "libresample.h"
#endif




// Load a wave file
WaveSound *create_wave_sound(char *fname)
{
 return soundfile_load(fname);
}


// get a channel of a wavesound. returns the last valid channel
// if channels > number of channels in this sound
Buffer *get_channel_wave_sound(WaveSound *sound, int channel)
{
    if(channel<sound->n_channels)
        return list_get_at(sound->channels, channel);
    else    
        return list_get_at(sound->channels, list_size(sound->channels)-1);    
}


// get the number of bytes needed to store the data in sound in the given byte format
int wave_sound_get_raw_bytes(WaveSound *sound, int format)
{
    int bytes;
    bytes = get_channel_wave_sound(sound, 0)->n_samples;
    
    bytes *= sound->n_channels;
        
    if(format==PCM_SIGNED_8 || format==PCM_UNSIGNED_8)
        return bytes;
    if(format==PCM_SIGNED_16)
        return bytes*sizeof(short);
    if(format==PCM_SIGNED_32)
        return bytes*sizeof(int);
    if(format==PCM_FLOAT)
        return bytes*sizeof(float);
    if(format==PCM_DOUBLE)
        return bytes*sizeof(double);                    
        
    // default is 16 bit
    return bytes * 2;
}


// convert a wavesound into a raw format writing into a buffer
// that must already have been allocated
void wave_sound_to_raw(WaveSound *sound, int format, void *bytes)
{
    int i,j,index;
    Buffer *buffer;
    index = 0;    
            
    for(i=0;i<sound->frames;i++)
    {
        for(j=0;j<sound->n_channels;j++)     
        { 
            buffer = list_get_at(sound->channels, j);  
            
            if(format==PCM_SIGNED_8)
                ((char *)bytes)[index++] = buffer->x[i];
            if(format==PCM_UNSIGNED_8)
                ((unsigned char *)bytes)[index++] = buffer->x[i];
            if(format==PCM_SIGNED_16)
                ((short *)bytes)[index++] = buffer->x[i];
            if(format==PCM_SIGNED_32)
                ((int *)bytes)[index++] = buffer->x[i];
            if(format==PCM_FLOAT)
                ((float *)bytes)[index++] = buffer->x[i];
            if(format==PCM_DOUBLE)
                ((double *)bytes)[index++] = buffer->x[i];
        }
    }
        

}



// convert a PCM byte array into a wavesound object
// byte arrays are expected to be interleaved channel data if they are multichannel
WaveSound *wave_sound_from_raw(void *bytes, int frames, int channels, int sample_rate, int format)
{
    
    Buffer *buffer;
    WaveSound *sound;        
    int i,j, index;
 
    // initialize the structure
    sound = malloc(sizeof(*sound)); 
    sound->frames = frames;
    sound->n_channels = channels;
    sound->sample_rate = sample_rate;
    sound->channels = malloc(sizeof(*sound->channels));
    list_init(sound->channels);
        
    
    // create the buffers
    for(j=0;j<sound->n_channels;j++)
    {
        buffer = create_buffer(sound->frames);
        list_append(sound->channels, buffer);     
    }
        
    index = 0;    
     for(i=0;i<sound->frames;i++)
      {
        for(j=0;j<sound->n_channels;j++)     
        { 
            buffer = list_get_at(sound->channels, j);
            if(format == PCM_SIGNED_8)    
               buffer->x[i] = ((char *)bytes)[index++] /256.0;
            if(format == PCM_UNSIGNED_8)    
               buffer->x[i] = ((unsigned char *)bytes)[index++] / 256.0;
            if(format == PCM_SIGNED_16)    
               buffer->x[i] = ((short *)bytes)[index++] / 32768.0;
            if(format == PCM_SIGNED_32)    
               buffer->x[i] = ((int *)bytes)[index++] / 2147483648.0;
            if(format == PCM_FLOAT)    
               buffer->x[i] = ((float *)bytes)[index++];
            if(format == PCM_DOUBLE)    
               buffer->x[i] = ((double *)bytes)[index++];
                   
        }
      }

    return sound;
}



// return a new buffer which is a resampled version of the input buffer
// doesn't destroy the incoming buffer
Buffer *resample_buffer(Buffer *in, double rate)
{
    void *resample_handle;       
    int used;
    Buffer *resampled;

    // fix unused variable warning if ifdef'd out below
    (void) used;
    (void) resample_handle;
    
     // create the destination buffer
    resampled = create_buffer(in->n_samples * rate + 100);
    
    // resample
#ifdef USE_LIBRESAMPLE
    resample_handle = resample_open(1, rate, rate);      
    resample_process(resample_handle, rate, in->x, in->n_samples, 0, &used, 
    resampled->x, resampled->n_samples);    
    resample_close(resample_handle);    
#else
    linear_resample_buffer(resampled, in, rate);
#endif    
    
    return resampled;
}





// resample a wavesound to a new sample rate
void resample_wave_sound(WaveSound *wave_sound, int new_rate)
{
  int i;
  Buffer *resampled, *original;
   
  // compute sampling rate
  double rate_ratio = (double)new_rate / (double)wave_sound->sample_rate;
  
  // do nothing if sample rate not changed!
  if(new_rate==wave_sound->sample_rate)
    return;
  
  
  for(i=0;i<wave_sound->n_channels;i++)
  {
    original = list_extract_at(wave_sound->channels, 0);    
    resampled = resample_buffer(original, rate_ratio);
    destroy_buffer(original);
    list_append(wave_sound->channels, resampled);
  }
    
 }
  
 


// free a wavesound structure
void destroy_wave_sound(WaveSound *wave_sound)
{
    Buffer *buffer;
    
    // destroy each channel
    list_iterator_start(wave_sound->channels);
    while(list_iterator_hasnext(wave_sound->channels))
    {
        buffer = (Buffer *)list_iterator_next(wave_sound->channels);
        destroy_buffer(buffer);
    
    }
    list_iterator_stop(wave_sound->channels);
    
    // destroy the channel list
    list_destroy(wave_sound->channels);
    free(wave_sound->channels);
    
    free(wave_sound);
}


// load a sequence of filenames of the type "xxx000.wav", "xxx001.wav" etc.
list_t *create_soundbank(char *basename)
{    
    int index;
    char fname[1024];
    list_t *bank;
    WaveSound *sound;
    
    bank = malloc(sizeof(*bank));
    list_init(bank);
      
    index = 0;
    do
    {
        sprintf(fname, "%s%03d.wav", basename, index++);
        sound = create_wave_sound(fname);
        if(sound)       
            list_append(bank, sound);        
        
    
    } while(sound);

    return bank;
}

// destroy a list of wavesounds
void destroy_soundbank(list_t *bank)
{
    WaveSound *sound;
    list_iterator_start(bank);
    
    
    while(list_iterator_hasnext(bank))
    {
        sound = (WaveSound *) list_iterator_next(bank);
        destroy_wave_sound(sound);    
    }
    list_iterator_stop(bank);
    list_destroy(bank);
    free(bank);
}
