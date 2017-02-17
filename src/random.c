/**    
    @file random.c
    @brief Random number generation, and transformations from uniform to other distributions.
    @author John Williamson
    
    Provides samples from uniform, Gaussian, Cauchy and Gamma distributions.
    
    Copyright (c) 2011 All rights reserved.
    Licensed under the BSD 3 clause license. See COPYING.
    
    This file is part of the OpenGrain distribution.
    http://opengrain.sourceforge.net   
*/              

#include "random.h"

static randctx *global_rand_stream; 

/** Initialise the random number generator. Uses a static variable 
    to hold the random stream. 
    @arg seed The initial seed of the RNG */
void init_random(int seed)
{    
    global_rand_stream = malloc(sizeof(*global_rand_stream));
    seed_random(seed);
}

/** Reseed the random number generator. 
    @arg seed The new seed of the RNG */
void seed_random(int seed)
{
    int i;
    for(i=0;i<RANDSIZ;i++)    
        global_rand_stream->randrsl[i] = seed | i;    
    randinit(global_rand_stream,1);
}


/* Return a random number in range [0.0, 1.0).
    @return A random double from 0.0 -> 1.0 */
double uniform_double(void)
{
    
    double d;    
    d = rand(global_rand_stream)/(double)0x100000000;
    return d;
}


/** Return a random number between a and b.
    @return An integer >= a and < b. */
int random_int(int a, int b)
{
    return (int)(uniform_double()*(b-a) + a);
}

/** Return a random number with a unit normal distribution (mean=0, std. dev=1.0).
    @return A standard normally distributed double */
double gaussian_double (void)
{
    double multiplier;
    double v1, v2, s;
    do
	{
	  v1 = 2 * uniform_double () - 1;
	  v2 = 2 * uniform_double () - 1;
	  s = v1 * v1 + v2 * v2;
	}
    while (s >= 1 || s == 0);

    multiplier = sqrt (-2 * log (s) / s);
    return v1 * multiplier;    
}


/** Return a random number with a Cauchy distribution, centered on 0. Useful when long tails are needed.
    @return A standard Cauchy distributed double. */
double cauchy_double(void)
{
    double x,y;
    x = gaussian_double();
    y = gaussian_double();
    return x/y;
}

/** Return a random number with an exponential distribution with a lambda=1.0.
    @return An exponentially distribution random double */
double exp_double(void)
{
    return -log(uniform_double());    
}

/** Return a random number with a gamma distribution. Uses Fishman's rejection method to sample.
    @arg shape Shape of the gamma distribution. Must be >0.0. Useful values are usually around 0.1 -> 10.0.
    @return A gamma distribution double with the given shape. */
double gamma_double(double shape)
{
    double v, w;    
    do
    {
        v = exp_double();
        w = exp_double();
    }    
    while(w<(shape-1)*(v-log(v)-1));
    return shape*v;        
}

