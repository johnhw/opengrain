/**    
    @file svf.c
    @brief
    @author John Williamson
    
    Copyright (c) 2011 All rights reserved.
    Licensed under the BSD 3 clause license. See COPYING.
    
    This file is part of the OpenGrain distribution.
    http://opengrain.sourceforge.net   
*/              

#include <math.h>
#include "svf.h"


// Set the SVF parameters
void set_SVF(SVF *svf, double freq, double q, double drive, int type)
{
    
    set_type_SVF(svf,type);
    set_freq_SVF(svf,freq);
    set_q_SVF(svf,q);
    set_drive_SVF(svf,drive);    
}


void set_freq_SVF(SVF *svf, double freq)
{
    svf->f = 2*sin(M_PI*MIN(0.25,(freq*2)/GLOBAL_STATE.sample_rate));
    svf->damp = MIN(2.0*(1.0-pow(svf->res, 0.25)), MIN(2.0, 2.0/svf->f-svf->f*0.5));
}


void set_q_SVF(SVF *svf, double q)
{
   svf->res = q;
   svf->damp = MIN(2.0*(1.0-pow(svf->res, 0.25)), MIN(2.0, 2.0/svf->f-svf->f*0.5));

}
void set_drive_SVF(SVF *svf, double drive)
{
   svf->drive = drive/21.0;
}

void set_type_SVF(SVF *svf, int type)
{
    svf->type = type;
}





// Create a new SVF filter
SVF *create_SVF()
{
    SVF *svf = malloc(sizeof(*svf));
    return svf;
}

// Destroy an SVF filter
void destroy_SVF(SVF *svf)
{
    free(svf);
}


// Process a buffer of samples with the state variable filter algorithm
void process_SVF(SVF *svf, Buffer *in, Buffer *out)
{
    int i;
    float input, output;
    double *ptr;
    
    switch(svf->type)
    {
        case SVF_LOWPASS:
            ptr = &(svf->low);
            break;
        case SVF_HIGHPASS:
            ptr = &(svf->high);
            break;
        case SVF_BANDPASS:
            ptr = &(svf->band);
            break;
        case SVF_NOTCH:    
            ptr = &(svf->notch);
            break;    
        default:
            ptr = &(svf->low);
            break;            
    }
            
        
    
    for(i=0;i<in->n_samples;i++)
    {
        input = in->x[i];
        
        // First pass
        svf->notch = input - svf->damp * svf->band;
        svf->low = svf->low + svf->f * svf->band;
        svf->high = svf->notch - svf->low;
        svf->band = svf->f*svf->high + svf->band - svf->drive*svf->band*svf->band;
        output = 0.5 * (*ptr);
        
        // Second pass
        svf->notch = input - svf->damp * svf->band;
        svf->low = svf->low + svf->f * svf->band;
        svf->high = svf->notch - svf->low;
        svf->band = svf->f*svf->high + svf->band - svf->drive*svf->band*svf->band;
        output += 0.5 * (*ptr);
        out->x[i] = output;
    }
}


float compute_SVF(SVF *svf, float input)
{
    float output;
    double *ptr;
    
    switch(svf->type)
    {
        case SVF_LOWPASS:
            ptr = &(svf->low);
            break;
        case SVF_HIGHPASS:
            ptr = &(svf->high);
            break;
        case SVF_BANDPASS:
            ptr = &(svf->band);
            break;
        case SVF_NOTCH:    
            ptr = &(svf->notch);
            break;    
        default:
            ptr = &(svf->low);
            break;            
    }
            
            
        // First pass
        svf->notch = input - svf->damp * svf->band;
        svf->low = svf->low + svf->f * svf->band;
        svf->high = svf->notch - svf->low;
        svf->band = svf->f*svf->high + svf->band - svf->drive*svf->band*svf->band;
        output = 0.5 * (*ptr);
        
        // Second pass
        svf->notch = input - svf->damp * svf->band;
        svf->low = svf->low + svf->f * svf->band;
        svf->high = svf->notch - svf->low;
        svf->band = svf->f*svf->high + svf->band - svf->drive*svf->band*svf->band;
        output += 0.5 * (*ptr);
    return output;
}


SVFDouble *create_SVFDouble()
{
    SVFDouble *sdouble = malloc(sizeof(*sdouble));
    sdouble->svf1 = create_SVF();
    sdouble->svf2 = create_SVF();
    return sdouble;
}


void destroy_SVFDouble(SVFDouble *svf)
{
    destroy_SVF(svf->svf1);
    destroy_SVF(svf->svf2);
    free(svf);
}



// Set a pair of SVF's to identical parameters
void set_SVFDouble(SVFDouble *svf, double freq, double q, double drive, int type)
{
    set_SVF(svf->svf1, freq, q, drive, type);
    set_SVF(svf->svf2, freq, q, drive, type);
}

// Process a buffer of samples with a pair of SVF's
void process_SVFDouble(SVFDouble *svf, Buffer *in, Buffer *out)
{    
    process_SVF(svf->svf1, in, out);    
    if(svf->doubled)
        process_SVF(svf->svf2, out, out);    
}


void set_freq_SVFDouble(SVFDouble *svf, double freq)
{
    set_freq_SVF(svf->svf1, freq);
    set_freq_SVF(svf->svf2, freq);
}

void set_q_SVFDouble(SVFDouble *svf, double q)
{
    set_q_SVF(svf->svf1, q);
    set_q_SVF(svf->svf2, q);
}

void set_drive_SVFDouble(SVFDouble *svf, double drive)
{
    set_drive_SVF(svf->svf1, drive);
    set_drive_SVF(svf->svf2, drive);
}


void set_type_SVFDouble(SVFDouble *svf, int type)
{
   set_type_SVF(svf->svf1, type);
   set_type_SVF(svf->svf2, type);
}


float compute_SVFDouble(SVFDouble *svf, float sample)
{
    float output;
    output = compute_SVF(svf->svf1, sample);
    output = compute_SVF(svf->svf1, output);
    return output;
  
   
}

// create a multichannel filter object with the given number of channels
MultichannelSVF *create_multichannel_SVF(int channels)
{
    int i;
    MultichannelSVF *svf = malloc(sizeof(*svf));
    svf->channels = channels;
    svf->filters = malloc(sizeof(*svf->filters)*channels);
    for(i=0;i<channels;i++)
        svf->filters[i] = create_SVF();
    return svf;
}

// free a multichannel filter object
void destroy_multichannel_SVF(MultichannelSVF *svf)
{
    int i;
    for(i=0;i<svf->channels;i++)
        free(svf->filters[i]);
    free(svf->filters);
    free(svf);        
}

// set the frequency of all channels
void set_freq_multichannel_SVF(MultichannelSVF *svf, float freq)
{
    int i;
   for(i=0;i<svf->channels;i++)
        set_freq_SVF(svf->filters[i], freq); 
}

// set q of all channels
void set_q_multichannel_SVF(MultichannelSVF *svf, float q)
{
    int i;
    for(i=0;i<svf->channels;i++)
        set_q_SVF(svf->filters[i], q); 
}

// set the filter type for all channels
void set_type_multichannel_SVF(MultichannelSVF *svf, int type)
{
    int i;
    for(i=0;i<svf->channels;i++)
        set_type_SVF(svf->filters[i], type); 
}


// set the drive for all channels
void set_drive_multichannel_SVF(MultichannelSVF *svf, float drive)
{    
    int i;
    for(i=0;i<svf->channels;i++)
        set_drive_SVF(svf->filters[i], drive); 
}


// filter an entire set of multichannel buffers
void process_multichannel_SVF(MultichannelSVF *svf, Buffer **ins, Buffer **outs)
{
    int i,j;
    for(j=0;i<svf->channels;i++)   
        for(i=0;i<ins[j]->n_samples;i++)        
            outs[j]->x[i] = compute_SVF(svf->filters[j], ins[j]->x[i]);
        
}
