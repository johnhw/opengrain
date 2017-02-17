/**    
    @file dsfgrain.c
    @brief
    Based on the algorithm given by James A. Moorer in Moorer, J. A. "The Synthesis of Complex Audio
    Spectra by Means of Discrete Summation Formulas", J. Audio Eng Sot., 1976 24:9
    @author John Williamson
    
    Copyright (c) 2011 All rights reserved.
    Licensed under the BSD 3 clause license. See COPYING.
    
    This file is part of the OpenGrain distribution.
    http://opengrain.sourceforge.net   
*/              

#include "dsfgrain.h"


#define DSF_TABLE_SIZE 1024

static float single_side_normalization[DSF_TABLE_SIZE];
static float double_side_normalization[DSF_TABLE_SIZE];
static int tables_created = 0;


static void create_dsf_tables(void)
{
    int i;
    float a;
    for(i=0;i<DSF_TABLE_SIZE;i++)
    {
        a = i / (float)DSF_TABLE_SIZE;
        single_side_normalization[i] = sqrt(1-a*a);
        double_side_normalization[i] = (sqrt((1-a*a)/(1+a*a)));        
    }    
    tables_created = 1;
}

// create a sine grain parameter structure
DSFGrainParameters *create_dsf_parameters(GrainSource *source)
{

    // make sure normalization tables have been created
    if(!tables_created)
        create_dsf_tables();
        
    DSFGrainParameters *dsfgrain;
    dsfgrain = malloc(sizeof(*dsfgrain));
    dsfgrain->frequency = create_rc_generator();
    dsfgrain->ratio = create_rc_generator();
    dsfgrain->depth = create_rc_generator();
    
    dsfgrain->one_sided = 0;
    dsfgrain->formant_mode = 1;
    dsfgrain->harmonic_mode = DSF_HARMONIC_DOUBLE_MODULATOR;
    dsfgrain->time_mode = TIME_MODE_RELATIVE;
    
    // defaults
    set_constant_distribution(dsfgrain->frequency->start, 140.0);
    set_constant_distribution(dsfgrain->frequency->end, 640.0);
    set_constant_distribution(dsfgrain->frequency->rate, 1.0);
    
    set_constant_distribution(dsfgrain->ratio->start, 140.0);
    set_constant_distribution(dsfgrain->ratio->end, 140.0);
    set_constant_distribution(dsfgrain->ratio->rate, 1.0);
    
    set_constant_distribution(dsfgrain->depth->start, 0.5);
    set_constant_distribution(dsfgrain->depth->end, 0.1);
    set_constant_distribution(dsfgrain->depth->rate, 1.0);
    
    
    
    return dsfgrain;
}


// destroy a dsfgrain parameter object
void destroy_dsf_parameters(DSFGrainParameters *dsfgrain)
{
    destroy_rc_generator(dsfgrain->frequency);
    destroy_rc_generator(dsfgrain->depth);
    destroy_rc_generator(dsfgrain->ratio);
    free(dsfgrain);
}


// set the double carrier/modulator mode, if needed
void set_harmonic_mode_dsf_parameters(DSFGrainParameters *dsfgrain, int mode)
{
    dsfgrain->harmonic_mode = mode;
}

// set whether grain duration relative or absolute time is used
void set_time_mode_dsf_parameters(DSFGrainParameters *dsfgrain, int mode)
{
    dsfgrain->time_mode = mode;
}

// set the formant mode of the DSF grain
// i.e. whether ratio is modulation ratio, or whether it is a fixed carrier frequency
void set_formant_mode_dsf_parameters(DSFGrainParameters *dsfgrain, int mode)
{
    dsfgrain->formant_mode = mode;
}

// set whether spectra are one or two sided
void set_one_sided_dsf_parameters(DSFGrainParameters *dsfgrain, int one_sided)
{
    dsfgrain->one_sided = one_sided;
}

// get the frequency distribution object of a DSFGrain parameter object
RCGenerator * get_frequency_rc_dsf_parameters(DSFGrainParameters *dsfgrain)
{
    return dsfgrain->frequency;
}

// get the ratio distribution of an fm grain
RCGenerator * get_ratio_rc_dsf_parameters(DSFGrainParameters *dsfgrain)
{
    return dsfgrain->ratio;
}

// get the depth depth distribution 
RCGenerator * get_depth_rc_dsf_parameters(DSFGrainParameters *dsfgrain)
{
    return dsfgrain->depth;
}


// initialise a DSF grain object
void init_dsfgrain(void *dsfgrain, void *source, Grain *grain)
{
    DSFGrain *active;
    DSFGrainParameters *parent;
    active = (DSFGrain *) dsfgrain;
    parent = (DSFGrainParameters *)source;
    
    float carrier_start, carrier_end, carrier_rate;
    float modulator_start, modulator_end, modulator_rate;
    float depth_start, depth_end, depth_rate;
    
    
    // set the carrier and modulator frequencies
    if(parent->formant_mode)
    {
        carrier_start = sample_from_distribution(parent->ratio->start);
        carrier_end = sample_from_distribution(parent->ratio->end);
                
        
        modulator_start = sample_from_distribution(parent->frequency->start);
        modulator_end = sample_from_distribution(parent->frequency->end);
        
            carrier_rate = sample_from_distribution(parent->ratio->rate);
            modulator_rate = sample_from_distribution(parent->frequency->rate);                
        
    }
    else
    {
        carrier_start = sample_from_distribution(parent->frequency->start);
        carrier_end = sample_from_distribution(parent->frequency->end);
        carrier_rate = sample_from_distribution(parent->frequency->rate);
        
        modulator_start = sample_from_distribution(parent->frequency->start) * sample_from_distribution(parent->ratio->start);
        modulator_end = sample_from_distribution(parent->frequency->end) * sample_from_distribution(parent->ratio->end);
        modulator_rate = sample_from_distribution(parent->frequency->rate) * sample_from_distribution(parent->ratio->rate);                
                
            carrier_rate = sample_from_distribution(parent->frequency->rate);
            modulator_rate = sample_from_distribution(parent->ratio->rate);                
        
    }
    
    depth_start = sample_from_distribution(parent->depth->start);
    depth_end = sample_from_distribution(parent->depth->end);
    depth_rate = sample_from_distribution(parent->depth->rate);
    
    if(parent->time_mode==TIME_MODE_RELATIVE)
    {
        depth_rate *= grain->duration;
        modulator_rate *= grain->duration;
        carrier_rate *= grain->duration;
    }
    
    set_rc_parameters(active->carrier, carrier_start, carrier_end, carrier_rate);
    set_rc_parameters(active->modulator, modulator_start, modulator_end, modulator_rate);                
    set_rc_parameters(active->depth, depth_start, depth_end, depth_rate);
    
    active->one_sided = parent->one_sided;
    active->modulator_phase = 0.0;
    active->carrier_phase = 0.0;   
    active->modulator_phase_2 = 0.0;
    active->carrier_phase_2 = 0.0;   
    
    active->harmonic_mode = parent->harmonic_mode;
}


// create a new grain from a DSFGrain parent object
void *create_dsfgrain(void *source)
{
    DSFGrain *active;
    active = malloc(sizeof(*active));
    active->carrier = create_rc();
    active->modulator = create_rc();
    active->depth = create_rc();
    
    return (void*)active;
}



// fill a buffer with a DSF wave at a fixed frequency
void fill_dsfgrain(void *dsfgrain, Buffer *buffer)
{
    int i,j;
    float v;
    DSFGrain *active;
    float frequency, modulator, depth;
    
    active = (DSFGrain *)dsfgrain;
    
    
    for(i=0;i<buffer->n_samples;i++)
        {        
            frequency = update_rc(active->carrier);
            modulator = update_rc(active->modulator);
            depth = update_rc(active->depth);
            
            j = depth * DSF_TABLE_SIZE;
            
            // Discrete summation formula
            if(active->one_sided)        
            {
                v = (sin(active->carrier_phase) - depth * sin(active->carrier_phase - active->modulator_phase)) / (1*depth*depth-2*depth*cos(active->modulator_phase)); 
            
                buffer->x[i] = single_side_normalization[j] * v;
            }
            else
            {
                v = ((1-depth*depth)*sin(active->carrier_phase)) / (1+depth*depth-2*depth*cos(active->modulator_phase)); 
                
                buffer->x[i] = double_side_normalization[j] * v;
            }
                                    
            active->carrier_phase += 2*M_PI*frequency / GLOBAL_STATE.sample_rate;    
            active->modulator_phase += 2*M_PI*modulator / GLOBAL_STATE.sample_rate;                
        }    
}

// destroy a sine grain object
void destroy_dsfgrain(void *dsfgrain)
{
    free((DSFGrain*)dsfgrain);

}