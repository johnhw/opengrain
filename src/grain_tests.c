/**    
    @file grain_tests.c
    @brief
    @author John Williamson
    
    Copyright (c) 2011 All rights reserved.
    Licensed under the BSD 3 clause license. See COPYING.
    
    This file is part of the OpenGrain distribution.
    http://opengrain.sourceforge.net   
*/              

#include "grain_tests.h"
#include "sinegrain.h"
#include "noisegrain.h"
#include "distributions.h"
#include "wavereader.h"
#include "wavegrain.h"
#include "impulsegrain.h"
#include "padsyngrain.h"
#include "grain_model.h"
#include "glissgrain.h"
#include "multisinegrain.h"
#include "pluckgrain.h"
#include "fmgrain.h"
#include "analoggrain.h"
#include "voicegrain.h"
#include "dsfgrain.h"
#include "stream_fx.h"

void test_hrtf_mode(GrainStream *stream);

void test_default_grain_generation(GrainStream *stream)
{

    
    Spatializer *spatializer;
    GrainModel *model;
   
    
    model = get_grain_model_stream(stream);

        
    set_constant_distribution(model->amplitude, -5.0);
    
    
    set_single_component_distribution(model->azimuth, DISTRIBUTION_TYPE_UNIFORM, 0.0, 0.0, DISTRIBUTION_POLARITY_UNCHANGED, 0);
    
    set_constant_distribution(model->elevation, 0.0);
    //set_constant_distribution(model->distance, 1.0);
    set_single_component_distribution(model->distance, DISTRIBUTION_TYPE_UNIFORM, 1.0, 0.0, DISTRIBUTION_POLARITY_POSITIVE, 0);
    
    set_rate_mode_model(model, RATE_MODE_STOCHASTIC);
    
    set_constant_distribution(model->rate, 10.0);
    set_constant_distribution(model->time, 0.0);
    set_constant_distribution(model->source, 0.0);
    set_constant_distribution(model->duration, 0.2);
    set_constant_distribution(model->attack, 0.05);
    set_constant_distribution(model->decay, 0.9);
    set_constant_distribution(model->shape, 0.1);
    model->envelope_type = ENVELOPE_TYPE_EXP;
    
    spatializer = get_spatializer_stream(stream);
    //set_spatializer_mode(spatializer, SPATIALIZATION_3D_FILTERING, SPATIALIZATION_PER_STREAM);
    test_hrtf_mode(stream);
    
    
}


void test_hrtf_mode(GrainStream *stream)
{
    GrainModel *model;
    Spatializer *spatializer;
    HRTFModel *hrtf;    
    
    model = get_grain_model_stream(stream);
    
    
    hrtf = create_hrtf_model("../hrtf");
    spatializer = get_spatializer_stream(stream);
    
    set_hrtf_spatializer(spatializer, hrtf);
    set_spatializer_mode(spatializer, SPATIALIZATION_3D_HRTF, SPATIALIZATION_PER_STREAM);

    
}

void test_cross_delay(GrainStream *stream)
{
     CrossDelay *delay;
    delay = get_delay_stream_fx(get_stream_fx_stream(stream));
    set_length_cross_delay(delay, 0.5);
    set_feedback_cross_delay(delay, -5.0, -100.0);
    set_filter_cross_delay(delay, 400.0, 2.0, BIQUAD_LOWPASS);
    enable_delay_stream_fx(get_stream_fx_stream(stream));

}

void test_default_mixer_settings(GrainMixer *mixer)
{
    enable_reverb_mixer(mixer);
    set_modulation_random_reverb(mixer->random_reverb, 1);
    set_reverb_level_mixer(mixer, -20.0);
    
    //enable_compressor_mixer(info->mixer);
    //set_compressor(get_compressor_mixer(info->mixer), -15.0, 25.0, 0.02, 0.5, 10.0, 1e-3); 
}


    
    
    
    
    
void test_voicegrain(GrainStream *stream)
{
    Distribution *frequency;
    VoiceGrainParameters *voice_parameters;
    GrainSource *source;    
    source = create_grain_source();
    add_source_stream(stream, source);    
    voice_parameters = create_voice_parameters(source);
    frequency = get_frequency_distribution_voice_parameters(voice_parameters);                
    
    set_single_component_distribution(frequency, DISTRIBUTION_TYPE_UNIFORM, 100.0, 0.0, DISTRIBUTION_POLARITY_POSITIVE, 0);    
    set_single_component_distribution(get_vowel_distribution_voice_parameters(voice_parameters), DISTRIBUTION_TYPE_UNIFORM, 0.0, 5.0, DISTRIBUTION_POLARITY_POSITIVE, 0);        
    set_single_component_distribution(get_noise_distribution_voice_parameters(voice_parameters), DISTRIBUTION_TYPE_UNIFORM, 0.0, 0.1, DISTRIBUTION_POLARITY_POSITIVE, 0);    
    
    
    
    set_grain_source(source, create_voicegrain, init_voicegrain, destroy_voicegrain, fill_voicegrain, voice_parameters);
}
  
    
    
void test_sinegrain(GrainStream *stream)
{
    Distribution *frequency;
    SineGrainParameters *sine_parameters;
    GrainSource *source;    
    source = create_grain_source();
    add_source_stream(stream, source);    
    sine_parameters = create_sine_parameters(source);
    frequency = get_frequency_distribution_sine_parameters(sine_parameters);                
    set_single_component_distribution(frequency, DISTRIBUTION_TYPE_UNIFORM, 40.0, 1000.0, DISTRIBUTION_POLARITY_POSITIVE, 0);    
    set_grain_source(source, create_sinegrain, init_sinegrain, destroy_sinegrain, fill_sinegrain, sine_parameters);
}

void test_noisegrain(GrainStream *stream)
{
    Distribution *frequency, *q;
    NoiseGrainParameters *noise_parameters;
    GrainSource *source;    
    source = create_grain_source();
    add_source_stream(stream, source);    
     noise_parameters = create_noise_parameters(source);
    frequency = get_frequency_distribution_noise_parameters(noise_parameters);                
    q = get_q_distribution_noise_parameters(noise_parameters);                
    set_single_component_distribution(frequency, DISTRIBUTION_TYPE_UNIFORM, 40.0, 2000.0, DISTRIBUTION_POLARITY_POSITIVE, 0);        
    set_constant_distribution(q, 15.0);
    set_grain_source(source, create_noisegrain, init_noisegrain, destroy_noisegrain, fill_noisegrain, noise_parameters);    
}

void test_impulsegrain(GrainStream *stream)
{
    ImpulseGrainParameters *impulse_parameters;
    
    GrainSource *source;    
    source = create_grain_source();
    add_source_stream(stream, source);    
    impulse_parameters = create_impulse_parameters(source);
    
    add_sound_bank_impulse_parameters(impulse_parameters, create_soundbank("..\\samples\\materials\\ping"));        
    set_grain_source(source, create_impulsegrain, init_impulsegrain, destroy_impulsegrain, fill_impulsegrain, impulse_parameters);
}

void test_fmgrain(GrainStream *stream)
{
    FMGrainParameters *fm_parameters;
    GrainSource *source;    
    source = create_grain_source();
    add_source_stream(stream, source);        
    fm_parameters = create_fm_parameters(source);    
    set_single_component_distribution(get_frequency_distribution_fm_parameters(fm_parameters), DISTRIBUTION_TYPE_GAUSSIAN, 100.0, 100.0, DISTRIBUTION_POLARITY_UNCHANGED, 0);    
    set_single_component_distribution(get_modulation_distribution_fm_parameters(fm_parameters), DISTRIBUTION_TYPE_UNIFORM, 0.1, 1.0, DISTRIBUTION_POLARITY_POSITIVE, 0);    
    set_single_component_distribution(get_ratio_distribution_fm_parameters(fm_parameters), DISTRIBUTION_TYPE_UNIFORM, 3.0, 0.05, DISTRIBUTION_POLARITY_POSITIVE, 0);    
    set_grain_source(source, create_fmgrain, init_fmgrain, destroy_fmgrain, fill_fmgrain, fm_parameters);
}


void test_analoggrain(GrainStream *stream)
{
    AnalogGrainParameters *analog_parameters;
    GrainSource *source;    
    source = create_grain_source();
    add_source_stream(stream, source);        
    analog_parameters = create_analog_parameters(source);    
    set_single_component_distribution(get_frequency_distribution_analog_parameters(analog_parameters), DISTRIBUTION_TYPE_GAUSSIAN, 500.0, 10.0, DISTRIBUTION_POLARITY_POSITIVE, 0);    
    set_single_component_distribution(get_brightness_distribution_analog_parameters(analog_parameters), DISTRIBUTION_TYPE_UNIFORM, 0.0, 500.0, DISTRIBUTION_POLARITY_POSITIVE, 0);    
    set_single_component_distribution(get_ratio_distribution_analog_parameters(analog_parameters), DISTRIBUTION_TYPE_UNIFORM, 1, 0.01, DISTRIBUTION_POLARITY_POSITIVE, 0);    
    set_single_component_distribution(get_fatness_distribution_analog_parameters(analog_parameters), DISTRIBUTION_TYPE_UNIFORM, 0.3, 0.0, DISTRIBUTION_POLARITY_POSITIVE, 0);    
    set_single_component_distribution(get_filter_frequency_rc_analog_parameters(analog_parameters)->start, DISTRIBUTION_TYPE_UNIFORM, 2000, 1000.0, DISTRIBUTION_POLARITY_POSITIVE, 0);    
    set_single_component_distribution(get_filter_frequency_rc_analog_parameters(analog_parameters)->end, DISTRIBUTION_TYPE_UNIFORM, 500, 0.0, DISTRIBUTION_POLARITY_POSITIVE, 0); 
    set_single_component_distribution(get_filter_frequency_rc_analog_parameters(analog_parameters)->rate, DISTRIBUTION_TYPE_UNIFORM, 1.0, 0.0, DISTRIBUTION_POLARITY_POSITIVE, 0);       
    
    
    
    set_grain_source(source, create_analoggrain, init_analoggrain, destroy_analoggrain, fill_analoggrain, analog_parameters);
}


void test_dsfgrain(GrainStream *stream)
{
    DSFGrainParameters *dsf_parameters;
    GrainSource *source;    
    
    source = create_grain_source();
    add_source_stream(stream, source);  
    dsf_parameters = create_dsf_parameters(source);
    
    //set_single_component_distribution(get_frequency_distribution_fm_parameters(fm_parameters), DISTRIBUTION_TYPE_GAUSSIAN, 4000.0, 100.0, DISTRIBUTION_POLARITY_UNCHANGED, 0);    
    //set_single_component_distribution(get_modulation_distribution_fm_parameters(fm_parameters), DISTRIBUTION_TYPE_UNIFORM, 0.1, 1.0, DISTRIBUTION_POLARITY_POSITIVE, 0);    
    //set_single_component_distribution(get_ratio_distribution_fm_parameters(fm_parameters), DISTRIBUTION_TYPE_UNIFORM, 4.0, 0.05, DISTRIBUTION_POLARITY_POSITIVE, 0);    
    set_grain_source(source, create_dsfgrain, init_dsfgrain, destroy_dsfgrain, fill_dsfgrain, dsf_parameters);    
}


void test_wavegrain(GrainStream *stream)
{
    Distribution *phase, *pitch;
    WaveGrainParameters *wave_parameters;
    WaveSound *wave_sound;
    GrainSource *source;    
    source = create_grain_source();
    add_source_stream(stream, source);  
    
    wave_parameters = create_wave_parameters(source);
    wave_sound = create_wave_sound("..\\samples\\speech.wav");
    phase = get_phase_distribution_wave_parameters(wave_parameters);
    set_source_wave_parameters(wave_parameters, wave_sound);
    set_single_component_distribution(phase, DISTRIBUTION_TYPE_UNIFORM, 0.0, 1, DISTRIBUTION_POLARITY_POSITIVE, 0);        
    set_rate_wave_parameters(wave_parameters, 0.5);
    set_phase_mode_wave_parameters(wave_parameters, WAVEGRAIN_PHASE_GRAINTIME);
    pitch = get_pitch_shift_distribution_wave_parameters(wave_parameters);
    set_single_component_distribution(pitch, DISTRIBUTION_TYPE_GAUSSIAN, 0.0, 0.0, DISTRIBUTION_POLARITY_UNCHANGED, 0);    
    set_grain_source(source, create_wavegrain, init_wavegrain, destroy_wavegrain, fill_wavegrain, wave_parameters);
}


void test_glissgrain(GrainStream *stream)
{
    Distribution *frequency;
    GlissGrainParameters *gliss_parameters;
    GrainSource *source;    
    source = create_grain_source();
    add_source_stream(stream, source);  
    
    gliss_parameters = create_gliss_parameters(source);
    frequency = get_frequency_distribution_gliss_parameters(gliss_parameters);                
    set_single_component_distribution(frequency, DISTRIBUTION_TYPE_UNIFORM, 90.0, 0.0, DISTRIBUTION_POLARITY_POSITIVE, 0);    
    set_single_component_distribution(get_frequency_target_distribution_gliss_parameters(gliss_parameters), DISTRIBUTION_TYPE_UNIFORM, 2000.0, 10.0, DISTRIBUTION_POLARITY_UNCHANGED, 0);    
    
    
    set_constant_distribution(get_time_distribution_gliss_parameters(gliss_parameters), 1.0);
    set_constant_distribution(get_noise_distribution_gliss_parameters(gliss_parameters), 0.0);
    set_constant_distribution(get_noise_target_distribution_gliss_parameters(gliss_parameters), 2.0);
    set_constant_distribution(get_noise_bandwidth_distribution_gliss_parameters(gliss_parameters), 500.0);
    
    //set_single_component_distribution(get_time_distribution_gliss_parameters(gliss_parameters), DISTRIBUTION_TYPE_UNIFORM, 440.0, 1000.0, DISTRIBUTION_POLARITY_POSITIVE, 0);    
    
    set_grain_source(source, create_glissgrain, init_glissgrain, destroy_glissgrain, fill_glissgrain, gliss_parameters);

}



void test_padsyngrain(GrainStream *stream)
{
    Distribution  *pitch;
    WaveSound *wave_sound;
    PadSynGrainParameters *padsyn_parameters;    
    GrainSource *source;    
    source = create_grain_source();
    add_source_stream(stream, source);  
        
    padsyn_parameters = create_padsyn_parameters(source);
    wave_sound = create_wave_sound("..\\samples\\right.wav");    
    set_wave_padsyn_parameters(padsyn_parameters, wave_sound, 131072*4, 0.0);        
    pitch = get_pitch_shift_distribution_padsyn_parameters(padsyn_parameters);
    
    //set_discrete_distribution(pitch, pitches, NULL, 3);
    set_single_component_distribution(pitch, DISTRIBUTION_TYPE_GAUSSIAN, -0.0, 0.0, DISTRIBUTION_POLARITY_UNCHANGED, 0);    
    set_grain_source(source, create_padsyngrain, init_padsyngrain, destroy_padsyngrain, fill_padsyngrain, padsyn_parameters);
}

void test_pluckgrain(GrainStream *stream)
{
    Distribution *frequency;
    WaveSound *wave_sound;
    PluckGrainParameters *pluck_parameters;    
    GrainSource *source;    
    source = create_grain_source();
    add_source_stream(stream, source);  
    
    pluck_parameters = create_pluck_parameters(source);
    wave_sound = create_wave_sound("excite-plucked.wav");
    
    frequency = get_frequency_distribution_pluck_parameters(pluck_parameters);                
    set_single_component_distribution(frequency, DISTRIBUTION_TYPE_UNIFORM, 130.0, 240.0, DISTRIBUTION_POLARITY_POSITIVE, 0);    
    //set_impulse_pluck_parameters(pluck_parameters, wave_sound);
    set_grain_source(source, create_pluckgrain, init_pluckgrain, destroy_pluckgrain, fill_pluckgrain, pluck_parameters);


}


void test_multisinegrain(GrainStream *stream)
{   
    Distribution *frequency;
    GrainSource *source;    
    MultiSineGrainParameters *multisine_parameters;
    source = create_grain_source();
    add_source_stream(stream, source);  

    multisine_parameters = create_multisine_parameters(source);
    frequency = get_frequency_distribution_multisine_parameters(multisine_parameters);                
    set_single_component_distribution(frequency, DISTRIBUTION_TYPE_UNIFORM, 120.0, 30.0, DISTRIBUTION_POLARITY_POSITIVE, 0);    
    set_single_component_distribution(get_n_sines_distribution_multisine_parameters(multisine_parameters), DISTRIBUTION_TYPE_UNIFORM, 1.0, 16.0, DISTRIBUTION_POLARITY_POSITIVE, 0);    
    set_single_component_distribution(get_amplitude_distribution_multisine_parameters(multisine_parameters), DISTRIBUTION_TYPE_UNIFORM, -10.0, 20.0, DISTRIBUTION_POLARITY_NEGATIVE, 0);    
    set_single_component_distribution(get_decay_distribution_multisine_parameters(multisine_parameters), DISTRIBUTION_TYPE_UNIFORM, 0.0, 1.0, DISTRIBUTION_POLARITY_POSITIVE, 0);    
        
    set_grain_source(source, create_multisinegrain, init_multisinegrain, destroy_multisinegrain, fill_multisinegrain, multisine_parameters);  
}



