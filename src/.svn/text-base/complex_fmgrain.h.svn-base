/**    
    @file complex_fmgrain.h
    @brief
    @author John Williamson
    
    Copyright (c) 2011 All rights reserved.
    Licensed under the BSD 3 clause license. See COPYING.
    
    This file is part of the OpenGrain distribution.
    http://opengrain.sourceforge.net   
*/              

#ifndef __SINEGRAIN_H__
#define __SINEGRAIN_H__

#include "audio.h"
#include "distributions.h"
#include "envelope.h"
#include "grain.h"
#include "grain_source.h"


// each structure always has a parameter structure
// and an active structure representing a specific instance of a grain
typedef struct FMGrainParameters
{
    void *parent; // parent source data
    Distribution *frequency;
    Distribution *ratio;
    Distribution *modulation;
    /*Distribution *modulation_target;
    Distribution *modulation_rate;
    Distribution *asymmetry;
    Distribution *asymmetry_target;
    Distribution *asymmetry_rate;        */    
} FMGrainParameters;



typedef struct FMGrain
{
    float mod_frequency;
    float carrier_frequency;
    float mod_level;
    /*
    float mod_target;
    float mod_coeff;
    
    float asymmmetry;
    float asymmmetry_target;
    float asymmmetry_coeff;
    
    float carrier_phase;
    float mod_phase;
      */ 
    float mod_phase_increment;
    float carrier_phase_increment;
} FMGrain;



FMGrainParameters *create_fm_parameters(GrainSource *source);
void destroy_fm_parameters(FMGrainParameters *fmgrain);
Distribution * get_frequency_distribution_fm_parameters(FMGrainParameters *fmgrain);

void *create_fmgrain(void *source);
void init_fmgrain(void *fmgrain, void *source, Grain *grain);
void destroy_fmgrain(void *fmgrain);
void fill_fmgrain(void *fmgrain, Buffer *buffer);



#endif