/**    
    @file single_distribution.c
    @brief
    @author John Williamson
    
    Copyright (c) 2011 All rights reserved.
    Licensed under the BSD 3 clause license. See COPYING.
    
    This file is part of the OpenGrain distribution.
    http://opengrain.sourceforge.net   
*/              

#include <math.h>
#include <stdlib.h>

#include "distributions.h"



// create a struct representing a single distribution component
SingleDistribution *create_single_distribution()
{
    SingleDistribution *sd;
    sd = malloc(sizeof(*sd));    
    return sd;
}


// destroy a single distribution component
void destroy_single_distribution(SingleDistribution *sd)
{
    free(sd);    
}


// Draw a sample from a mixture of continuous distributions, each of which
// has a type (distribution), scale, mean/center and optionally a polarity adjustment (e.g. positive only)
double sample_from_single_distribution(SingleDistribution *sd)
{
    double result;
    // Sample from the appropriate unit distribution for this component type
    switch(sd->type)
    {
        case DISTRIBUTION_TYPE_CONSTANT:
            result = 0;
            break;
        case DISTRIBUTION_TYPE_UNIFORM:
            result = uniform_double();
            break;
        case DISTRIBUTION_TYPE_GAUSSIAN:
            result = gaussian_double();
            break;
        case DISTRIBUTION_TYPE_CAUCHY:
            result = cauchy_double();
            break;
        case DISTRIBUTION_TYPE_EXPONENTIAL:
            result = exp_double();
            break;
        case DISTRIBUTION_TYPE_GAMMA:
            result = gamma_double(sd->shape);
            break;                    
    }
    return result;
}