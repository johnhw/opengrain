/**    
    @file analoggrain.c
    @brief Classic analog waveform synthesis using the Modified FM technique for bandlimited pulse generation and
    state-variable filters for filtering.
    
    @author John Williamson
    
    Copyright (c) 2011 All rights reserved.
    Licensed under the BSD 3 clause license. See COPYING.
    
    This file is part of the OpenGrain distribution.
    http://opengrain.sourceforge.net   
*/              

#include "analoggrain.h"

// create a sine grain parameter structure
AnalogGrainParameters *create_analog_parameters(GrainSource *source)
{
    AnalogGrainParameters *analoggrain;
    analoggrain = malloc(sizeof(*analoggrain));
    analoggrain->frequency = create_distribution();
    analoggrain->ratio = create_distribution();
    analoggrain->brightness = create_distribution();
    analoggrain->filter_frequency = create_rc_generator();
    analoggrain->q = create_distribution();    
    analoggrain->fatness = create_distribution();
    
    analoggrain->filter_type = SVF_LOWPASS;
    
    // defaults
    set_constant_distribution(analoggrain->frequency, 440.0);
    set_constant_distribution(analoggrain->brightness, 44.0);
    set_constant_distribution(analoggrain->ratio, 1.0);
    set_constant_distribution(analoggrain->fatness, 0.02);
    set_constant_distribution(analoggrain->q, 0.4);
    
    analoggrain->integration_mode = 1;
    analoggrain->duration_mode = TIME_MODE_RELATIVE;
    
    
    return analoggrain;
}




// destroy a analoggrain parameter object
void destroy_analog_parameters(AnalogGrainParameters *analoggrain)
{
    destroy_distribution(analoggrain->frequency);
    free(analoggrain);
}


// get the frequency distribution object of a AnalogGrain parameter object
Distribution * get_frequency_distribution_analog_parameters(AnalogGrainParameters *analoggrain)
{
    return analoggrain->frequency;
}


// get the frequency distribution object of a AnalogGrain parameter object
Distribution * get_fatness_distribution_analog_parameters(AnalogGrainParameters *analoggrain)
{
    return analoggrain->fatness;
}


Distribution * get_ratio_distribution_analog_parameters(AnalogGrainParameters *analoggrain)
{
    return analoggrain->ratio;
}

Distribution * get_brightness_distribution_analog_parameters(AnalogGrainParameters *analoggrain)
{
    return analoggrain->brightness;
}

Distribution * get_q_distribution_analog_parameters(AnalogGrainParameters *analoggrain)
{
    return analoggrain->q;
}



RCGenerator * get_filter_frequency_rc_analog_parameters(AnalogGrainParameters *analoggrain)
{
    return analoggrain->filter_frequency;

}

void set_duration_mode_analog_parameters(AnalogGrainParameters *analoggrain, int duration_mode)
{
    analoggrain->duration_mode = duration_mode;
}


// set whether or not to integrate the output of the oscillators
void set_integration_mode_analog_parameters(AnalogGrainParameters *analoggrain, int mode)
{
    analoggrain->integration_mode = mode;
}


// set the filter mode (SVF_LOWPASS, SVF_HIGHPASS, SVF_BANDPASS, or SVF_NOTCH
void set_filter_mode_analog_parameters(AnalogGrainParameters *analoggrain, int type)
{
    analoggrain->filter_type = type;
    
}


// set the parameters of a Analog grain from a parent specification
void init_analoggrain(void *analoggrain, void *source, Grain *grain)
{
    AnalogGrain *active;    
    AnalogGrainParameters *parent;
    float max, n, ratio;
    int  i;
    float carrier, fatness, freq, freq_start, freq_end, duration;
    active = (AnalogGrain *) analoggrain;
    parent = (AnalogGrainParameters *)source;
    
    // split ratio into fractional and integer part for double carrier mode
    carrier = sample_from_distribution(parent->frequency);
    ratio = sample_from_distribution(parent->ratio);
    fatness = SEMITONES_TO_RATE(sample_from_distribution(parent->fatness));
            
    active->brightness =  sample_from_distribution(parent->brightness);
        
    // set the carrier and modulator phases/phase increments
    for(i=0;i<ANALOG_CARRIERS;i++)
    {
        active->carrier_phase[i] = 0.0;
        freq = carrier+carrier*(fatness-1.0)*(i-ANALOG_CARRIERS/2);
        active->carrier_increment[i] = (2*M_PI*freq) / (double)GLOBAL_STATE.sample_rate;     
        active->modulator_phase[i] = 0.0;
        active->modulator_increment[i] = (2*M_PI*freq*ratio) / (double)GLOBAL_STATE.sample_rate;     
        
        
        active->integrator[i] = 0.0;
        active->blocker[i] = 0.0;
    }
    
    active->integration_mode = parent->integration_mode;
        
    // set the filter parameters
    set_q_SVF(active->filter, sample_from_distribution(parent->q));
    set_drive_SVF(active->filter, 0);
    set_type_SVF(active->filter, parent->filter_type);
    
    // set filter frequency envelope
    freq_start = sample_from_distribution(parent->filter_frequency->start);
    freq_end = sample_from_distribution(parent->filter_frequency->end);
        
    if(parent->duration_mode==TIME_MODE_RELATIVE)    
        duration = grain->duration * sample_from_distribution(parent->filter_frequency->rate);       
    else
        duration = sample_from_distribution(parent->filter_frequency->rate);       

        set_rc_parameters(active->filter_frequency, freq_start, freq_end, duration);
            
    // limit modulation brightness so that it is bandlimited    
    n = 12 * log((((44100.0/GLOBAL_STATE.sample_rate) * carrier)/440.0)/log(2)) + 69;    
    max = exp(-0.1513*n + 15.927);
    if(active->brightness>max/2)            
        active->brightness = max/2;
}


// create a new grain from a AnalogGrain parent object
void *create_analoggrain(void *source)
{
    AnalogGrain *active;
    active = malloc(sizeof(*active));
    active->filter = create_SVF();
    active->filter_frequency = create_rc();
    return (void*)active;
}



// fill a buffer with a sinewave at a fixed frequency
void fill_analoggrain(void *analoggrain, Buffer *buffer)
{
    int i,j;
    float x, c, output, freq;
    AnalogGrain *active;
    active = (AnalogGrain *)analoggrain;
        
    for(i=0;i<buffer->n_samples;i++)
    {        
        output = 0.0;
        // sum each of the waveforms
        for(j=0;j<ANALOG_CARRIERS;j++)
        {    
            // mix together modulators
            c =  cos(active->modulator_phase[j]);
            // compute analog
            x = exp(active->brightness*c) * cos(active->carrier_phase[j]) * exp(-active->brightness);
            
            // integrate, if necessary, with a DC blocker
            if(active->integration_mode)
            {
                active->integrator[j] += x;
                active->blocker[j] = 0.9*active->blocker[j] + 0.1*active->integrator[j];
                output += active->integrator[j] - active->blocker[j];
            }
            else
                output += x;
            
            // increment phasors
            active->carrier_phase[j] += active->carrier_increment[j];    
            active->modulator_phase[j] += active->modulator_increment[j];        
            
        }
        freq = update_rc(active->filter_frequency);
        buffer->x[i] = compute_SVF(active->filter, output/ANALOG_CARRIERS);        
    }
    
    // update filter frequency
    set_freq_SVF(active->filter, freq);
}

// destroy a sine grain object
void destroy_analoggrain(void *analoggrain)
{
    free((AnalogGrain*)analoggrain);

}