/**    
    @file grain_source.c
    @brief
    @author John Williamson
    
    Copyright (c) 2011 All rights reserved.
    Licensed under the BSD 3 clause license. See COPYING.
    
    This file is part of the OpenGrain distribution.
    http://opengrain.sourceforge.net   
*/              

#include "grain_source.h"
#include "distributions.h"






void set_grain_source(GrainSource *source, create_grain_func create_func, init_grain_func init_func, destroy_grain_func destroy_func,
    fill_grain_func fill_func, void *source_data)
{
    source->source_data = source_data;
        
    // copy in the function pointers    
    source->create_grain = create_func;
    source->destroy_grain = destroy_func;
    source->fill_grain = fill_func;
    source->init_grain = init_func;
    
    source->valid = 1;
}



// Create a new grain source
GrainSource *create_grain_source(void)
{
    GrainSource *source;
    source = malloc(sizeof(*source));
    
    source->source_data = NULL;
    source->create_grain = NULL;
    source->destroy_grain = NULL;
    source->fill_grain = NULL;        
    source->valid = 0;
       
    source->specifics_list = malloc(sizeof(*source->specifics_list));
    list_init(source->specifics_list);
    source->dead_specifics_list = malloc(sizeof(*source->dead_specifics_list));
    list_init(source->dead_specifics_list);
                
    return source;
}


// create or return an already created specifics object
void *revive_specifics_source(GrainSource *source, Grain *grain)
{
    void *specifics;
    if(list_size(source->dead_specifics_list)>0)    
        specifics = list_extract_at(source->dead_specifics_list, 0);    
    else    
        specifics = source->create_grain(source);      

    source->init_grain(specifics, source->source_data, grain);
    return specifics;
}


// put a specifics object back onto the list
void kill_specifics_source(GrainSource *source, void *specifics)
{        
    int index;
    
    // remove from list
    index = list_locate(source->specifics_list, specifics);
    list_extract_at(source->specifics_list, index);           
    
    // put into the dead list
    list_append(source->dead_specifics_list, specifics);
}

// Destroy a complete GrainSource object
void destroy_source(GrainSource *source)
{

    void *specifics;
    
    // destroy the active specifcs
    list_iterator_start(source->specifics_list);    
    while(list_iterator_hasnext(source->specifics_list))
    {
        specifics = list_iterator_next(source->specifics_list);
        source->destroy_grain(specifics);    
    }
    list_iterator_stop(source->specifics_list);       
    list_destroy(source->specifics_list);
    free(source->specifics_list);
    
    // destroy the dead specifcs    
    list_iterator_start(source->dead_specifics_list);    
    while(list_iterator_hasnext(source->dead_specifics_list))
    {
        specifics = list_iterator_next(source->dead_specifics_list);
        source->destroy_grain(specifics);    
    }
    list_iterator_stop(source->dead_specifics_list);       
    list_destroy(source->dead_specifics_list);
    free(source->dead_specifics_list);
    
    
    // free the source data itself
    free(source);
    
    
}



