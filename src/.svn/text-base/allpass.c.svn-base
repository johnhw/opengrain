/**    
    @file allpass.c
    @brief
    @author John Williamson
    
    Copyright (c) 2011 All rights reserved.
    Licensed under the BSD 3 clause license. See COPYING.
    
    This file is part of the OpenGrain distribution.
    http://opengrain.sourceforge.net   
*/              

#include "allpass.h"


// Create a fractional delay line, using an allpass filter for the delay
ThiranAllpass *create_thiran_allpass()
{
    ThiranAllpass *allpass = malloc(sizeof(*allpass));
    allpass->x1 = 0;
    allpass->y1 = 0;
    allpass->integer_delay = create_delay();    
    allpass->delay = 0;
    set_thiran_allpass_delay(allpass, 0.01);    
    return allpass;
}


// Destroy an allpass delay line
void destroy_thiran_allpass(ThiranAllpass *allpass)
{
    destroy_delay(allpass->integer_delay);
    free(allpass);
}


// Set the total delay of the fractional delay line
void set_thiran_allpass_delay(ThiranAllpass *allpass, float delay)
{
    float fractional_part;
    
        
        delay = delay * GLOBAL_STATE.sample_rate;
        
        fractional_part = delay - floor(delay);
        allpass->delay = delay;
        
        // restrict the allpass filter to delays in [0.3, 1.3]
        if(fractional_part<0.3)
        {
            set_delay(allpass->integer_delay, (int)floor(delay)-1);        
            fractional_part += 1.0;
            allpass->tau = (1-fractional_part)/(1+fractional_part);    
        
        }
        else
        {
            set_delay(allpass->integer_delay, (int)floor(delay));        
            allpass->tau = (1-fractional_part)/(1+fractional_part);    
        }
    
    
}

// get the current output of the delay line
float get_thiran_allpass(ThiranAllpass *allpass)
{
    return allpass->y1;

}


// put a sample through the fractional delay line
void insert_thiran_allpass(ThiranAllpass *allpass, float sample)
{
    float x, y;
    x = delay(allpass->integer_delay, sample);
    
    
    y = allpass->tau*(x-allpass->y1)+allpass->x1;
    allpass->x1 = x;
    allpass->y1 = y;       
}



// Pass a sample through the fractional delay
float process_thiran_allpass(ThiranAllpass *allpass, float x)
{

    insert_thiran_allpass(allpass, x);
    return get_thiran_allpass(allpass);
}


