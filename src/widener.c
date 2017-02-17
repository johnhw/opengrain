/**    
    @file widener.c
    @brief
    @author John Williamson
    
    Copyright (c) 2011 All rights reserved.
    Licensed under the BSD 3 clause license. See COPYING.
    
    This file is part of the OpenGrain distribution.
    http://opengrain.sourceforge.net   
*/              

#include "widener.h"

//Create a stereo widener
Widener *create_widener()
{
    Widener *widener = malloc(sizeof(*widener));
    widener->mix_amount = 0.0;
    widener->delay_length = 0.0;
    widener->left_delay =  create_delay();
    widener->right_delay =  create_delay();    
    return widener;
}

// destroy a stereo widener and free its delay lines
void destroy_widener(Widener *widener)
{
    destroy_delay(widener->left_delay);
    destroy_delay(widener->right_delay);
    free(widener);
}
  
  
// set the stereo widener properties, with a delay in seconds, and a mix fraction
// delays of 0.5--10 ms and mix fractions of 0.0 -> 0.1 work well
void set_widener(Widener *widener, float delay, float mix)
{
    widener->mix_amount = mix;
    widener->delay_length = delay;
    set_delay(widener->left_delay, delay*GLOBAL_STATE.sample_rate);
    set_delay(widener->right_delay, delay*GLOBAL_STATE.sample_rate);    
}


// compute the result of the widener
void compute_widener(Widener *widener, float l, float r, float *l_out, float *r_out)
{
    float dl, dr;
    
    // delay the incoming left and right signals
    dl = get_delay(widener->left_delay, l);
    dr = get_delay(widener->right_delay, r);
    
    // feed the inverted delayed signals into the opposite channels mix
    *r_out = r * (1-widener->mix_amount) + widener->mix_amount * -dl;
    *l_out = l * (1-widener->mix_amount) + widener->mix_amount * -dr;        
}
