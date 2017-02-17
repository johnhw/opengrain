/**    
    @file trigger_tests.h
    @brief
    @author John Williamson
    
    Copyright (c) 2011 All rights reserved.
    Licensed under the BSD 3 clause license. See COPYING.
    
    This file is part of the OpenGrain distribution.
    http://opengrain.sourceforge.net   
*/              

#ifndef __TRIGGER_TESTS_H__
#define __TRIGGER_TESTS_H__
#include "audio.h"
#include "grain_stream.h"
#include "grainmixer.h"
#include "trigger.h"
#include "pitchtrigger.h"
#include "impulsetrigger.h"

struct OutputInfo;
void test_impulsetrigger(struct OutputInfo *info, GrainStream *stream);
void test_pitchtrigger(struct  OutputInfo *info, GrainStream *stream);



#endif 