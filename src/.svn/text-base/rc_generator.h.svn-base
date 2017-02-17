/**    
    @file rc_generator.h
    @brief
    @author John Williamson
    
    Copyright (c) 2011 All rights reserved.
    Licensed under the BSD 3 clause license. See COPYING.
    
    This file is part of the OpenGrain distribution.
    http://opengrain.sourceforge.net   
*/              

#ifndef __RC_GENERATOR_H__
#define __RC_GENERATOR_H__
#include "audio.h"
#include "distributions.h"
#include <stdlib.h>
#include <math.h>

#define RC_NOT_CONSTANT HUGE_VAL


typedef struct RCGenerator
{
    Distribution *start;
    Distribution *end;
    Distribution *rate;    
    float val;
} RCGenerator;

typedef struct RC
{
    
    float target;
    float val;
    float coeff;   
    
} RC;


RCGenerator *create_rc_generator();
void destroy_rc_generator(RCGenerator *generator);
Distribution *get_start_rc_generator(RCGenerator *generator);
Distribution *get_end_rc_generator(RCGenerator *generator);
Distribution *get_rate_rc_generator(RCGenerator *generator);
void set_constant_rc_generator(RCGenerator *generator, float value);
void resample_rc(RC *smooth);
RC *create_rc();


void set_rc_parameters(RC *rc, float start, float end, float rate);
void set_rc(RCGenerator *generator, RC *rc);
void destroy_rc(RC *smooth);
float update_rc(RC *smooth);


#endif