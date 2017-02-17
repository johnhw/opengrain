/**    
    @file wavewriter.c
    @brief
    @author John Williamson
    
    Copyright (c) 2011 All rights reserved.
    Licensed under the BSD 3 clause license. See COPYING.
    
    This file is part of the OpenGrain distribution.
    http://opengrain.sourceforge.net   
*/              

#include "wavewriter.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>



// Create a wavewriter, writing in 16 bit stereo to files called output_000.wav etc.
WaveWriter* create_wavewriter(void)
{
    WaveWriter *writer = malloc(sizeof(*writer));
    writer->channel_mode = MULTICHANNEL_INTERLEAVE;
    writer->overwrite_mode = OVERWRITE_MODE_NEW_FILE;
    writer->bit_depth = WAVEWRITER_BIT_DEPTH_16;
    writer->path = "output";
    writer->write_buffer = NULL;    
    writer->writing = 0;
    writer->channels = 0;
    writer->out_channels = 0;
    writer->speaker_locations = NULL;
    writer->handles = malloc(sizeof(*writer->handles));
    set_multichannel_wavewriter(writer, 2, MULTICHANNEL_INTERLEAVE);
    list_init(writer->handles);    
    return writer;
}


// Free a wavewriter object, closing the current file if it is open
void destroy_wavewriter(WaveWriter *writer)
{
    
    if(writer->writing) stop_wavewriter(writer);    
    list_destroy(writer->handles);
    free(writer->handles);    
    
    free(writer);   
}


// set a new filename. If writing, close the old file and open a new one.
void set_path_wavewriter(WaveWriter *writer, char *path)
{
    // path is unchanged, do nothing
    if(!strcmp(path, writer->path))
        return;
        
    if(writer->writing)
    {
        stop_wavewriter(writer);
        writer->path=path;
        start_wavewriter(writer);        
    }
    else
    {
        writer->path = path;
    }

}

// set the speaker locations, for HRTF multichannel output
void set_speaker_locations_wavewriter(WaveWriter *writer, list_t *locations)
{
    if(list_size(locations)!=writer->channels)
        opengrain_error("Number of speakers does not match the number of channels set on this wavewriter");
    writer->speaker_locations = locations;
}

// set the overwrite mode
void set_overwrite_wavewriter(WaveWriter *writer, int overwrite_mode)
{
    writer->overwrite_mode = overwrite_mode;

}


// set the stereo mode, closing and reopening the file if necessary
void set_multichannel_wavewriter(WaveWriter *writer, int channels, int channel_mode)
{

    int out_channels;
    if(writer->channel_mode==channel_mode && channels==writer->channels)
        return;
    
    // multiple output files if separate is used
    if(channel_mode==MULTICHANNEL_SEPARATE)
        out_channels = channels;
    else
        out_channels = 1;        
    
    if(writer->writing)
    {
        stop_wavewriter(writer);
        writer->channel_mode = channel_mode;
        writer->channels = channels;
        writer->out_channels = out_channels;
        start_wavewriter(writer);        
    }
    else
    {
        writer->channel_mode = channel_mode;
        writer->channels = channels;
        writer->out_channels = out_channels;
    }

}

// set the bit depth, closing and reopening the file if necessary
void set_bit_depth_wavewriter(WaveWriter *writer, int bit_depth)
{
     if(writer->bit_depth==bit_depth)
        return;
        
    if(writer->writing)
    {
        stop_wavewriter(writer);
        writer->bit_depth = bit_depth;
        start_wavewriter(writer);        
    }
    else
    {
        writer->bit_depth = bit_depth;
    }

}


// test if path already exists
int file_exists(char *path)
{
    FILE *exist_test;
    int file_exists=0;
   exist_test = fopen(path, "r");
    if(exist_test) 
    {
        file_exists=1;
        fclose(exist_test);
    }
    return file_exists;
}


// open a new wav file for writing to
void start_wavewriter(WaveWriter *writer)
{   
    int exists, index, i;
    int file_channels;
    char output_path_name[1024];
    void *handle;
   
    // restart writing if already writing
    if(writer->writing)
        stop_wavewriter(writer);
        
    if(writer->channels==0)
        opengrain_error("Channels == 0 when starting wavewriting.");
    
    
    // find a new file to write into
    if(writer->overwrite_mode == OVERWRITE_MODE_NEW_FILE)
    {
        exists = 0;
        index = 0;
        do
        {
            if(writer->channel_mode==MULTICHANNEL_SEPARATE)
                sprintf(output_path_name, "%s_%03d_c0.wav", writer->path, index);
            else
                sprintf(output_path_name, "%s_%03d.wav", writer->path, index);
            index++;
        }
        while(file_exists(output_path_name));                
    }
    else
    {
        sprintf(output_path_name, "%s.wav", writer->path);
    }
    
    // how many output channels per file
    if(writer->channel_mode == MULTICHANNEL_MIX || writer->channel_mode == MULTICHANNEL_SEPARATE)
        file_channels = 1;
        
    if(writer->channel_mode == MULTICHANNEL_HRTF)
        file_channels = 2;
        
    if(writer->channel_mode == MULTICHANNEL_INTERLEAVE)
        file_channels = writer->channels;
                   
    // allocate some memory for the output
    writer->write_buffer = create_buffer(writer->channels*GLOBAL_STATE.frames_per_buffer);
    
    if(writer->channel_mode==MULTICHANNEL_SEPARATE)
    {
        // open one file for each output channel
        for(i=0;i<writer->channels;i++)
        {
            sprintf(output_path_name, "%s_%03d_c%d.wav", writer->path, index-1, i);
            handle = soundfile_open(file_channels, GLOBAL_STATE.sample_rate, WAVEWRITER_BIT_DEPTH_16, writer->overwrite_mode, &output_path_name[0]);
            list_append(writer->handles, handle);
        }
    }
    else
    {
        handle = soundfile_open(file_channels, GLOBAL_STATE.sample_rate, WAVEWRITER_BIT_DEPTH_16, writer->overwrite_mode, &output_path_name[0]);
        list_append(writer->handles, handle);
    }
    writer->writing = 1;    
        
}    
        
    

// close the currently active file and NULL the handle
void stop_wavewriter(WaveWriter *writer)
{
    int i;
    void *handle;
    if(writer->out_channels)
    {
        for(i=0;i<writer->out_channels;i++)
        {
            handle = list_get_at(writer->handles, i);
            soundfile_stop(handle);    
        }
    }
    if(writer->write_buffer)
        destroy_buffer(writer->write_buffer);
    writer->write_buffer = NULL;
    writer->writing = 0;
}


// Write a stereo buffer to disk
void write_wavewriter(WaveWriter *writer, Buffer **channels)
{
    int i, j,k;
    void *handle;
    if(!writer->writing)
        return;
        
    j = 0;
    
    // interleaved data
    if(writer->channel_mode == MULTICHANNEL_INTERLEAVE)
    {
        for(i=0;i<channels[0]->n_samples;i++)
        {
            for(k=0;k<writer->channels;k++)            
                writer->write_buffer->x[j++] = channels[k]->x[i];            
        }           
        
        handle = list_get_at(writer->handles, 0);
        // write the data to disk        
        soundfile_write(handle,  writer->write_buffer->x, channels[0]->n_samples*writer->channels);    
    }
    
    // mixed data
    if(writer->channel_mode == MULTICHANNEL_MIX)
    {
        zero_buffer(writer->write_buffer);
        for(i=0;i<channels[0]->n_samples;i++)
        {
            for(k=0;k<writer->channels;k++)            
                writer->write_buffer->x[i] += channels[k]->x[i] / writer->channels;            
        }           
        
        handle = list_get_at(writer->handles, 0);
        // write the data to disk        
        soundfile_write(handle,  writer->write_buffer->x, channels[0]->n_samples);    
    }
    
    // separate file data
    if(writer->channel_mode == MULTICHANNEL_SEPARATE)
    {
        for(k=0;k<writer->channels;k++)            
        {
            for(i=0;i<channels[0]->n_samples;i++)                       
                writer->write_buffer->x[i] = channels[k]->x[i];                                              
        handle = list_get_at(writer->handles, k);
        // write the data to disk        
        soundfile_write(handle,  writer->write_buffer->x, channels[k]->n_samples);    
        }                
    }
    
    // hrtf faked surround
    if(writer->channel_mode == MULTICHANNEL_HRTF)
    {
        if(!writer->speaker_locations)
            opengrain_error("Speaker locations not set when using HRTF mode in a wavewriter!");
        else
        {
            for(k=0;k<writer->channels;k++)            
            {
                for(i=0;i<channels[0]->n_samples;i++)                           
                    writer->write_buffer->x[i] = channels[k]->x[i];            
                                       //fix
                handle = list_get_at(writer->handles, 0);
                // write the data to disk        
                soundfile_write(handle,  writer->write_buffer->x, 2*channels[0]->n_samples);    
            }
        }
    }
    
    
}