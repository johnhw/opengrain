/**    
    @file dsfgrain.h
    @brief
    @author John Williamson
    
    Copyright (c) 2011 All rights reserved.
    Licensed under the BSD 3 clause license. See COPYING.
    
    This file is part of the OpenGrain distribution.
    http://opengrain.sourceforge.net   
*/              

#ifndef __DSFGRAIN_H__
#define __DSFGRAIN_H__

#include "audio.h"
#include "distributions.h"
#include "envelope.h"
#include "grain.h"
#include "grain_source.h"
#include "rc_generator.h"




#define DSF_HARMONIC_MODE 0
#define DSF_FORMANT_MODE 1

#define DSF_HARMONIC_NORMAL 0
#define DSF_HARMONIC_DOUBLE_CARRIER 1
#define DSF_HARMONIC_DOUBLE_MODULATOR 2



// each structure always has a parameter structure
// and an active structure representing a specific instance of a grain
typedef struct DSFGrainParameters
{
    void *parent; // parent source data
    
    RCGenerator *frequency;
    RCGenerator *ratio;
    RCGenerator *depth;
    
    int time_mode; // relative or absolute
    int formant_mode; // carrier is fixed, modulator is swept or modulator = ratio*carrier
    int one_sided;
    int harmonic_mode;
    
    
} DSFGrainParameters;



typedef struct DSFGrain
{
    RC *carrier;
    RC *depth;
    RC *modulator;
    
    int one_sided;
    int harmonic_mode;
    
    float modulator_phase;
    float modulator_phase_2;
    float modulator_frac;
    
    float carrier_phase;
    float carrier_phase_2;
    
    float carrier_frac;
    
    
        
} DSFGrain;



DSFGrainParameters *create_dsf_parameters(GrainSource *source);
void destroy_dsf_parameters(DSFGrainParameters *dsfgrain);
RCGenerator * get_frequency_rc_dsf_parameters(DSFGrainParameters *dsfgrain);
RCGenerator * get_ratio_rc_dsf_parameters(DSFGrainParameters *dsfgrain);
RCGenerator * get_depth_rc_dsf_parameters(DSFGrainParameters *dsfgrain);
void set_one_sided_dsf_parameters(DSFGrainParameters *dsfgrain, int one_sided);
void set_formant_mode_dsf_parameters(DSFGrainParameters *dsfgrain, int mode);
void set_time_mode_dsf_parameters(DSFGrainParameters *dsfgrain, int mode);
void set_harmonic_mode_dsf_parameters(DSFGrainParameters *dsfgrain, int mode);


void *create_dsfgrain(void *source);
void init_dsfgrain(void *dsfgrain, void *source, Grain *grain);
void destroy_dsfgrain(void *dsfgrain);
void fill_dsfgrain(void *dsfgrain, Buffer *buffer);



#endif