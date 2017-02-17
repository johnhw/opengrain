/**    
    @file pool.h
    @brief
    @author John Williamson
    
    Copyright (c) 2011 All rights reserved.
    Licensed under the BSD 3 clause license. See COPYING.
    
    This file is part of the OpenGrain distribution.
    http://opengrain.sourceforge.net   
*/              

#ifndef __POOL_H__
#define __POOL_H__
typedef struct PoolEntry 
{  
    void *data
    int free;
} PoolEntry;

// a pool of data that can be expanded and can allocate data as required
typdef struct Pool
{
    int n_entries;
    int n_free;
    PoolEntry *elements;
    void *(*)create_fn;
    void (*)destroy_fn;  
}
Pool;

Pool *create_pool(int initial_capacity, void *(*)create_fn, void (*)destroy_fn);
void expand_pool(Pool *pool);
void *get_from_pool(Pool *pool);
void destroy_pool(Pool *pool);


#endif