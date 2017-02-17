/**    
    @file compressor.c
    @brief
    @author John Williamson
    
    Copyright (c) 2011 All rights reserved.
    Licensed under the BSD 3 clause license. See COPYING.
    
    This file is part of the OpenGrain distribution.
    http://opengrain.sourceforge.net   
*/              

#include "compressor.h"


// create a new compressor which initially does nothing
StereoCompressor *create_compressor(void)
{
    StereoCompressor *compressor = malloc(sizeof(*compressor));    
    compressor->power_tracker_l = create_RMS();
    compressor->power_tracker_r = create_RMS();
    compressor->compress_gain = 1.0;
    // default, do nothing compressor
    set_compressor(compressor, 0.0, 1.0, 0.1, 0.1, 0.0, 0.001);
    return compressor;
}

// free a compressor object
void destroy_compressor(StereoCompressor *compressor)
{
    destroy_RMS(compressor->power_tracker_l);
    destroy_RMS(compressor->power_tracker_r);
    free(compressor);
}


// set all the parameters of a compressor
void set_compressor(StereoCompressor *compressor, float thresholddB, float ratio, float attack, float decay, float gaindB, float speed)
{
    set_threshold_compressor(compressor, thresholddB);
    set_ratio_compressor(compressor, ratio);
    set_attack_compressor(compressor, attack);
    set_decay_compressor(compressor, decay);
    set_gain_compressor(compressor, gaindB);
    set_speed_compressor(compressor, speed);
    
}

// set the time constant of the RMS tracking behaviour
void set_speed_compressor(StereoCompressor *compressor, float speed)
{
    compressor->speed = speed;
    set_time_RMS(compressor->power_tracker_l, speed);
    set_time_RMS(compressor->power_tracker_r, speed);
}


// set the threshold of the "knee" (in dB)
void set_threshold_compressor(StereoCompressor *compressor, float thresholddB)
{
    compressor->threshold = dB_to_gain(thresholddB);   
    
}

// set the ratio of the compressor
void set_ratio_compressor(StereoCompressor *compressor, float ratio)
{
    compressor->ratio = ratio;
}

// set the attack time (in time to change by 10dB)
void set_attack_compressor(StereoCompressor *compressor, float attack)
{
    compressor->attack = attack;
    compressor->attack_coeff = exp(-1.0 / (compressor->attack*GLOBAL_STATE.sample_rate));
}

// set the decay time (in time to change by 10dB)
void set_decay_compressor(StereoCompressor *compressor, float decay)
{
   compressor->decay = decay;
   compressor->decay_coeff = exp(-1.0 / (compressor->decay*GLOBAL_STATE.sample_rate));
}

// set the compensation gain of the compressor in dB
void set_gain_compressor(StereoCompressor *compressor, float gaindB)
{
    compressor->gain = dB_to_gain(gaindB);
}

// apply the compressor to a stereo channel
void compute_compressor(StereoCompressor *compressor, float l, float r, float *out_l, float *out_r)
{
    float power, lpower, rpower;
    // compute power level
    update_RMS(compressor->power_tracker_l,l);
    update_RMS(compressor->power_tracker_r,r);
    
    lpower = compute_RMS(compressor->power_tracker_l);
    rpower = compute_RMS(compressor->power_tracker_l);
    
    
    // total stereo power
    power = sqrt(l*l+r*r);
    
    // trigger compressor
    if(power>compressor->threshold)        
        // attack
        compressor->compress_gain = compressor->attack_coeff * compressor->compress_gain + (1-compressor->attack_coeff) * (1.0/compressor->ratio);
        
    else            
        // decay
        compressor->compress_gain = compressor->decay_coeff * compressor->compress_gain + (1-compressor->decay_coeff) * (1.0);
          
    // apply output gain
    *out_l = l * compressor->gain * compressor->compress_gain;
    *out_r = r * compressor->gain * compressor->compress_gain;
    
}
