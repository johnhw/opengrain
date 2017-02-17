/**    
    @file hrtf.c
    @brief Implements an head-related transfer function system for 3D audio rendering. Uses an FFT based convolution method.
    @author John Williamson
    
    Copyright (c) 2011 All rights reserved.
    Licensed under the BSD 3 clause license. See COPYING.
    
    This file is part of the OpenGrain distribution.
    http://opengrain.sourceforge.net   
*/              

#include "hrtf.h"


const float MIT_azimuth_increments[] = {6.43, 6.0, 5.0, 5.0, 5.0, 5.0, 5.0, 6.0, 6.43, 8.0, 10.0, 15.0, 30.0, 180.0};



// create a complete HRTF model, and load the HRTF impulses from the given path
// (MIT compact model)
HRTFModel *create_hrtf_model(char *path)
{
    HRTFModel *model;
    model = malloc(sizeof(*model));
    model->impulses = malloc(sizeof(*model->impulses));    
    list_init(model->impulses);        
    model->buffer_size = 128; 
    model->fft = create_fft(model->buffer_size * 2);
    
    
    load_MIT_hrtf_model(model, path);
    return model;
        
}

// create an impulse with an HRTF impulse and an azimuth and elevation
HRTFImpulse *create_hrtf_impulse(WaveSound *sound, float azimuth, float elevation, FFT *fft, int switch_channels)
{
    HRTFImpulse *impulse;
    impulse = malloc(sizeof(*impulse));
    impulse->sound = malloc(sizeof(*impulse->sound));
    
    // copy the impulse references
    impulse->sound->left = create_buffer(fft->n_samples);
    impulse->sound->right = create_buffer(fft->n_samples);
    
    zero_buffer(impulse->sound->left);
    zero_buffer(impulse->sound->right);
    
    // switch left ear for right ear for the samples on the other side
    if(switch_channels)
    {
        mix_buffer(impulse->sound->right, get_channel_wave_sound(sound, 1), 1.0);
        mix_buffer(impulse->sound->left, get_channel_wave_sound(sound, 0), 1.0);
    }
    else
    {
        mix_buffer(impulse->sound->left, get_channel_wave_sound(sound, 1), 1.0);
        mix_buffer(impulse->sound->right, get_channel_wave_sound(sound, 0), 1.0);
    }
                
    
    // fft the impulses
    impulse->sound->left_fft = create_complex_buffer(fft->n_samples/2+1);
    impulse->sound->right_fft = create_complex_buffer(fft->n_samples/2+1);
    
    zero_complex_buffer(impulse->sound->left_fft);
    zero_complex_buffer(impulse->sound->right_fft);
    
    
    fft_buffer(fft, impulse->sound->left, impulse->sound->left_fft);
    fft_buffer(fft, impulse->sound->right, impulse->sound->right_fft);
    
    // set the position of this impulse
    impulse->azimuth = azimuth;
    impulse->elevation = elevation;   

    return impulse;
}


// destroy and impulse, and free the wave data
void destroy_hrtf_impulse(HRTFImpulse *impulse)
{
    free(impulse->sound);
    free(impulse);    
}





// Load an MIT compact HRTF model from the path pointed to by path
void load_MIT_hrtf_model(HRTFModel *model, char *path)
{
    int az, el;
    int j;
    float increment, azimuth;
    char fname[1024];
    HRTFImpulse *impulse;
    WaveSound *sound;
        
    j = 0;
    // elevation steps
    for(el=-40;el<100;el+=10)
    {
        
        // get the azimuth increment for this step
        increment = MIT_azimuth_increments[j++];
        azimuth = 0.0;
        // load each azimuth impulse for this elevation
        while(azimuth<180.0)
        {
             
             az = (int) (azimuth+0.5);
             // compute pathname
             sprintf(fname, "%s/elev%d/H%de%03da.wav", path, el, el, az);
    
             sound = create_wave_sound(fname);
             
             if(sound)
             {
                 // load stereo hrtf impulse, and put it in the list
                impulse = create_hrtf_impulse(sound, azimuth, el, model->fft, 0);                                                       
                list_append(model->impulses, impulse);                
                impulse = create_hrtf_impulse(sound,  (azimuth-180), el, model->fft, 1);                                       
                list_append(model->impulses, impulse);                            
            }
            azimuth += increment;
        }           
        
    }
     
}



// free up an HRTF model and all its associated impulses
void  destroy_hrtf_model(HRTFModel *model)
{

    HRTFImpulse *impulse;
    
    // free all the impulses
    list_iterator_start(model->impulses);
    while(list_iterator_hasnext(model->impulses))
    {
        impulse = (HRTFImpulse *) list_iterator_next(model->impulses);
        destroy_hrtf_impulse(impulse);        
    }
        
    list_iterator_stop(model->impulses);
    list_destroy(model->impulses);
    
    destroy_fft(model->fft);
    free(model->impulses);
    free(model);
}



// return the great circle distance between two azimuth elevation co-ordinates
double great_circle_distance(float az1, float el1, float az2, float el2)
{
    double lat1, lon1, lat2, lon2, d, p1, p2;
    
    lon1 = TO_RADIANS(az1);
    lat1 = TO_RADIANS(el1);
    lon2 = TO_RADIANS(az2);
    lat2 = TO_RADIANS(el2);
    
    p1 = (sin((lat1-lat2)/2));
    p2 = (sin((lon1-lon2)/2));

    d = 2*asin(sqrt((p1*p1) + cos(lat1)*cos(lat2)*(p2*p2)));        
    return d;
   
}


// get the nearest HRTF impulse to a given azimuth/elevation
HRTFImpulse *get_hrtf(HRTFModel *model, float azimuth, float elevation, int flags)
{
    HRTFImpulse *impulse, *min_impulse;
    double distance;
    double min_distance = HUGE_VAL;    
    int valid;
    
    min_impulse = NULL;        

    
    // find nearest point
    list_iterator_start(model->impulses);
    while(list_iterator_hasnext(model->impulses))
    {
        impulse = (HRTFImpulse *) list_iterator_next(model->impulses);        
        // get spherical distance to this point
        distance = great_circle_distance(azimuth, elevation, impulse->azimuth, impulse->elevation);
               
        if(distance<min_distance)        
        {
            valid = 1;
            
            // is it compatible with the flags?
            if(impulse->azimuth > azimuth && (flags & HRTF_FIND_AZIMUTH_LESS_THAN))
                valid = 0;
            if(impulse->azimuth <= azimuth && (flags & HRTF_FIND_AZIMUTH_GREATER_THAN))
                valid = 0;
            if(impulse->elevation > elevation && (flags & HRTF_FIND_ELEVATION_LESS_THAN))
                valid = 0;
            if(impulse->elevation <= elevation && (flags & HRTF_FIND_ELEVATION_GREATER_THAN))
                valid = 0;
         
            // find new minimum
            if(valid)
            {
                min_distance = distance;
                min_impulse = impulse;
            }                   
        }                
    }
        
    list_iterator_stop(model->impulses);    
    return min_impulse;
}

// return the length of the impulses (should be power of 2!)
int get_hrtf_buffer_size(HRTFModel *model)
{
    return model->buffer_size;
}

// set the time to interpolate between to hrtf's
void set_interpolate_time_hrtf_convolver(HRTFConvolver *convolver, float time)
{   
  convolver->interpolate_rate = 1.0/(GLOBAL_STATE.sample_rate*time);
    
}


// create a convolver based on an impulse
HRTFConvolver *create_hrtf_convolver(HRTFModel *model)
{
    HRTFConvolver *convolver;
    convolver = malloc(sizeof(*convolver));
    
    
    // convolvers
    convolver->left = create_convolver();
    convolver->right = create_convolver();
    set_empty_impulse_convolver(convolver->left, model->buffer_size);
    set_empty_impulse_convolver(convolver->right, model->buffer_size);
    
    convolver->old_left = create_convolver();
    convolver->old_right = create_convolver();
    set_empty_impulse_convolver(convolver->old_left, model->buffer_size);
    set_empty_impulse_convolver(convolver->old_right, model->buffer_size);
    
    // temporary working space
    convolver->left_v1 = create_buffer(GLOBAL_STATE.frames_per_buffer);
    convolver->right_v1 = create_buffer(GLOBAL_STATE.frames_per_buffer);
    convolver->left_v2 = create_buffer(GLOBAL_STATE.frames_per_buffer);
    convolver->right_v2 = create_buffer(GLOBAL_STATE.frames_per_buffer);
    
    
    convolver->azimuth = HUGE_VAL;
    convolver->elevation = HUGE_VAL;
    
    convolver->interpolate = 0.0;
    set_interpolate_time_hrtf_convolver(convolver, 0.01);
  
    
    // temporary working space
    convolver->temp_1 = create_complex_buffer(model->buffer_size + 1);
    convolver->temp_2 = create_complex_buffer(model->buffer_size + 1);
    
    convolver->model = model;
    set_hrtf_convolver(convolver, 0, 0);    
    set_hrtf_convolver(convolver, 10, 0);    
    
    
    return convolver;
}


// destroy an HRTF convolver
void destroy_hrtf_convolver(HRTFConvolver *hrtfconvolver)
{
    destroy_convolver(hrtfconvolver->left);
    destroy_convolver(hrtfconvolver->right);
    destroy_complex_buffer(hrtfconvolver->temp_1);
    destroy_complex_buffer(hrtfconvolver->temp_2);
    
    free(hrtfconvolver);
}

// set the convolver to an bilinearly interpolated impulse
void set_hrtf_convolver(HRTFConvolver *convolver, float azimuth, float elevation)
{
    HRTFImpulse *a, *b, *c, *d;
    ComplexBuffer *left_fft, *right_fft;
    float t1, t2;
    Convolver *temp;
    
    
    // don't change if we haven't moved by at least a few degrees
    if(fabs(convolver->azimuth-azimuth)<2 && fabs(convolver->elevation-elevation)<2)
        return;
            
   

    // get nearest hrtf
    a = get_hrtf(convolver->model, azimuth, elevation, HRTF_FIND_AZIMUTH_LESS_THAN | HRTF_FIND_ELEVATION_LESS_THAN);
    b = get_hrtf(convolver->model, azimuth, elevation, HRTF_FIND_AZIMUTH_GREATER_THAN | HRTF_FIND_ELEVATION_LESS_THAN);
    c = get_hrtf(convolver->model, azimuth, elevation, HRTF_FIND_AZIMUTH_GREATER_THAN | HRTF_FIND_ELEVATION_GREATER_THAN);
    d = get_hrtf(convolver->model, azimuth, elevation, HRTF_FIND_AZIMUTH_LESS_THAN | HRTF_FIND_ELEVATION_GREATER_THAN);
    
    // if no greater than/lesser than, just choose the nearest
    if(!a)
        a = get_hrtf(convolver->model, azimuth, elevation, 0);
    if(!b)
        b = get_hrtf(convolver->model, azimuth, elevation, 0);
    if(!c)
        c = get_hrtf(convolver->model, azimuth, elevation, 0);
    if(!d)
        d = get_hrtf(convolver->model, azimuth, elevation, 0);
        
      
    // compute interpolation factors
    if(a->azimuth == c->azimuth)
        t1 = 0.0;
    else
        t1 = fabs(azimuth - a->azimuth) / fabs(a->azimuth - c->azimuth);
        
    if(a->elevation == c->elevation)
        t2 = 0.0;
    else
        t2 = fabs(elevation - a->elevation) / fabs(a->elevation - c->elevation);
            
    
    
    
    
    // left channel impulse
    left_fft = get_fft_impulse_convolver(convolver->left);    
    interpolate_complex_buffer(convolver->temp_1, a->sound->left_fft,b->sound->left_fft,t1);
    interpolate_complex_buffer(convolver->temp_2, d->sound->left_fft,c->sound->left_fft,t1);
    interpolate_complex_buffer(left_fft, convolver->temp_1, convolver->temp_2,t2);
    
    // right channel impulse
    right_fft = get_fft_impulse_convolver(convolver->right);    
    interpolate_complex_buffer(convolver->temp_1, a->sound->right_fft,b->sound->right_fft, t1);
    interpolate_complex_buffer(convolver->temp_2, d->sound->right_fft,c->sound->right_fft, t1);
    interpolate_complex_buffer(right_fft, convolver->temp_1, convolver->temp_2, t2);                 
            
    // switch the convolvers around...
    temp = convolver->left;
    convolver->left = convolver->old_left;
    convolver->old_left = temp;
    
    temp = convolver->right;
    convolver->right = convolver->old_right;
    convolver->old_right = temp;
    convolver->interpolate = 0.0;
    
    convolver->azimuth = azimuth;
    convolver->elevation = elevation;
    
}


// pass a buffer through the convolvers
void hrtf_convolve(HRTFConvolver *convolver, Buffer *in, Buffer *left_out, Buffer *right_out)
{
    float t;
    int i;
    
    // crossfade together the old and the new...
    process_convolver(convolver->left, in, convolver->left_v1);
    process_convolver(convolver->right, in, convolver->right_v1);
    process_convolver(convolver->old_left, in, convolver->left_v2);
    process_convolver(convolver->old_right, in, convolver->right_v2);
    for(i=0;i<in->n_samples;i++)
    {
        t = convolver->interpolate;
            
        left_out->x[i] = (t) * convolver->left_v1->x[i] + (1-t) * convolver->left_v2->x[i];
        right_out->x[i] = (t) * convolver->right_v1->x[i] + (1-t) * convolver->right_v2->x[i];                       
        if(convolver->interpolate<1.0)
            convolver->interpolate += convolver->interpolate_rate;
        
    }
           
    
    
}


void test_hrtf(void)
{
    
    HRTFConvolver *convolver;
    HRTFModel *model;
    
    int i;
    float az, el;
    
    
    model = create_hrtf_model("../hrtf");

    convolver = create_hrtf_convolver(model);
    
    for(i=0;i<200;i++)
    {
        az = uniform_double()*360.0 - 180.0;
        el = uniform_double()*180.0 - 90.0;
        set_hrtf_convolver(convolver, az, el);
        
    
    }
    
    

}
