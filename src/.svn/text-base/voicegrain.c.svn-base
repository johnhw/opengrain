/**    
    @file voicegrain.c
    @brief
    @author John Williamson
    
    Copyright (c) 2011 All rights reserved.
    Licensed under the BSD 3 clause license. See COPYING.
    
    This file is part of the OpenGrain distribution.
    http://opengrain.sourceforge.net   
*/              

#include "voicegrain.h"


// freq, dB, bW (hz)
float vowel_a[] = {
650, 0, 60,
1080, -6, 90,
2650, -7, 120,
2900, -8, 130,
3250, -22, 140
};

float vowel_e[] = {
400, 0, 70,
1700, -14, 80,
2600, -12, 100,
3200, -14, 120,
3580, -20, 120
};

float vowel_i[] = {
290, 0, 40,
1870, -15, 90,
2800, -18, 100,
3250, -20, 120,
3540, -30, 120
};


float vowel_o[] = {
400, 0, 40,
800, -10, 80,
2600, -12, 100,
2800, -12, 120,
3000, -26, 120
};


float vowel_u[] = {
350, 0, 40,
600, -20, 60,
2700, -17, 100,
3900, -14, 120,
3300, -26, 120
};


float *vowels[] = {&vowel_a[0], &vowel_e[0], &vowel_i[0], &vowel_o[0], &vowel_u[0]};

// create a sine grain parameter structure
VoiceGrainParameters *create_voice_parameters(GrainSource *source)
{
    VoiceGrainParameters *voicegrain;
    voicegrain = malloc(sizeof(*voicegrain));
    voicegrain->frequency = create_distribution();
    voicegrain->noise = create_distribution();
    voicegrain->vibrato = create_distribution();
    voicegrain->vibrato_frequency = create_distribution();
    voicegrain->vowel = create_distribution();
        
    // defaults
    set_constant_distribution(voicegrain->frequency, 80.0);
    set_constant_distribution(voicegrain->noise, 0.0);
    set_constant_distribution(voicegrain->vibrato, 0.2);
    set_constant_distribution(voicegrain->vibrato_frequency, 6.5);
    set_constant_distribution(voicegrain->vowel, 2.0);
    
    return voicegrain;
}



// destroy a voicegrain parameter object
void destroy_voice_parameters(VoiceGrainParameters *voicegrain)
{
    destroy_distribution(voicegrain->frequency);
    free(voicegrain);
}

// get the frequency distribution object of a VoiceGrain parameter object
Distribution * get_frequency_distribution_voice_parameters(VoiceGrainParameters *voicegrain)
{
    return voicegrain->frequency;
}

Distribution * get_noise_distribution_voice_parameters(VoiceGrainParameters *voicegrain)
{
    return voicegrain->noise;
}


Distribution * get_vibrato_distribution_voice_parameters(VoiceGrainParameters *voicegrain)
{
    return voicegrain->vibrato;
}

Distribution * get_vibrato_frequency_distribution_voice_parameters(VoiceGrainParameters *voicegrain)
{
    return voicegrain->vibrato;
}


Distribution * get_vowel_distribution_voice_parameters(VoiceGrainParameters *voicegrain)
{
    return voicegrain->vowel;
}


// set the formant filters for a grain given a vowel index
void set_formants(VoiceGrain *active, float vowel)
{
    float vowel_t;
    int vowel_index;
    float formants[15];
    float freq, bw, gain;
    int i;
    
    if(vowel<0)
        vowel = 0;
    if(vowel>4)
        vowel = 4;
    vowel_index = floor(vowel);
    vowel_t = vowel - vowel_index;
    
    // interpolate formant values
    if(vowel_index<=3)
    {
        for(i=0;i<15;i++)        
            formants[i] = (1-vowel_t)*vowels[vowel_index][i] + (vowel_t)*vowels[vowel_index+1][i];            
    }
    else
    {
        for(i=0;i<15;i++)        
            formants[i] = vowels[vowel_index][i];            
    }
    
    // set vowel filters
    for(i=0;i<5;i++)
    {
        freq = formants[i*3];
        bw = formants[i*3+2];
        gain = dB_to_gain(formants[i*3+1]);       
        
        // set the filter and gain        
        reset_biquad(active->f[i]);
        biquad_bandpass_bw(active->f[i], freq, bw/2);        
        active->f_gain[i] = gain;               
    }   
    biquad_lowpass(active->lowpass, active->frequency*4, 1.0);
    reset_biquad(active->lowpass);
    
}


void init_voicegrain(void *voicegrain, void *source, Grain *grain)
{
    VoiceGrain *active;
    float vowel,vibrato_depth;
    float n, max;
    VoiceGrainParameters *parent;
    active = (VoiceGrain *) voicegrain;
    parent = (VoiceGrainParameters *)source;    
    active->frequency = sample_from_distribution(parent->frequency);
    active->vibrato_frequency = sample_from_distribution(parent->vibrato_frequency);
    
    // compute vibrato depth in semitones
    vibrato_depth = sample_from_distribution(parent->vibrato);
    active->vibrato = SEMITONES_TO_RATE(vibrato_depth) * active->frequency - active->frequency;
    
    
    // compute bandlimiting
    n = 12 * log((((44100.0/GLOBAL_STATE.sample_rate) * (active->frequency))/440.0)/log(2)) + 69;    
    
    max = exp(-0.1513*n + 15.927);    
    if(max>400.0)
        max = 400.0;
    active->depth = max;    
    active->noise_level = sample_from_distribution(parent->noise);                    
    active->phase = 0.0;        
    // random vibrato phase
    active->vibrato_phase = uniform_double() * 2 * M_PI;
    
    // vowel selection!
    vowel = sample_from_distribution(parent->vowel);    
    set_formants(active, vowel);
    
}


// create a new grain from a VoiceGrain parent object
void *create_voicegrain(void *source)
{
    VoiceGrain *active;
    active = malloc(sizeof(*active));
    active->f[0] = create_biquad();
    active->f[1] = create_biquad();
    active->f[2] = create_biquad();
    active->f[3] = create_biquad();
    active->f[4] = create_biquad();
    
    active->lowpass = create_biquad();
    
  
    
    return (void*)active;
}



// fill a buffer with a vowel filtered pulse train
void fill_voicegrain(void *voicegrain, Buffer *buffer)
{
    int i;
    float vibrato, voiced, unvoiced, input, output;
    VoiceGrain *active;
    active = (VoiceGrain *)voicegrain;
    
    for(i=0;i<buffer->n_samples;i++)
    {        
        voiced = exp(-cos(active->phase)*active->depth) * cos(active->phase) * exp(-active->depth);
        unvoiced = (uniform_double()-0.5)*2;
        
        input = (1-active->noise_level) * voiced + active->noise_level*unvoiced;
        input = process_biquad(active->lowpass, input);
        
        
        output = 0.0;   
        output += active->f_gain[0] * process_biquad(active->f[0], input);
        output += active->f_gain[1] * process_biquad(active->f[1], input);
        output += active->f_gain[2] * process_biquad(active->f[2], input);
        output += active->f_gain[3] * process_biquad(active->f[3], input);
        output += active->f_gain[4] * process_biquad(active->f[4], input);
        buffer->x[i] = output;
        
        vibrato = sin(active->vibrato_phase) * active->vibrato;        
        active->vibrato_phase += (2*M_PI*(active->vibrato_frequency))/GLOBAL_STATE.sample_rate;    
        active->phase += (2*M_PI*(active->frequency+vibrato))/GLOBAL_STATE.sample_rate;    
                
        
    }

}

// destroy a sine grain object
void destroy_voicegrain(void *voicegrain)
{
    free((VoiceGrain*)voicegrain);

}