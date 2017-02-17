/**    
    @file libsndfile.c
    @brief Sound file reading and writing with libsndfile
    @author John Williamson
    
    Copyright (c) 2011 All rights reserved.
    Licensed under the BSD 3 clause license. See COPYING.
    
    This file is part of the OpenGrain distribution.
    http://opengrain.sourceforge.net   
*/ 


#include "wavereader.h"
#include "wavewriter.h"
#include "soundfile.h"
#include <sndfile.h>


// load a wave file using libsndfile
WaveSound *soundfile_load(char *fname)
{
 SF_INFO file_info;
 SNDFILE *sound_file;
 float *sound_data;
 WaveSound *sound;
 Buffer *buffer;
 int sample_rate;
 int i,j;
 float *ptr;
 
 sound = malloc(sizeof(*sound)); 
 file_info.format = 0; // read format from file
 sound_file = sf_open(fname, SFM_READ, &file_info);
 
 if(sound_file==NULL)
    {
        fprintf(stderr, "Could not load wave file %s\n", fname);
        return NULL;    
    }
        
 sound_data = (float *)calloc(sizeof(*sound_data)*file_info.channels*file_info.frames,1);  
 // read the floats
 sf_readf_float(sound_file, sound_data, file_info.frames); 
 
 sample_rate = file_info.samplerate;

 // set the parameters in the structure 
 sound->sample_rate = sample_rate;
 sound->n_channels = file_info.channels;
 sound->frames = file_info.frames;
 
 sound->channels = malloc(sizeof(*sound->channels));
 list_init(sound->channels);
 
 // create the buffers
 for(j=0;j<sound->n_channels;j++)
 {
    buffer = create_buffer(file_info.frames);
    list_append(sound->channels, buffer);     
 }
 
 
 // de-interleave
 ptr = sound_data; 
    
    
 for(i=0;i<file_info.frames;i++)
  {
    for(j=0;j<sound->n_channels;j++)     
    { 
        buffer = list_get_at(sound->channels, j);
        buffer->x[i] = *ptr++;        
    }
  }

 
 // free the float buffer and close the file
 free(sound_data);  
 sf_close(sound_file);
 
 return sound; 
}




void *soundfile_open(int channels, int sample_rate, int bit_depth, int overwrite_mode, char *output_path_name)
{
    void *handle;
    SF_INFO sndfile_info;
       
    sndfile_info.frames = 0;
    // Sample rate is locked to the global sample rate
    sndfile_info.samplerate = sample_rate;
    sndfile_info.sections = 0;
    sndfile_info.seekable = 0;
    
    sndfile_info.channels = channels;
    
    // always write WAV files
    sndfile_info.format = SF_FORMAT_WAV | SF_ENDIAN_FILE;
    
    // set the bit depth field
    if(bit_depth == WAVEWRITER_BIT_DEPTH_8)
        sndfile_info.format |= SF_FORMAT_PCM_U8;
    
    if(bit_depth == WAVEWRITER_BIT_DEPTH_16)
        sndfile_info.format |= SF_FORMAT_PCM_16;
        
    if(bit_depth == WAVEWRITER_BIT_DEPTH_24)
        sndfile_info.format |= SF_FORMAT_PCM_24;
        
    if(bit_depth == WAVEWRITER_BIT_DEPTH_FLOAT)
        sndfile_info.format |= SF_FORMAT_FLOAT;
    
       
    // either append or ovewrite the file
    if(overwrite_mode == OVERWRITE_MODE_APPEND && file_exists(output_path_name))
    {
        handle = sf_open(output_path_name, SFM_RDWR, &sndfile_info);
        if(handle)
            sf_seek(handle, 0, SEEK_END);
    }
    else
    {
        handle = sf_open(output_path_name, SFM_WRITE, &sndfile_info);
    }
    
    
    // print an error if something bad happens
    // note that this will just result in no audio being written
    if(!handle)
    {
        opengrain_error((char *)sf_strerror(NULL));            
    }   
    return handle;    
}


void soundfile_stop(void *handle)
{
    sf_write_sync(handle);
    sf_close(handle);    
}


void soundfile_write(void *handle, float *data, int n)
{
    sf_write_float(handle, data, n);
}
