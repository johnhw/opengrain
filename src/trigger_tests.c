/**    
    @file trigger_tests.c
    @brief
    @author John Williamson
    
    Copyright (c) 2011 All rights reserved.
    Licensed under the BSD 3 clause license. See COPYING.
    
    This file is part of the OpenGrain distribution.
    http://opengrain.sourceforge.net   
*/              

#include "trigger_tests.h"
#include "output.h"

void test_impulsetrigger(OutputInfo *info, GrainStream *stream)
{
    Trigger *trigger;
    ImpulseTrigger *impulse_trigger;    
    trigger = create_trigger();
    set_grain_stream_trigger(trigger, stream);                        
    impulse_trigger = create_impulse_trigger();
    set_processor_trigger(trigger, process_impulse_trigger, impulse_trigger);       
    set_filter_impulse_trigger(impulse_trigger, 3500.0, 10.0);
    set_threshold_impulse_trigger(impulse_trigger, -31.0, 0.001);
    set_deriv_threshold_impulse_trigger(impulse_trigger, 500.0, 0.001);
    set_inhibit_impulse_trigger(impulse_trigger, 0.01);                    
    connect_live_trigger(info, trigger);
}


void test_pitchtrigger(OutputInfo *info, GrainStream *stream)
{
    Trigger *trigger;
    PitchTrigger *pitch_trigger;    
    trigger = create_trigger();
    set_grain_stream_trigger(trigger, stream);          
    pitch_trigger = create_pitch_trigger(128);
    set_processor_trigger(trigger, process_pitch_trigger, pitch_trigger);       
    set_minimum_level_pitch_trigger(pitch_trigger, -15.0);
    set_copy_amplitude_pitch_trigger(pitch_trigger, 1, -10.0);
    connect_live_trigger(info, trigger);
}



