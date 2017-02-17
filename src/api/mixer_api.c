/**    
    @file mixer_api.c
    @brief Implements the various mixer controls for the OpenGrain system
    @author John Williamson
    
    Copyright (c) 2011 All rights reserved.
    Licensed under the BSD 3 clause license. See COPYING.
    
    This file is part of the OpenGrain distribution.
    http://opengrain.sourceforge.net   
*/             
#include "gr.h"
#include "errors.h"
#include "base_api.h"
#include "../audio.h"
#include "../grain_mixer.h"
#include "mixer_api.h"


/** Set the overall output gain instanteously.
    @arg gaindB Gain, in decibels (e.g. -20.0 = 1/10th, 6.0 ~= 2x)
*/
void grGaindBf(float gaindB)
{
    set_gain_mixer(gr_context->mixer, gaindB);
}



/** Fade the overall output gain.
    @arg gaindB Gain to fade to
    @arg time Time in seconds to reach gain (actually to be within -80dB of the gain)
*/
void grFadetodBf(float gaindB, float time)
{
    fade_gain_mixer(gr_context->mixer, gaindB, time);
}


/** Sets the various global effect parameters. Possible parameters are:
    GR_REVERB_ROOM_SIZE             Room size as a factor (1.0 = default, 0.5 = half size etc.)
    GR_REVERB_LEVEL                 Reverb level in decibels.
    GR_REVERB_PREDELAY              Predelay of reverb, in seconds.
    GR_REVERB_DIFFUSE_LEVEL         Additional contribution from diffuse field, in decibels.
    GR_REVERB_DECAY_TIME            Time in seconds to decay to ~36% of maximum level.
    GR_REVERB_DIFFUSION             Diffusion of reverb (0.0--1.0)
    GR_REVERB_MODULATION_FREQUENCY  Frequency of delay modulation as a factor (1.0 = default).
    GR_REVERB_MODULATION_DEPTH      Depth of delay modulation as a factor (1.0 = default, 0.0 = no modulation).
    GR_REVERB_DAMPING               Damping, as a frequency in Hz (gives the -3dB rolloff for the damping)
    GR_REVERB_BANDWIDTH             Bandwidth, as a frequency in Hz (gives the -3dB rolloff for the damping)
    
    GR_WIDENER_DELAY                The inter-channel delay for the stereo widener
    
    GR_COMPRESSOR_THRESHOLD         The knee/transition point for the compressor.
    GR_COMPRESSOR_RATIO             The compression ratio (e.g. 3.0 = 3:1)
    GR_COMPRESSOR_ATTACK            Attack speed of the compressor in 10dB/s units (e.g. 1.0 = 10dB/s attack rate)
    GR_COMPRESSOR_DECAY             Decay speed, in 10dB/s units.
    GR_COMPRESSOR_GAIN              Make-up gain for the compressor, in dB.
    GR_REVERB_MODE                  Selects the reverb algorithm. Can be GR_REVERB_DATTORO or GR_REVERB_RANDOM.
        
    @arg parameter The effect parameter to modify.
    @arg value The new value for the given parameter.
*/
void grEffectParameterf(int parameter, float value)
{


}

/** Sets the various integer global effect parameters. Possible parameters are:
    GR_REVERB_MODE                  Selects the reverb algorithm. Can be GR_REVERB_DATTORO or GR_REVERB_RANDOM.
    @arg parameter The effect parameter to modify.
    @arg value The new value for the given parameter.
*/
void grEffectParameteri(int parameter, int value)
{

}


/** Return a global effect parameter. See grEffectParameterf for a list of valid parameters 
    @arg parameter The effect parameter to modify.
    @return The value of the given effect parameter.
*/
float getEffectParameterf(int parameter)
{

}

/** Return am integer global effect parameter. See grEffectParameteri for a list of valid parameters 
    @arg parameter The effect parameter to modify.
    @return The value of the given effect parameter.
*/
int getEffectParameteri(int parameter)
{

}


/** Set the output parameteric EQ. Various equalizer sections can be modified. 
    Each section takes a frequency (in Hz) and a gain (in decibels).
    
    Possible EQ sections are
    GR_EQ_LOW           The lowshelf, below which all frequencies will be attenuated/amplified.
    GR_EQ_PEAK_1        The first of the three peaking sections, around which frequencies are altered.
    GR_EQ_PEAK_2        The second peaking section.
    GR_EQ_PEAK_3        The third peaking section.
    GR_EQ_HIGH          The highshelf.
    
    The default is all gains = 0dB and frequencies are 100Hz, 300Hz, 900Hz, 2500Hz and 7000Hz.
*/
void grEq2f(int section, float frequency, float gaindB)
{

}


/** Get the output parameteric EQ values. See grEq2f() for details.
    Note that the values are returned via pointers. */
void grGetEq2f(int section, float *frequency, float *gaindB)
{

}
    
    
void init_mixer_api(void)
{
    gr_context->mixer = create_mixer(2);
    gr_context->mixer_binding = malloc(sizeof(*gr_context->mixer_binding));
    
    
}    