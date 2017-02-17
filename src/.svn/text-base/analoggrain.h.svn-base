/**    
    @file analoggrain.h
    @brief Classic analog waveform synthesis using the Modified FM technique for bandlimited pulse generation and
    state-variable filters for filtering.
    
    @author John Williamson
    
    Copyright (c) 2011 All rights reserved.
    Licensed under the BSD 3 clause license. See COPYING.
    
    This file is part of the OpenGrain distribution.
    http://opengrain.sourceforge.net   
*/              

#ifndef __ANALOGGRAIN_H__
#define __ANALOGGRAIN_H__

#include "audio.h"
#include "distributions.h"
#include "envelope.h"
#include "rc_generator.h"
#include "grain.h"
#include "grain_source.h"
#include "svf.h"


#define ANALOG_CARRIERS 3

// each structure always has a parameter structure
// and an active structure representing a specific instance of a grain
typedef struct AnalogGrainParameters
{
    Distribution *brightness;
    Distribution *frequency;
    Distribution *ratio;
    RCGenerator *filter_frequency;
    Distribution *q;
    Distribution *fatness;
    int integration_mode;
    int duration_mode;
    int filter_type;
} AnalogGrainParameters;

typedef struct AnalogGrain
{
    
    float brightness;
    float carrier_phase[ANALOG_CARRIERS];
    float modulator_phase[ANALOG_CARRIERS];
    
    float carrier_increment[ANALOG_CARRIERS];
    float modulator_increment[ANALOG_CARRIERS];
    
    float integrator[ANALOG_CARRIERS];
    float blocker[ANALOG_CARRIERS];
    int integration_mode;
                
    RC *filter_frequency;
    SVF *filter;     
} AnalogGrain;



AnalogGrainParameters *create_analog_parameters(GrainSource *source);
void destroy_analog_parameters(AnalogGrainParameters *analoggrain);
Distribution * get_frequency_distribution_analog_parameters(AnalogGrainParameters *analoggrain);
Distribution * get_fatness_distribution_analog_parameters(AnalogGrainParameters *analoggrain);
Distribution * get_ratio_distribution_analog_parameters(AnalogGrainParameters *analoggrain);
Distribution * get_brightness_distribution_analog_parameters(AnalogGrainParameters *analoggrain);
Distribution * get_q_distribution_analog_parameters(AnalogGrainParameters *analoggrain);
RCGenerator * get_filter_frequency_rc_analog_parameters(AnalogGrainParameters *analoggrain);


void set_duration_mode_analog_parameters(AnalogGrainParameters *analoggrain, int duration_mode);
void set_integration_mode_analog_parameters(AnalogGrainParameters *analoggrain, int mode);
void set_filter_mode_analog_parameters(AnalogGrainParameters *analoggrain, int type);



void *create_analoggrain(void *source);
void init_analoggrain(void *analoggrain, void *source, Grain *grain);
void destroy_analoggrain(void *analoggrain);
void fill_analoggrain(void *analoggrain, Buffer *buffer);



#endif