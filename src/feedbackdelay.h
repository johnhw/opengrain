/**    
    @file feedbackdelay.h
    @brief
    @author John Williamson
    
    Copyright (c) 2011 All rights reserved.
    Licensed under the BSD 3 clause license. See COPYING.
    
    This file is part of the OpenGrain distribution.
    http://opengrain.sourceforge.net   
*/              

#ifndef __FEEDBACK_DELAY_H__
#define __FEEDBACK_DELAY_H__
#include "audio.h"
#include "delayline.h"
#include "svf.h"


typedef struct FeedbackDelay
{    
    DelayLine *delay;
    float feedback;
    float wet_level;
    float dry_level;
    SVF *filter;
} FeedbackDelay;

// Create a feedback delay unit
FeedbackDelay *create_feedback_delay();

void set_length_feedback_delay(FeedbackDelay *fbk_delay, double delay);
void set_feedback_feedback_delay(FeedbackDelay *fbk_delay, double feedback);
void set_levels_feedback_delay(FeedbackDelay *fbk_delay, double wet, double dry);
SVF *get_filter_feedback_delay(FeedbackDelay *fbk_delay);

void destroy_feedback_delay(FeedbackDelay *fbk_delay);
float process_feedback_delay(FeedbackDelay *delay, float sample);

#endif

