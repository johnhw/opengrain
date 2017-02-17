/**    
    @file rc_generator.c
    @brief
    @author John Williamson
    
    Copyright (c) 2011 All rights reserved.
    Licensed under the BSD 3 clause license. See COPYING.
    
    This file is part of the OpenGrain distribution.
    http://opengrain.sourceforge.net   
*/              

#include "rc_generator.h"


// create an object that interpolates between a start and an end point
// at a given rate
RCGenerator *create_rc_generator()
{
    RCGenerator *generator  = malloc(sizeof(*generator));    
    generator->start = create_distribution();
    generator->end = create_distribution();
    generator->rate = create_distribution();
    generator->val = RC_NOT_CONSTANT;
    return generator;
    
}

// set a constant val, or RC_NOT_CONSTANT to disable the val
void set_constant_rc_generator(RCGenerator *generator, float val)
{
    generator->val = val;
}

// destroy an RC generator
void destroy_rc_generator(RCGenerator *generator)
{
    free(generator->start);
    free(generator->end);
    free(generator->rate);    
    free(generator);
}


// get the start val distribution
Distribution *get_start_rc(RCGenerator *generator)
{
    return generator->start;
}

// get the end (target) val distribution
Distribution *get_end_rc(RCGenerator *generator)
{
    return generator->end;
}

// get the rate distribution
Distribution *get_rate_rc(RCGenerator *generator)
{
    return generator->rate;
}

// create an unitialised rc interpolator
RC *create_rc()
{
    RC *rc;
    rc = malloc(sizeof(*rc));
 
    return rc;
}

// get an object which interpolates from start to end exponentially
void set_rc(RCGenerator *generator, RC *rc)
{    
    if(generator->val != RC_NOT_CONSTANT)
    {
        rc->val = generator->val;
        rc->target = generator->val;
        rc->coeff = 1.0;
    }
    else
    {        
        rc->val = sample_from_distribution(generator->start);
        rc->target = sample_from_distribution(generator->end);
        rc->coeff = rc_time(sample_from_distribution(generator->rate), -80.0);         
    }

}


// set the parameters of an RC interpolator manually
void set_rc_parameters(RC *rc, float start, float end, float rate)
{
    rc->val = start;
    rc->target = end;
    rc->coeff = rc_time(rate, -80.0);     
}

// free an interpolation object
void destroy_rc(RC *rc)
{
    free(rc);
}

// update the interpolation
float update_rc(RC *rc)
{
    rc->val = rc->coeff * rc->val + (1-rc->coeff) * rc->target;
    return rc->val;
    
}