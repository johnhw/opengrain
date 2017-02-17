/**    
    @file grain.c
    @brief
    @author John Williamson
    
    Copyright (c) 2011 All rights reserved.
    Licensed under the BSD 3 clause license. See COPYING.
    
    This file is part of the OpenGrain distribution.
    http://opengrain.sourceforge.net   
*/              

#include "grain.h"
#include "grain_source.h"
#include "spatializer.h"


// Create an empty grain structure
Grain *create_grain(void)
{
    Grain *grain;
    grain = malloc(sizeof(*grain));
    grain->amplitude = 1.0;
    grain->envelope = create_envelope();    
    grain->duration = 0.0;
    grain->duration_samples = 0;
    grain->samples_passed = 0;
    grain->specifics = NULL;
    grain->finished = 0;
    grain->source = NULL;
    grain->frequency = 0;
    grain->location = create_location();
    set_cartesian_location(grain->location, 0, 0, 0);
    return grain;
}


// mark a grain as finished
void finish_grain(Grain *grain)
{
    grain->finished =1;
}


// reset grain, and update duration in samples
void reset_grain(Grain *grain)
{
    grain->duration_samples = grain->duration * GLOBAL_STATE.sample_rate;
    grain->samples_passed = 0;
    grain->finished = 0;
}


// Delete a grain
void destroy_grain(Grain *grain)
{   
    destroy_envelope(grain->envelope);
    free(grain);
}


