/**    
    @file distributions.c
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



// Sample from a normalised discrete distribution
SingleDistribution *sample_from_weight_distribution(Distribution *distribution)
{
    double cum,r;
   
    SingleDistribution *sd, *result;
    
    r = uniform_double();
    cum = 0.0;
    result = NULL;
    list_iterator_start(distribution->mixtures);    
    while(list_iterator_hasnext(distribution->mixtures))
    {
        sd = list_iterator_next(distribution->mixtures);
        cum += sd->weight;
        if(r<cum)
        {
            result = sd;
            break;
        }
    }
    list_iterator_stop(distribution->mixtures);
            
    return result;
}


// Normalise a discrete distribution
void normalise_weight_distribution(Distribution *distribution)
{
    double cum;
    SingleDistribution *sd;
    
    cum = 0.0;
    
    list_iterator_start(distribution->mixtures);    
    while(list_iterator_hasnext(distribution->mixtures))
    {
        sd = list_iterator_next(distribution->mixtures);
        cum += sd->weight;        
    }
    list_iterator_stop(distribution->mixtures);
    
    
    list_iterator_start(distribution->mixtures);    
    while(list_iterator_hasnext(distribution->mixtures))
    {
        sd = list_iterator_next(distribution->mixtures);
        sd->weight /= cum;
                
    }
    list_iterator_stop(distribution->mixtures);
          
}


// Create an empty distribution
Distribution *create_distribution(void)
{
    Distribution *distribution;
    distribution = malloc(sizeof(*distribution));
    distribution->mixtures = malloc(sizeof(*distribution->mixtures));
    list_init(distribution->mixtures);    
    distribution->value = HUGE_VAL;
    distribution->mixture_mode = DISTRIBUTION_MIXTURE_STOCHASTIC;
    distribution->mixture_index = 0;
    distribution->sequence_values = malloc(sizeof(*distribution->sequence_values));
    distribution->transformer = NULL;
    distribution->transformer_data = NULL;
    
    list_init(distribution->sequence_values);
    list_attributes_copy(distribution->sequence_values, list_meter_double, 1);
    distribution->integer_mode = 0;
    return distribution;
}

// Destroy a mixture of components
void destroy_distribution(Distribution *distribution)
{
    SingleDistribution *sd;
    
    list_iterator_start(distribution->mixtures);    
    while(list_iterator_hasnext(distribution->mixtures))
    {
        sd = list_iterator_next(distribution->mixtures);
        destroy_single_distribution(sd);    
    }
    list_destroy(distribution->mixtures);
    
    free(distribution->mixtures);
    free(distribution);
        
}


// add a sequence of values to a distribution such that they will be used for the next
// draws from the distribution
void add_temporary_sequence_distribution(Distribution *distribution, double *values, int n_values)
{
    int i;
    for(i=0;i<n_values;i++)    
        list_append(distribution->sequence_values, &values[i]);
    

}

// set the mixture mode: whether to choose components randomly (_STOCHASTIC) or sequentially in a loop
void set_mixture_mode_distribution(Distribution *distribution, int mode)
{
    distribution->mixture_mode = mode;
}

// set whether the distribution only returns integer values, or whether it returns fractional values
void set_integer_mode_distribution(Distribution *distribution, int mode)
{   
    distribution->integer_mode = mode;
}


// get the list of mixtures
list_t *get_component_list_distribution(Distribution *distribution)
{
    return distribution->mixtures;
}

// utility function, creates a distribution with just one component
void set_single_component_distribution(Distribution *distribution, int type, double mean, double scale, int polarity, double shape)
{    
    SingleDistribution *sd;    
    remove_all_components_distribution(distribution);
    add_component_distribution(distribution);    
    distribution->value = HUGE_VAL;
    sd = get_component_distribution(distribution,0);
    sd->weight = 1.0;
    sd->type = type;
    sd->mean = mean;
    sd->scale = scale;
    sd->polarity = polarity;
    sd->shape = shape;
            

}


// discrete distribution
// if weights are not NULL, specify weights of each component
// else weights are 1/n
void set_discrete_distribution(Distribution *distribution, double *values, double *weights, int n_values)
{
    int i;
    SingleDistribution *sd;
    
    // remove old components, if any
    remove_all_components_distribution(distribution);
    
    for(i=0;i<n_values;i++)
    {       
        add_component_distribution(distribution);
        sd = get_component_distribution(distribution, i);
        sd->type = DISTRIBUTION_TYPE_CONSTANT;
        sd->mean = values[i];
        if(weights!=NULL)    
            sd->weight = weights[i];
    
        else        
            sd->weight = 1.0/(double)n_values;       
    }
    
}


// Create a special distribution with just one value (i.e. a constant)
void set_constant_distribution(Distribution *distribution, double value)
{    
    distribution->value = value;    
}

// Add a component to the distribution
void add_component_distribution(Distribution *distribution)
{

    SingleDistribution *sd;
    distribution->value = HUGE_VAL;
    sd = create_single_distribution();
    list_append(distribution->mixtures, sd);        
}




// return a pointer to a distribution element
SingleDistribution *get_component_distribution(Distribution *distribution, int component)
{
   SingleDistribution *sd;
   if(component>=0 && component<list_size(distribution->mixtures) )
   {
    sd = list_get_at(distribution->mixtures, component);
    return sd; 
   }
   return NULL;
}

// remove an element from the mixture
void remove_component_distribution(Distribution *distribution, int component)
{   
    SingleDistribution *sd;
    if(component>=0 && component<list_size(distribution->mixtures))
    {
        sd = list_extract_at(distribution->mixtures, component);
        destroy_single_distribution(sd);
    }
    
}


// remove all the components from a mixture
void remove_all_components_distribution(Distribution *distribution)
{
    while(list_size(distribution->mixtures)>0)    
        remove_component_distribution(distribution, 0);    
    
}


// Set the weight of a component, and renormalise the distribution 
void set_component_weight_distribution(Distribution *distribution, int component, double weight)
{
    SingleDistribution *sd;
    sd = list_get_at(distribution->mixtures, component);
    sd->weight = weight;    
    normalise_weight_distribution(distribution);
}


// get the weight of a component
double get_component_weight_distribution(Distribution *distribution, int component)
{
    SingleDistribution *sd;
    sd = list_get_at(distribution->mixtures, component);    
    return sd->weight;
}


// set the transformation function for a distribution (e.g. decibels to ratio)
void set_transformation_function_distribution(Distribution *distribution, DistributionTransform *transformer)
{
    distribution->transformer = transformer;
    distribution->transformer_data = NULL;
}

// set the transformation function for a distribution (e.g. decibels to ratio). Data must not be NULL!
void set_extended_transformation_function_distribution(Distribution *distribution, DistributionExtendedTransform *transformer, void *transformer_data)
{
   distribution->transformer = transformer;      
   distribution->transformer_data = transformer_data;
}



// Draw a sample from a mixture of continuous distributions, each of which
// has a type (distribution), scale, mean/center and optionally a polarity adjustment (e.g. positive only)
double sample_from_distribution(Distribution *distribution)
{
    SingleDistribution *sd;    
    double result;
    
    
    // return a temporary sequence value, and remove it from the list
    if(list_size(distribution->sequence_values)>0)
    {
        double *result_ptr;
        
        result_ptr = list_get_at(distribution->sequence_values, 0);
        result = *result_ptr;
        list_delete_at(distribution->sequence_values, 0);            
        goto return_result;
    }
    
    
    // return the value, if this distribution has one
    if(distribution->value!=HUGE_VAL)
    {
        result = distribution->value;
        goto return_result;
    }
                    
    
    if(distribution->mixture_mode == DISTRIBUTION_MIXTURE_STOCHASTIC)
    {
        // randomly select a mixture
        sd = sample_from_weight_distribution(distribution);            
    }
    else
    {
        // sequentially select a mixture
        int len;
        len = list_size(distribution->mixtures);
        
        if(distribution->mixture_index>=len)
            distribution->mixture_index = 0;
            
        sd = list_get_at(distribution->mixtures, distribution->mixture_index);
        distribution->mixture_index++;
    }
    
    if(sd==NULL) return NAN;
    
    
    result = sample_from_single_distribution(sd);
            
    // Adjust the sign of the result as required
    // Note: abs() is taken before scale/shift
    if(sd->polarity==DISTRIBUTION_POLARITY_UNCHANGED)
    {
        result = result*sd->scale + sd->mean;    
    }
    if(sd->polarity==DISTRIBUTION_POLARITY_POSITIVE)
    {
        result = fabs(result)*sd->scale + sd->mean;    
    }
       
    if(sd->polarity==DISTRIBUTION_POLARITY_NEGATIVE)
    {
        result = -fabs(result)*sd->scale + sd->mean;    
    }
    
    // allows positive only distributions to generate signed results (e.g. sampling from Laplace)
    if(sd->polarity==DISTRIBUTION_POLARITY_RANDOM_SYMMETRIC)
    {
        double x = uniform_double();
        if(x<0.5)        
            result = -fabs(result)*sd->scale + sd->mean;    
        else
            result = fabs(result)*sd->scale + sd->mean;    
    }
   
   
    // apply post transformation
    return_result:
    
    if(distribution->integer_mode)
        result = floor(result+0.5);
    
    
    // apply post-transformation
    if(distribution->transformer)
    {
            // extended, if there is a data component
            if(distribution->transformer_data)
            {
                DistributionExtendedTransform transform;
                transform = (DistributionExtendedTransform)distribution->transformer;                
                result = transform(result, distribution->transformer_data, distribution);
            }
            else
            {
                DistributionTransform transform;
                transform = (DistributionTransform)(distribution->transformer);             
                result = transform(result);                
            }
        
    }
    
    
    return result;


}


