/**    
    @file grain_tests.h
    @brief
    @author John Williamson
    
    Copyright (c) 2011 All rights reserved.
    Licensed under the BSD 3 clause license. See COPYING.
    
    This file is part of the OpenGrain distribution.
    http://opengrain.sourceforge.net   
*/              

#ifndef __GRAIN_TESTS_H__
#define __GRAIN_TESTS_H__
#include "audio.h"
#include "grain_stream.h"
#include "grainmixer.h"

void test_default_grain_generation(GrainStream *stream);
void test_default_mixer_settings(GrainMixer *mixer);

void test_sinegrain(GrainStream *stream);
void test_noisegrain(GrainStream *stream);
void test_impulsegrain(GrainStream *stream);
void test_fmgrain(GrainStream *stream);
void test_analoggrain(GrainStream *stream);
void test_dsfgrain(GrainStream *stream);
void test_wavegrain(GrainStream *stream);
void test_glissgrain(GrainStream *stream);
void test_padsyngrain(GrainStream *stream);
void test_pluckgrain(GrainStream *stream);
void test_multisinegrain(GrainStream *stream);
void test_voicegrain(GrainStream *stream);
void test_hrtf(void);


#endif 