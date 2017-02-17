/**    
    @file distributions.h
    @brief
    @author John Williamson
    
    Copyright (c) 2011 All rights reserved.
    Licensed under the BSD 3 clause license. See COPYING.
    
    This file is part of the OpenGrain distribution.
    http://opengrain.sourceforge.net   
*/              

#ifndef __DISTRIBUTIONS_H__
#define __DISTRIBUTIONS_H__

#include <math.h>
#include "single_distribution.h"
#include "rand.h"
#include "simclist.h"
#include "random.h"
//#include "discrete_distribution.h"





#define DISTRIBUTION_POLARITY_UNCHANGED 0
#define DISTRIBUTION_POLARITY_POSITIVE 1
#define DISTRIBUTION_POLARITY_NEGATIVE 2
#define DISTRIBUTION_POLARITY_RANDOM_SYMMETRIC 3

#define DISTRIBUTION_MIXTURE_STOCHASTIC 0
#define DISTRIBUTION_MIXTURE_SEQUENTIAL 1

struct Distribution;

typedef float (*DistributionTransform)(float);
typedef float (*DistributionExtendedTransform)(float,void *,struct Distribution *);



typedef struct Distribution
{
    double value; // special case for "delta" distribution!
    list_t *sequence_values;
    list_t *mixtures;       
    int mixture_mode;
    int mixture_index;
    
    int integer_mode;
    void *transformer;
    void *transformer_data;       
} Distribution;


Distribution *create_distribution(void);
void destroy_distribution(Distribution *distribution);

void set_constant_distribution(Distribution *distribution, double value);
void add_temporary_sequence_distribution(Distribution *distribution, double *values, int n_values);
void set_mixture_mode_distribution(Distribution *distribution, int mode);


void set_integer_mode_distribution(Distribution *distribution, int mode);

void set_discrete_distribution(Distribution *distribution, double *values, double *weights, int n_values);

void set_single_component_distribution(Distribution *distribution,int type, double mean, double scale, int polarity, double shape);

void set_transformation_function_distribution(Distribution *distribution, DistributionTransform *transformer);
void set_extended_transformation_function_distribution(Distribution *distribution, DistributionExtendedTransform *transformer, void *transformer_data);


list_t *get_component_list_distribution(Distribution *distribution);


SingleDistribution *get_component_distribution(Distribution *distribution, int index);
double get_component_weight_distribution(Distribution *distribution, int component);
void set_component_weight_distribution(Distribution *distribution, int component, double weight);
void add_component_distribution(Distribution *distribution);
void remove_component(Distribution *distribution, int index);
void remove_all_components_distribution(Distribution *distribution);
void normalize_weights_distribution(Distribution *distribution);

double sample_from_distribution(Distribution *distribution);


#endif