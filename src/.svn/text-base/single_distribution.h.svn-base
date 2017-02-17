/**    
    @file single_distribution.h
    @brief
    @author John Williamson
    
    Copyright (c) 2011 All rights reserved.
    Licensed under the BSD 3 clause license. See COPYING.
    
    This file is part of the OpenGrain distribution.
    http://opengrain.sourceforge.net   
*/              

#ifndef __SINGLE_DISTRIBUTION_H__
#define __SINGLE_DISTRIBUTION_H__

#include <math.h>
#include "simclist.h"
#include "random.h"




#define DISTRIBUTION_TYPE_CONSTANT -1
#define DISTRIBUTION_TYPE_UNIFORM 0
#define DISTRIBUTION_TYPE_GAUSSIAN 1
#define DISTRIBUTION_TYPE_CAUCHY 2
#define DISTRIBUTION_TYPE_EXPONENTIAL 3
#define DISTRIBUTION_TYPE_GAMMA 4



typedef struct SingleDistribution
{
    double weight;    
    int type;
    int polarity;    
    double mean, scale;
    double shape; // Only for gamma
} SingleDistribution;


SingleDistribution *create_single_distribution();
void destroy_single_distribution(SingleDistribution *sd);
double sample_from_single_distribution(SingleDistribution *sd);


#endif