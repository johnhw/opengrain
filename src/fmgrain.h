/**    
    @file fmgrain.h
    @brief
    @author John Williamson
    
    Copyright (c) 2011 All rights reserved.
    Licensed under the BSD 3 clause license. See COPYING.
    
    This file is part of the OpenGrain distribution.
    http://opengrain.sourceforge.net   
*/              

#ifndef __FMGRAIN_H__
#define __FMGRAIN_H__

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
    
} FMGrainParameters;



typedef struct FMGrain
{
    float mod_phase, carrier_phase;
    float modulation;
    float mod_phase_increment;
    float carrier_phase_increment;
} FMGrain;



FMGrainParameters *create_fm_parameters(GrainSource *source);
void destroy_fm_parameters(FMGrainParameters *fmgrain);
Distribution * get_frequency_distribution_fm_parameters(FMGrainParameters *fmgrain);
Distribution * get_ratio_distribution_fm_parameters(FMGrainParameters *fmgrain);
Distribution * get_modulation_distribution_fm_parameters(FMGrainParameters *fmgrain);

void *create_fmgrain(void *source);
void init_fmgrain(void *fmgrain, void *source, Grain *grain);
void destroy_fmgrain(void *fmgrain);
void fill_fmgrain(void *fmgrain, Buffer *buffer);



#endif