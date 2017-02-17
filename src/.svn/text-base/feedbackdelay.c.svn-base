/**    
    @file feedbackdelay.c
    @brief
    @author John Williamson
    
    Copyright (c) 2011 All rights reserved.
    Licensed under the BSD 3 clause license. See COPYING.
    
    This file is part of the OpenGrain distribution.
    http://opengrain.sourceforge.net   
*/              

#include "feedbackdelay.h"


// Create a feedback delay unit
FeedbackDelay *create_feedback_delay(void)
{
    FeedbackDelay *fbk_delay = malloc(sizeof(*fbk_delay));
    
    fbk_delay->delay = create_delay();
    set_delay(fbk_delay->delay, 2);
    fbk_delay->feedback = 0.0;
    fbk_delay->wet_level = 0.5;
    fbk_delay->dry_level = 0.5;
    fbk_delay->filter = create_SVF(); 
    return fbk_delay;
}


void set_length_feedback_delay(FeedbackDelay *fbk_delay, double delay)
{
    set_delay(fbk_delay->delay, delay*GLOBAL_STATE.sample_rate);
}

void set_feedback_feedback_delay(FeedbackDelay *fbk_delay, double feedback)
{
    fbk_delay->feedback = feedback;
}

void set_levels_feedback_delay(FeedbackDelay *fbk_delay, double wet, double dry)
{
    fbk_delay->wet_level = wet;
    fbk_delay->dry_level = dry;   
}


void set_filter_feedback_delay(FeedbackDelay *fbk_delay, SVF *filter)
{
    fbk_delay->filter = filter;
}

SVF *get_filter_feedback_delay(FeedbackDelay *fbk_delay)
{
    return fbk_delay->filter;
}

// Destroy the feedback delay unit
void destroy_feedback_delay(FeedbackDelay *fbk_delay)
{
    destroy_delay(fbk_delay->delay);
    destroy_SVF(fbk_delay->filter);
    free(fbk_delay);
}


// Process one sample of the feedback delay unit
float process_feedback_delay(FeedbackDelay *fbk_delay, float sample)
{
    float x,y;
    x = delay_out(fbk_delay->delay);
    
    // Filter, if required
    if(fbk_delay->filter!=NULL)
        x = compute_SVF(fbk_delay->filter, sample);
        
    y = x * fbk_delay->feedback + sample;
    
    // Feedback
    delay_in(fbk_delay->delay, y);
    return y*fbk_delay->wet_level + sample * fbk_delay->dry_level;
}



